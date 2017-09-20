#include "StdAfx.h"
#include "UsbEnumerator.h"
#include <cfgmgr32.h>
#include <WinIoCtl.h>

using namespace std;

CUsbEnumerator::CUsbEnumerator(void)
{
	m_bThreadRun = false;
	Clear();
}


CUsbEnumerator::~CUsbEnumerator(void)
{
	Stop();

	Clear();
}

void CUsbEnumerator::Clear(void)
{
	m_mapDevId2Info.clear();
}

bool CUsbEnumerator::Start(void)
{
	if(!m_bThreadRun)
	{
		m_threadEnum = boost::thread(boost::bind(&CUsbEnumerator::EnumThread, this));
		m_bThreadRun = true;
	}
	
	return true;
}

void CUsbEnumerator::Stop(void)
{
	if(m_bThreadRun)
	{
		m_bThreadRun = false;
		m_threadEnum.interrupt();
		m_threadEnum.join();
	}
}

void CUsbEnumerator::EnumThread()
{
	EnumUsb();

	EnumMS();
}

void CUsbEnumerator::EnumUsb(void)
{
	GUID guid = { 0xA5DCBF10L, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } };
	vector<CConnectionInfo::Pointer> vctResult;
	GetDevicesByGuid(guid, vctResult);
	vector<CConnectionInfo::Pointer>::const_iterator iter;
	for(iter = vctResult.begin(); iter != vctResult.end(); ++iter)
	{
		const CConnectionInfo::Pointer &pConnectionInfo = *iter;
		CDeviceInfo::Pointer pDeviceInfo = CDeviceInfo::Create(
			pConnectionInfo->DevicePath, pConnectionInfo->FriendlyName, "Unknown device", pConnectionInfo);
		m_mapDevId2Info[pDeviceInfo->GetId()] = pDeviceInfo;
	}
}

void CUsbEnumerator::EnumMS(void)
{
	try
	{
		vector<CRemovableDeviceInfo::Pointer> vctDisk;
		SearchRemovalDisks(vctDisk);
		for(auto iter = vctDisk.begin(); iter != vctDisk.end(); ++iter)
		{
			CRemovableDeviceInfo::Pointer pRemovableDeviceInfo = *iter;
			CConnectionInfo::Pointer pConnectionInfo = CConnectionInfo::Create();

			pConnectionInfo->m_emType = CConnectionInfo::TypeUnknown;
			pConnectionInfo->FriendlyName = pRemovableDeviceInfo->strFriendlyName;
			pConnectionInfo->DevicePath = pRemovableDeviceInfo->strDevInterfaceVolume;
			pConnectionInfo->HardwareID = pRemovableDeviceInfo->strHardwareID;
			pConnectionInfo->PhysicalDeviceName = pRemovableDeviceInfo->strEnumeratorName;
			pConnectionInfo->DeviceDescription = pRemovableDeviceInfo->strDeviceDesc;
			pConnectionInfo->ServiceName = pRemovableDeviceInfo->strCompatibleIDs;

			CDeviceInfo::Pointer pDeviceInfo = CDeviceInfo::Create(pRemovableDeviceInfo->strHardwareID, 
				pRemovableDeviceInfo->strFriendlyName, "Mass Storage device", pConnectionInfo);
			m_mapDevId2Info[pRemovableDeviceInfo->strHardwareID] = pDeviceInfo;
		}
	}
	catch(const std::exception& ex)
	{
		ex.what();
		// EXCEPTION: Cannot do SearchRemovalDisks(). Error = %s", ex.what());
	}
}

