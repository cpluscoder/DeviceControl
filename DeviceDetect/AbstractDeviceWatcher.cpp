#include "StdAfx.h"
#include "AbstractDeviceWatcher.h"
#include "AutoCriticalSection.h"
#include "IDeviceWatcherObserver.h"

using namespace DeviceDetectLibrary;

CAbstractDeviceWatcher::CAbstractDeviceWatcher(IDeviceWatcherObserver* observer) : observer_(observer)
{
}

CAbstractDeviceWatcher::~CAbstractDeviceWatcher(void)
{
}

void CAbstractDeviceWatcher::Start(void)
{
	StartEnumerators();
	tempDevices_.clear();
}

void CAbstractDeviceWatcher::Stop(void)
{
	StopEnumerators();
	actualDevices_.clear();
}

CNotifyWindow& CAbstractDeviceWatcher::CreateNotifyWindow(void)
{
	if(NULL == window_.get())
	{
		// Create NotifyWindow
		window_.reset(new CNotifyWindow(*this));
	}
	return *window_;
}

void CAbstractDeviceWatcher::RemoveNotifyWindow(void)
{
	// Remove NotifyWindow
	window_.reset(nullptr);
}

void CAbstractDeviceWatcher::Found(const CDeviceInfo::Pointer& pDeviceInfo)
{
	CAutoCriticalSection lock(section_);
	auto iter = tempDevices_.find(pDeviceInfo->GetId());
	if(iter != tempDevices_.end())
	{
		tempDevices_.erase(iter);
	}
	tempDevices_.insert(std::make_pair(pDeviceInfo->GetId(), pDeviceInfo));

	if(actualDevices_.count(pDeviceInfo->GetId()) == 0)
	{
		actualDevices_.insert(std::make_pair(pDeviceInfo->GetId(), pDeviceInfo));
		observer_->AppearedDevice(pDeviceInfo);
	}
}

void CAbstractDeviceWatcher::Lost(const std::wstring& deviceId)
{
	CAutoCriticalSection lock(section_);
	auto iter = actualDevices_.find(deviceId);
	if(iter != actualDevices_.end())
	{
		observer_->DisappearedDevice(iter->second);
		actualDevices_.erase(deviceId);
	}
}
