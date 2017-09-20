#include "StdAfx.h"
#include "MSEnumerator.h"
#include "AutoCriticalSection.h"
#include "EnumDisks.h"


using namespace DeviceDetectLibrary;
using namespace DeviceDetectLibrary::Connection;

CMSEnumerator::CMSEnumerator(ICollector& collector)
	: collector_(collector)
{
}

CMSEnumerator::~CMSEnumerator(void)
{
}

void CMSEnumerator::TryThis(const std::wstring& devicePath)
{
	CAutoCriticalSection lock(sec_);
	if(checkers_.count(devicePath) == 0)
	{
		CMSChecker::Pointer pMSChecker = CMSChecker::Create(collector_, devicePath);
		checkers_.insert(std::make_pair(devicePath, pMSChecker));
		//checkers_[devicePath] = pMSChecker;
	}
	// Cancel previous process
	checkers_[devicePath]->Stop();
	// Begin new process
	checkers_[devicePath]->Start();
}

void CMSEnumerator::RemoveThis(const std::wstring& devicePath)
{
	CAutoCriticalSection lock(sec_);
	if(checkers_.count(devicePath) != 0)
	{
		checkers_.erase(devicePath);
	}
}

void CMSEnumerator::Collect(const CDeviceInfo::Pointer& pDeviceInfo)
{
	try
	{
		Disk::Types::RemovableDeviceInfo_vt vctDisks;
		Disk::Functions::SearchRemovalDisks(vctDisks);
		for(Disk::Types::RemovableDeviceInfo_vt_cit iter = vctDisks.begin(); iter != vctDisks.end(); ++iter)
		{
			CConnectionInfo::Pointer pConnectionInfo = CConnectionInfo::Create();

			pConnectionInfo->Type = TypeUnknown;
			pConnectionInfo->FriendlyName = iter->wsFriendlyName;
			pConnectionInfo->DevicePath = iter->wsDevInterfaceVolume;
			pConnectionInfo->HardwareID = iter->wsHardwareID;
			pConnectionInfo->PhysicalDeviceName = iter->wsEnumeratorName;
			pConnectionInfo->DeviceDescription = iter->wsDeviceDesc;
			pConnectionInfo->ServiceName = iter->wsCompatibleIDs;

			//ConnectionInfo info = {TypeUnknown, iter->wsFriendlyName, iter->wsDevInterfaceVolume, 
			//	iter->wsHardwareID, iter->wsEnumeratorName, iter->wsDeviceDesc, iter->wsCompatibleIDs};

			CDeviceInfo::Pointer pDevice = CDeviceInfo::Create(iter->wsHardwareID, iter->wsFriendlyName, L"Mass Storage device", pConnectionInfo);
			collector_.Found(pDevice);
		}
	}
	catch(const std::exception& ex)
	{
		ex.what();
		// EXCEPTION: Cannot do SearchRemovalDisks(). Error = %s", ex.what());
	}
}
