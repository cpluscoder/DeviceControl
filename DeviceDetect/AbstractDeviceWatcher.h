#ifndef _A_DEVICE_WATCHER_H_
#define _A_DEVICE_WATCHER_H_

#include "Win_CriticalSection.h"
#include "IDeviceChanged.h"
#include "ICollector.h"
#include "NotifyWindow.h"

namespace DeviceDetectLibrary
{
	class IDeviceWatcherObserver;

	class CAbstractDeviceWatcher : public boost::noncopyable, public ICollector, public IDeviceChanged
	{
	public:
		virtual ~CAbstractDeviceWatcher(void);

		virtual void Start();
		virtual void Stop();

	public: // ICollector
		void Found(const CDeviceInfo::Pointer& pDeviceInfo);
		void Lost(const std::wstring& devId);

	protected:
		explicit CAbstractDeviceWatcher(IDeviceWatcherObserver* observer);

		virtual void StartEnumerators(void) = 0;
		virtual void StopEnumerators(void) = 0;

		CNotifyWindow& CreateNotifyWindow(void);
		void RemoveNotifyWindow(void);

	private:
		Devices actualDevices_;
		Devices tempDevices_;

		IDeviceWatcherObserver* observer_;

		CCriticalSection section_;
		std::auto_ptr<CNotifyWindow> window_;
	};
}
#endif _A_DEVICE_WATCHER_H_