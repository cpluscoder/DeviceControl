#ifndef _TEST_DEVICE_WATCHER_OBSERVER_H_
#define _TEST_DEVICE_WATCHER_OBSERVER_H_

#include "IDeviceWatcherObserver.h"

class CTestDeviceWatcherObserver : public DeviceDetectLibrary::IDeviceWatcherObserver
{
public:
	explicit CTestDeviceWatcherObserver(void);
	virtual ~CTestDeviceWatcherObserver(void);

public: // IDeviceWatcherObserver interface

	void AppearedDevice(const DeviceDetectLibrary::CDeviceInfo::Pointer& pDeviceInfo);
	void DisappearedDevice(const DeviceDetectLibrary::CDeviceInfo::Pointer& pDeviceInfo);

private:
	CTestDeviceWatcherObserver(const CTestDeviceWatcherObserver&);
	CTestDeviceWatcherObserver& operator=(const CTestDeviceWatcherObserver&);

private:

	DeviceDetectLibrary::Devices devices_;

};

#endif // _TEST_DEVICE_WATCHER_OBSERVER_H_