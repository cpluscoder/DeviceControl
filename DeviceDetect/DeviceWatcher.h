#ifndef _DEVICE_WATCHER_H_
#define _DEVICE_WATCHER_H_

#include "AbstractDeviceWatcher.h"

#include "UsbEnumerator.h"
#include "MSEnumerator.h"

namespace DeviceDetectLibrary
{
	class IDeviceWatcherObserver;

	class CDeviceWatcher : public CAbstractDeviceWatcher
	{
	public:
		typedef boost::shared_ptr<CDeviceWatcher> Pointer;
		static CDeviceWatcher::Pointer Create(IDeviceWatcherObserver* observer)
		{
			return CDeviceWatcher::Pointer(new CDeviceWatcher(observer));
		}

		virtual ~CDeviceWatcher(void);

	protected:
		explicit CDeviceWatcher(IDeviceWatcherObserver* observer);

		virtual void StartEnumerators(void);
		virtual void StopEnumerators(void);

	private: // IDeviceChanged
		void InterfaceArrival(const GUID& guid);
		void InterfaceRemoved(const std::wstring& devId);
		void VolumeArrival(const std::wstring& devId);
		void VolumeRemoved(const std::wstring& devId);

	private:
		Connection::CUsbEnumerator::Pointer m_pUsbEnumerator;
		Connection::CMSEnumerator::Pointer m_pMsEnumerator;
	};
}

#endif // _DEVICE_WATCHER_H_