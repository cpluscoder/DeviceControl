#ifndef _IDEVICECHANGED_H_
#define _IDEVICECHANGED_H_

#include <string>

namespace DeviceDetectLibrary
{
	class IDeviceChanged
	{
	public:
		explicit IDeviceChanged(void) {}
		virtual ~IDeviceChanged(void) {}

		virtual void InterfaceArrival(const GUID& guid) = 0;
		virtual void InterfaceRemoved(const std::wstring& devId) = 0;
		virtual void VolumeArrival(const std::wstring& devId) = 0;
		virtual void VolumeRemoved(const std::wstring& devId) = 0;

	private:
		IDeviceChanged(const IDeviceChanged&);
		IDeviceChanged& operator=(const IDeviceChanged&);
	};

}

#endif // _IDEVICECHANGED_H_