bool CUsbEnumerator::GetDevicesByGuid(const GUID &guid, vector<CConnectionInfo::Pointer> &vctResult)
{
	vctResult.clear();
	HDEVINFO hDeviceInfo = ::SetupDiGetClassDevs(&guid, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
	if(INVALID_HANDLE_VALUE == hDeviceInfo) 
	{
		return false;
	}

	DWORD dwCount = 0;
	SP_DEVICE_INTERFACE_DATA devInterfaceData;
	while(true)
	{
		ZeroMemory(&devInterfaceData, sizeof(SP_DEVICE_INTERFACE_DATA));
		devInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		if(!::SetupDiEnumDeviceInterfaces(hDeviceInfo, NULL, &guid, dwCount, &devInterfaceData))
		{
			break;
		}

		try
		{
			CConnectionInfo::Pointer pConnectionInfo = CConnectionInfo::Create();
			if(PureChecker(hDeviceInfo, &devInterfaceData, pConnectionInfo))
			{
				vctResult.push_back(pConnectionInfo);
			}
		}
		catch(const std::exception & ex)
		{
			TRACE("%s\n", ex.what());
		}
		
		++dwCount;
	}

	::SetupDiDestroyDeviceInfoList(hDeviceInfo);

	return true;
}

bool CUsbEnumerator::PureChecker(
	HDEVINFO hDeviceInfo, SP_DEVICE_INTERFACE_DATA* devInterfaceData, CConnectionInfo::Pointer &pConnectionInfo)
{
	pConnectionInfo->Clear();
	char szBuffer[0x400] = {0x00};
	PSP_DEVICE_INTERFACE_DETAIL_DATA pDevInfoDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)szBuffer;
	pDevInfoDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

	SP_DEVINFO_DATA devInfoData;
	ZeroMemory(&devInfoData, sizeof(SP_DEVINFO_DATA));
	devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

	DWORD dwReqSize = 0;
	if(!::SetupDiGetDeviceInterfaceDetail(hDeviceInfo, devInterfaceData, 
		pDevInfoDetail, sizeof(szBuffer)/sizeof(szBuffer[0]), &dwReqSize, &devInfoData))
	{
		throw std::runtime_error("Can't SetupDiGetInterfaceDeviceDetail");
		return false;
	}

	pConnectionInfo->m_emType = CConnectionInfo::TypeUsb;

	TRACE("\nFriendlyName:\t");
	if(!GetDeviceRegistryProperty(hDeviceInfo, &devInfoData, SPDRP_FRIENDLYNAME, pConnectionInfo->FriendlyName))
	{
		TRACE("\n");
	}
	TRACE("HardwareID:\t\t");
	if(!GetDeviceRegistryProperty(hDeviceInfo, &devInfoData, SPDRP_HARDWAREID, pConnectionInfo->HardwareID))
	{
		TRACE("\n");
	}
	TRACE("DeviceDesc:\t\t");
	if(!GetDeviceRegistryProperty(hDeviceInfo, &devInfoData, SPDRP_DEVICEDESC, pConnectionInfo->DeviceDescription))
	{
		TRACE("\n");
	}
	TRACE("Service:\t\t");
	if(!GetDeviceRegistryProperty(hDeviceInfo, &devInfoData, SPDRP_SERVICE, pConnectionInfo->ServiceName))
	{
		TRACE("\n");
	}
	pConnectionInfo->DevicePath = pDevInfoDetail->DevicePath;
	TRACE("DevicePath:\t\t%s\n", pConnectionInfo->DevicePath.c_str());
	
	return true;
}

bool CUsbEnumerator::GetDeviceRegistryProperty(
	HDEVINFO & hDeviceInfo, PSP_DEVINFO_DATA pDevInfoData, DWORD dwProperty, string &strProperty)
{
	strProperty.clear();

	if(NULL == pDevInfoData)
	{
		throw std::runtime_error("Wrong PSP_DEVINFO_DATA parameter");
	}

	DWORD dwSize = 0, dwDataType = 0;
	if(!::SetupDiGetDeviceRegistryProperty(hDeviceInfo, pDevInfoData, dwProperty, &dwDataType, NULL, 0, &dwSize))
	{
		DWORD error = ::GetLastError();
		if(ERROR_INVALID_DATA == error)
		{
			return false;
		}
		if(ERROR_INSUFFICIENT_BUFFER != error)
		{
			// Can't get Device Registry Property");
			throw std::runtime_error("Can't get Device Registry Property");
		}
	}

	vector<char> vctResult(dwSize, 0x00);
	if(vctResult.empty())
	{
		// Can't get Device Registry Property is empty");
		throw std::runtime_error("Can't get Device Registry Property is empty");
	}

	if(!::SetupDiGetDeviceRegistryProperty(hDeviceInfo, pDevInfoData, 
		dwProperty, &dwDataType, (PBYTE)&vctResult.front(), (DWORD)vctResult.size(), &dwSize))
	{
		// Can't get Device Registry Property");
		throw std::runtime_error("Can't get Device Registry Property");
	}

	vctResult.resize(dwSize);
	if(vctResult.empty())
	{
		// Device Registry Property is empty");
		throw std::runtime_error("Device Registry Property is empty");
	}

	strProperty = string(&vctResult.front());
	TRACE("%s\n", strProperty.c_str());

	return true;
}

