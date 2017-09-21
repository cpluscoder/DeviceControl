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
	m_hMainWnd = nullptr;

	m_strHardwareId.clear();

	m_strFriendlyName.clear();

	m_strDeviceName.clear();
}

bool CUsbEnumerator::Start(HWND hMainWnd, const string &strHardwareId, const string &strFriendlyName)
{
	if(!m_bThreadRun)
	{
		m_hMainWnd = hMainWnd;
		m_strHardwareId = strHardwareId;
		m_strFriendlyName = strFriendlyName;
		m_strDeviceName.clear();

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
	TRACE("---Start--- UsbEnumerator(%s)\n", COleDateTime::GetCurrentTime().Format("%H:%M:%S"));

	SearchRemovalDisks();

	if(m_hMainWnd != nullptr)
	{
		if(::IsWindow(m_hMainWnd))
		{
			::PostMessage(m_hMainWnd, WM_ENUM_USB_DEVICE_COMPLETE, NULL, NULL);
		}
	}

	TRACE("---Stop --- UsbEnumerator(%s)\n", COleDateTime::GetCurrentTime().Format("%H:%M:%S"));

	m_bThreadRun = false;
}

bool CUsbEnumerator::SearchRemovalDisks(void)
{
	/*GUID_DEVCLASS_DISKDRIVE*/
	CONST CLSID CLSID_DeviceInstance = { 0x4D36E967, 0xE325, 0x11CE, { 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 } };
	HDEVINFO hDeviceInfo = ::SetupDiGetClassDevs(&CLSID_DeviceInstance, NULL, NULL, DIGCF_PRESENT);
	if(INVALID_HANDLE_VALUE == hDeviceInfo)
	{
		return false;
	}

	DWORD dwCount = 0;
	DWORD dwSize = 0;
	DWORD dwDataType = 0;
	DWORD dwRemovalPolicy = 0;
	string strHardwareId;
	string strFriendlyName;

	try
	{
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
					TRACE("SPDRP_HARDWAREID:");
					GetDeviceRegistryProperty(hDeviceInfo, &devInfoData, SPDRP_HARDWAREID, strHardwareId);

					TRACE("SPDRP_FRIENDLYNAME:");
					GetDeviceRegistryProperty(hDeviceInfo, &devInfoData, SPDRP_FRIENDLYNAME, strFriendlyName);
					
					if(boost::algorithm::iequals(m_strHardwareId, strHardwareId) && boost::algorithm::iequals(m_strFriendlyName, strFriendlyName))
					{
						if(GetDevInterfaceVolume(&devInfoData, m_strDeviceName))
						{
							TRACE("DevInterfaceVolume:%s\n", m_strDeviceName.c_str());
						}
						break;
					}
				}
			}

			dwCount++;

		} while (true);

		::SetupDiDestroyDeviceInfoList(hDeviceInfo);
		return true;
	}
	catch(const std::exception& ex)
	{
		::SetupDiDestroyDeviceInfoList(hDeviceInfo);
		throw;
	}

	return false;
}

bool CUsbEnumerator::GetDevInterfaceVolume(PSP_DEVINFO_DATA pDevInfoData, string &strVolume)
{
	ULONG ulSize = 0;
	bool isDisk = false;
	DEVINST childDevInst;

	if(nullptr == pDevInfoData)
	{
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
		throw std::runtime_error("Can't get Child Device Instance");
	}

	char bDeviceID[MAX_DEVICE_ID_LEN] = {0};
	if(CR_SUCCESS != CM_Get_Device_ID_Ex(childDevInst, bDeviceID, sizeof(bDeviceID)/sizeof(char), 0, NULL))
	{
		throw std::runtime_error("Can't get Device ID");
	}

	LPGUID InterfaceClassGuid = isDisk ? (LPGUID)&DiskClassGuid : (LPGUID)&VolumeClassGuid;
	if(CR_SUCCESS != CM_Get_Device_Interface_List_Size(&ulSize, InterfaceClassGuid, bDeviceID, 0))
	{
		throw std::runtime_error("Can't get Device Instance List Size");
	}

	vector<char> sResult(ulSize, 0x00);
	if(sResult.empty())
	{
		throw std::runtime_error("Device Interface Volume is empty");
	}

	InterfaceClassGuid = isDisk ? (LPGUID)&GUID_DEVINTERFACE_DISK : (LPGUID)&GUID_DEVINTERFACE_VOLUME;
	if(CR_SUCCESS != CM_Get_Device_Interface_List(InterfaceClassGuid, bDeviceID, &sResult.front(), ulSize, 0))
	{
		throw std::runtime_error("Can't get Device Instance List");
	}

	sResult.resize(ulSize);
	if(sResult.empty())
	{
		throw std::runtime_error("Device Interface Volume is empty");
	}

	string strResult(&sResult.front());
	strVolume = strResult;

	return !strVolume.empty();
}

bool CUsbEnumerator::GetDeviceRegistryProperty(HDEVINFO & hDeviceInfo, PSP_DEVINFO_DATA pDevInfoData, DWORD dwProperty, string &strProperty)
{
	strProperty.clear();

	if(nullptr == pDevInfoData)
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
			throw std::runtime_error("Can't get Device Registry Property");
		}
	}

	vector<char> vctResult(dwSize, 0x00);
	if(vctResult.empty())
	{
		throw std::runtime_error("Can't get Device Registry Property is empty");
	}

	if(!::SetupDiGetDeviceRegistryProperty(hDeviceInfo, pDevInfoData, dwProperty, &dwDataType, (PBYTE)&vctResult.front(), (DWORD)vctResult.size(), &dwSize))
	{
		throw std::runtime_error("Can't get Device Registry Property");
	}

	vctResult.resize(dwSize);
	if(vctResult.empty())
	{
		throw std::runtime_error("Device Registry Property is empty");
	}

	strProperty = string(&vctResult.front());
	TRACE("%s\n", strProperty.c_str());

	return true;
}
