#ifndef _DEVICE_INFO_H
#define _DEVICE_INFO_H

#include <map>
#include <boost/shared_ptr.hpp>
#include "ConnectionInfo.h"

namespace DeviceDetectLibrary
{
	class CDeviceInfo
	{
	public:
		typedef boost::shared_ptr<CDeviceInfo> Pointer;
		static CDeviceInfo::Pointer Create(void)
		{
			return CDeviceInfo::Pointer(new CDeviceInfo);
		}

		static CDeviceInfo::Pointer Create(const std::wstring& id, const std::wstring& name, 
			const std::wstring& deviceDisplayName, const CConnectionInfo::Pointer& pConnInfo)
		{
			return CDeviceInfo::Pointer(new CDeviceInfo(id, name, deviceDisplayName, pConnInfo));
		}
		
		virtual ~CDeviceInfo(void);

	public:
		inline const std::wstring& GetId(void) const
		{
			return id_;
		}

		inline const std::wstring& GetName(void) const
		{
			return name_;
		}

		inline void SetName(const std::wstring& name)
		{
			name_ = name;
		}

		inline std::wstring GetDeviceDisplayName(void) const
		{
			return deviceDisplayName_;
		}

		inline void SetDeviceDisplayName(const std::wstring& deviceDisplayName)
		{
			deviceDisplayName_ = deviceDisplayName;
		}

		inline const CConnectionInfo::Pointer& GetConnectionInfo(void) const
		{
			return m_pConnInfo;
		}

		//inline void SetConnectionInfo(const ConnectionInfo& connInfo)
		//{
		//	connInfo_ = connInfo;
		//}

	private:
		CDeviceInfo(void);
		CDeviceInfo(const std::wstring& id, const std::wstring& name, 
			const std::wstring& deviceDisplayName, const CConnectionInfo::Pointer& pConnInfo);

		CDeviceInfo(const CDeviceInfo&);
		CDeviceInfo& operator=(const CDeviceInfo&);

	private:
		std::wstring id_;
		std::wstring deviceDisplayName_;
		std::wstring name_;
		CConnectionInfo::Pointer m_pConnInfo;
	};

	typedef std::map<std::wstring, CDeviceInfo::Pointer> Devices;
}

#endif // _DEVICE_INFO_H