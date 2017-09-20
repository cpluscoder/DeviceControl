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
		strDevInterfaceVolume.clear();
		strDeviceDesc.clear();
		strEnumeratorName.clear();
		strCompatibleIDs.clear();
		strHardwareID.clear();
		strMFG.clear();
		strFriendlyName.clear();
		strPath.clear();
	}

	RemovableDeviceType emDeviceType;
	std::string strDevInterfaceVolume; // REG_SZ string that contains the interface volume name of a device
	std::string strDeviceDesc        ; // REG_SZ string containing the description of a device
	std::string strEnumeratorName    ; // REG_SZ string string containing the name of the device's enumerator
	std::string strCompatibleIDs     ; // REG_MULTI_SZ string string containing the list of compatible IDs for a device
	std::string strHardwareID        ; // REG_MULTI_SZ string string containing the list of hardware IDs for a device
	std::string strMFG               ; // REG_SZ string that contains the name of the device manufacturer
	std::string strFriendlyName      ; // REG_SZ string that contains the friendly name of a device
	std::string strPath              ; // REG_SZ string that contains the disk letter of a device
	
	//DiskGeometry diskGeometry      ; // structure describes the geometry of disk devices and media
	//std::vector<unsigned int> vHarddiskIndexes    ; // vector hard disk indexes
protected:
	explicit CRemovableDeviceInfo(void) { Clear(); }
};

class CUsbEnumerator
{
	DEFINE_BOOST_SHARED_PTR(CUsbEnumerator)
public:

	bool Start(void);

	void Stop(void);
	
	virtual ~CUsbEnumerator(void);

protected:
	explicit CUsbEnumerator(void);

	void Clear(void);

	bool GuidEqual(const GUID &guid1, const GUID &guid2) { return memcmp(&guid1, &guid1, sizeof(GUID)) == 0; }

	void EnumThread();

//////////////////////////////////////////////////////////////////////////
	void EnumUsb(void);

	bool GetDevicesByGuid(const GUID &guid, std::vector<CConnectionInfo::Pointer> &vctResult);

	bool PureChecker(HDEVINFO hDeviceInfo, SP_DEVICE_INTERFACE_DATA* devInterfaceData, CConnectionInfo::Pointer &pConnectionInfo);

	bool GetDeviceRegistryProperty(HDEVINFO &hDeviceInfo, PSP_DEVINFO_DATA pDevInfoData, DWORD dwProperty, std::string &strProperty);

//////////////////////////////////////////////////////////////////////////
	void EnumMS(void);

	bool SearchRemovalDisks(std::vector<CRemovableDeviceInfo::Pointer> &vctDevice);

	bool GetDevInterfaceVolume(PSP_DEVINFO_DATA pDevInfoData, std::string &strVolume);

	CRemovableDeviceInfo::RemovableDeviceType GetDeviceType(PSP_DEVINFO_DATA pDevInfoData);

protected:
	boost::thread m_threadEnum;
	boost::atomic_bool m_bThreadRun;

	std::vector<GUID> m_vctDeviceGuid;

	std::map<std::string, CDeviceInfo::Pointer> m_mapDevId2Info;
};
