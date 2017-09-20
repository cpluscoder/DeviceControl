#ifndef _CONNECTION_INFO_H_
#define _CONNECTION_INFO_H_

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace DeviceDetectLibrary
{
	enum DeviceType
	{
		TypeUnknown = 0,
		TypeSerial,
		TypeUsb,
		TypePureUsb
	};

	class CConnectionInfo
	{
	public:
		typedef boost::shared_ptr<CConnectionInfo> Pointer;
		static CConnectionInfo::Pointer Create(void)
		{
			return CConnectionInfo::Pointer(new CConnectionInfo);
		}

		virtual ~CConnectionInfo(void) { Clear(); }

		void Clear(void)
		{
			Type = TypeUnknown;
			FriendlyName.clear();
			DevicePath.clear();
			HardwareID.clear();
			PhysicalDeviceName.clear();
			DeviceDescription.clear();
			ServiceName.clear();
		}

		DeviceType Type;
		std::wstring FriendlyName;
		std::wstring DevicePath;
		std::wstring HardwareID;
		std::wstring PhysicalDeviceName;
		std::wstring DeviceDescription;
		std::wstring ServiceName;

	protected:
		explicit CConnectionInfo(void) { Clear(); }

		CConnectionInfo(const CConnectionInfo&);
		CConnectionInfo& operator=(const CConnectionInfo&);
	};

	bool operator == (const CConnectionInfo & left, const CConnectionInfo & right);
	bool operator < (const CConnectionInfo & left, const CConnectionInfo & right);

	typedef std::vector<CConnectionInfo::Pointer> ConnectionInfo_vt;
}

#endif // _CONNECTION_INFO_H_