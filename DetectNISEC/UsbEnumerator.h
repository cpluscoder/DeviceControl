#pragma once

#include "CommonDef.h"
#include <boost/thread.hpp>
#include <boost/atomic.hpp>
#include <setupapi.h>


class CRemovableDeviceInfo
{
	DEFINE_BOOST_SHARED_PTR(CRemovableDeviceInfo);

public:
	typedef enum _RemovableDeviceType
	{
		rdtUnknown  = 0,
		rdtFlash,
		rdtHardDisk,
	}RemovableDeviceType;

	virtual ~CRemovableDeviceInfo(void) {}

	void Clear(void)
	{
		emDeviceType = rdtUnknown;
		strHardwareID.clear();
		strFriendlyName.clear();
		strDevInterfaceVolume.clear();
	}

	RemovableDeviceType emDeviceType;
	std::string strHardwareID;			// REG_MULTI_SZ string string containing the list of hardware IDs for a device
	std::string strFriendlyName;		// REG_SZ string that contains the friendly name of a device
	std::string strDevInterfaceVolume;	// REG_SZ string that contains the interface volume name of a device

protected:
	explicit CRemovableDeviceInfo(void) { Clear(); }
};

class CUsbEnumerator
{
	DEFINE_BOOST_SHARED_PTR(CUsbEnumerator)
public:

	bool Start(HWND hMainWnd, const std::string &strHardwareId, const std::string &strFriendlyName);

	void Stop(void);

	inline const std::string GetDeviceName(void) const { return m_strDeviceName; }
	
	virtual ~CUsbEnumerator(void);

protected:
	explicit CUsbEnumerator(void);

	void Clear(void);

	void EnumThread();

	bool SearchRemovalDisks(void);

	bool GetDeviceRegistryProperty(HDEVINFO &hDeviceInfo, PSP_DEVINFO_DATA pDevInfoData, DWORD dwProperty, std::string &strProperty);

	bool GetDevInterfaceVolume(PSP_DEVINFO_DATA pDevInfoData, std::string &strVolume);

protected:
	HWND m_hMainWnd;

	std::string m_strHardwareId;
	std::string m_strFriendlyName;

	std::string m_strDeviceName;

	boost::thread m_threadEnum;
	boost::atomic_bool m_bThreadRun;
};
