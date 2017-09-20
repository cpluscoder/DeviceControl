#ifndef _I_DEVICE_WATCHER_OBSERVER_H_
#define _I_DEVICE_WATCHER_OBSERVER_H_

#include "DeviceInfo.h"

namespace DeviceDetectLibrary
{
	class IDeviceWatcherObserver
	{
	public:
		explicit IDeviceWatcherObserver(void) {}
		virtual ~IDeviceWatcherObserver(void) {}

		virtual void AppearedDevice(const CDeviceInfo::Pointer& pDeviceInfo) = 0;
		virtual void DisappearedDevice(const CDeviceInfo::Pointer& pDeviceInfo) = 0;

	private:
		IDeviceWatcherObserver(const IDeviceWatcherObserver&);
		IDeviceWatcherObserver& operator=(const IDeviceWatcherObserver&);
	};
}


#endif // _I_DEVICE_WATCHER_OBSERVER_H_