bool CUsbEnumerator::SearchRemovalDisks(vector<CRemovableDeviceInfo::Pointer> &vctDevice)
{
	vctDevice.clear();

	/*GUID_DEVCLASS_DISKDRIVE*/
	CONST CLSID CLSID_DeviceInstance = { 0x4D36E967, 0xE325, 0x11CE, { 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 } };
	HDEVINFO hDeviceInfo = ::SetupDiGetClassDevs(&CLSID_DeviceInstance, NULL, NULL, DIGCF_PRESENT);
	if(INVALID_HANDLE_VALUE == hDeviceInfo)
	{
		return false;
	}

	try
	{
		DWORD dwCount = 0;
		DWORD dwSize = 0;
		DWORD dwDataType = 0;
		DWORD dwRemovalPolicy = 0;

		do 
		{
			dwSize = 0;
			dwDataType = 0;
			dwRemovalPolicy = 0;

			SP_DEVINFO_DATA devInfoData;
			ZeroMemory(&devInfoData, sizeof(devInfoData));
			devInfoData.cbSize = sizeof(devInfoData);
			if(!::SetupDiEnumDeviceInfo(hDeviceInfo, dwCount, &devInfoData))
			{
				break;
			}

			if(::SetupDiGetDeviceRegistryProperty(hDeviceInfo, &devInfoData, SPDRP_REMOVAL_POLICY, &dwDataType, (PBYTE)&dwRemovalPolicy, sizeof(dwRemovalPolicy), &dwSize))
			{
				if(CM_REMOVAL_POLICY_EXPECT_NO_REMOVAL != dwRemovalPolicy)
				{
					CRemovableDeviceInfo::Pointer pDevInfo = CRemovableDeviceInfo::Create();
					GetDeviceRegistryProperty(hDeviceInfo, &devInfoData, SPDRP_DEVICEDESC, pDevInfo->strDeviceDesc);
					GetDeviceRegistryProperty(hDeviceInfo, &devInfoData, SPDRP_ENUMERATOR_NAME, pDevInfo->strEnumeratorName);
					GetDeviceRegistryProperty(hDeviceInfo, &devInfoData, SPDRP_COMPATIBLEIDS, pDevInfo->strCompatibleIDs);
					GetDeviceRegistryProperty(hDeviceInfo, &devInfoData, SPDRP_HARDWAREID, pDevInfo->strHardwareID);
					GetDeviceRegistryProperty(hDeviceInfo, &devInfoData, SPDRP_MFG, pDevInfo->strMFG);
					GetDeviceRegistryProperty(hDeviceInfo, &devInfoData, SPDRP_FRIENDLYNAME, pDevInfo->strFriendlyName);
					GetDevInterfaceVolume(&devInfoData, pDevInfo->strDevInterfaceVolume);
					pDevInfo->emDeviceType = GetDeviceType(&devInfoData);

					vctDevice.push_back(pDevInfo);
				}
			}

			dwCount++;

		} while (true);

		::SetupDiDestroyDeviceInfoList(hDeviceInfo);

		return true;
	}
	catch(...)
	{
		::SetupDiDestroyDeviceInfoList(hDeviceInfo);
		throw;
	}

	return false;
}

bool CUsbEnumerator::GetDevInterfaceVolume(PSP_DEVINFO_DATA pDevInfoData, string &strVolume)
{
	ULONG ulSize = 0;
	DEVINST childDevInst;
	bool isDisk = false;

	if(NULL == pDevInfoData)
	{
		// Wrong PSP_DEVINFO_DATA parameter
		throw std::runtime_error("Wrong PSP_DEVINFO_DATA parameter");
	}

	DWORD dwRtn = CM_Get_Child_Ex(&childDevInst, pDevInfoData->DevInst, 0, NULL);
	isDisk = (CR_NO_SUCH_DEVNODE == dwRtn);
	if(isDisk)
	{
		childDevInst = pDevInfoData->DevInst;
	}
	else if(CR_SUCCESS != dwRtn)
	{
		// Can't get Child Device Instance
		throw std::runtime_error("Can't get Child Device Instance");
	}

	char bDeviceID[MAX_DEVICE_ID_LEN] = {0};
	if(CR_SUCCESS != CM_Get_Device_ID_Ex(childDevInst, bDeviceID, sizeof(bDeviceID)/sizeof(char), 0, NULL))
	{
		// Can't get Device ID");
		throw std::runtime_error("Can't get Device ID");
	}

	LPGUID InterfaceClassGuid = isDisk ? (LPGUID)&DiskClassGuid : (LPGUID)&VolumeClassGuid;

	if(CR_SUCCESS != CM_Get_Device_Interface_List_Size(&ulSize, InterfaceClassGuid, bDeviceID, 0))
	{
		// Can't get Device Instance List Size");
		throw std::runtime_error("Can't get Device Instance List Size");
	}

	vector<char> sResult(ulSize, 0x00);
	if(sResult.empty())
	{
		// Device Interface Volume is empty");
		throw std::runtime_error("Device Interface Volume is empty");
	}

	InterfaceClassGuid = isDisk ? (LPGUID)&GUID_DEVINTERFACE_DISK : (LPGUID)&GUID_DEVINTERFACE_VOLUME;

	if(CR_SUCCESS != CM_Get_Device_Interface_List(InterfaceClassGuid, bDeviceID, &sResult.front(), ulSize, 0))
	{
		// Can't get Device Instance List");
		throw std::runtime_error("Can't get Device Instance List");
	}

	sResult.resize(ulSize);
	if(sResult.empty())
	{
		// Device Interface Volume is empty
		throw std::runtime_error("Device Interface Volume is empty");
	}

	string strResult(&sResult.front());
	strVolume = strResult;

	return true;
}

CRemovableDeviceInfo::RemovableDeviceType CUsbEnumerator::GetDeviceType(PSP_DEVINFO_DATA pDevInfoData)
{
	DEVINST childDevInst;

	if(CR_NO_SUCH_DEVNODE == CM_Get_Child_Ex(&childDevInst, pDevInfoData->DevInst, 0, NULL))
	{
		return CRemovableDeviceInfo::rdtHardDisk;
	}
	else
	{
		return CRemovableDeviceInfo::rdtFlash;
	}
}
