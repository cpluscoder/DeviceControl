#include "StdAfx.h"
#include "MSChecker.h"
#include "EnumDisks.h"

using namespace DeviceDetectLibrary;
using namespace DeviceDetectLibrary::Connection;

CMSChecker::CMSChecker(ICollector& collector, const std::wstring& devicePath)
	: collector_(collector)
	, devicePath_(devicePath)
	, thread_(ThreadFunction)
{
}

CMSChecker::~CMSChecker(void)
{
	Stop();
}

void CMSChecker::Start(void)
{
	stopEvent_.Reset();
	thread_.Start(this);
}

void CMSChecker::Stop(void)
{
	stopEvent_.Set();
	thread_.Wait(INFINITE);
}

void CMSChecker::ThreadFunction(void* state)
{
	CMSChecker* This = static_cast<CMSChecker *>(state);
	if(This != NULL)
	{
		This->WorkThread();
	}
}

void CMSChecker::WorkThread(void)
{
	int tries = 60;
	const int sleepTimeout = 1000;
	Disk::Types::RemovableDeviceInfo_vt disks;
	while((tries--) > 0)
	{
		try
		{
			Disk::Functions::SearchRemovalDisks(disks);
			break;
		}
		catch(const std::exception& ex)
		{
			ex.what();
			// EXCEPTION: Cannot found drives for (%S)", devicePath_.c_str());
			if(stopEvent_.Wait(sleepTimeout))
			{
				return;
			}
		}
	}

	for(Disk::Types::RemovableDeviceInfo_vt_cit iter = disks.begin(); iter != disks.end(); ++iter)
	{
		if(devicePath_ == iter->wsPath)
		{
			CConnectionInfo::Pointer pConnectionInfo = CConnectionInfo::Create();
			pConnectionInfo->Type = TypeUnknown;
			pConnectionInfo->FriendlyName = iter->wsFriendlyName;
			pConnectionInfo->DevicePath = iter->wsDevInterfaceVolume;
			pConnectionInfo->HardwareID = iter->wsPath;
			pConnectionInfo->PhysicalDeviceName = L"";
			pConnectionInfo->DeviceDescription = iter->wsPath;
			pConnectionInfo->ServiceName = iter->wsCompatibleIDs;
			CDeviceInfo::Pointer pDeviceInfo = CDeviceInfo::Create(iter->wsPath, iter->wsFriendlyName, L"Mass Storage device", pConnectionInfo);
			collector_.Found(pDeviceInfo);
		}
	}
}
