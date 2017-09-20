#include "StdAfx.h"
#include <Windows.h>
#include "TestDeviceWatcherObserver.h"
#include "DeviceDetectLog.h"


using namespace DeviceDetectLibrary;

CTestDeviceWatcherObserver::CTestDeviceWatcherObserver(void)
{
}

CTestDeviceWatcherObserver::~CTestDeviceWatcherObserver(void)
{
}

bool WideStrToMultiStr(const std::wstring &wstrSrc, std::string &strDst)
{
	int nBytes = ::WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, wstrSrc.c_str(), -1, NULL, 0, NULL, NULL);
	if(nBytes <= 0)
	{
		return false;
	}

	char* pszDst = new char[nBytes];
	if(NULL == pszDst)
	{
		return false;
	}

	if(0 == ::WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, wstrSrc.c_str(), -1, pszDst, nBytes, NULL, NULL))
	{
		delete [] pszDst;
		pszDst = NULL;
		return false;
	}

	pszDst[nBytes -1] = '\0';
	strDst = pszDst;
	delete [] pszDst;
	pszDst = NULL;
	return true;
}

void CTestDeviceWatcherObserver::AppearedDevice(const CDeviceInfo::Pointer& pDeviceInfo)
{
	std::string strText;
	if(WideStrToMultiStr(pDeviceInfo->GetName(), strText))
	{
		LogMessage("\n发现设备: %s", strText.c_str());
	}

	CConnectionInfo::Pointer pConnectionInfo = pDeviceInfo->GetConnectionInfo();
	if(WideStrToMultiStr(pConnectionInfo->DeviceDescription, strText))
	{
		LogMessage("    Device description: %s", strText.c_str());
	}
	if(WideStrToMultiStr(pDeviceInfo->GetConnectionInfo()->DevicePath, strText))
	{
		LogMessage("    Device path: %s", strText.c_str());
	}
	if(WideStrToMultiStr(pDeviceInfo->GetConnectionInfo()->FriendlyName, strText))
	{
		LogMessage("    Friendly name: %s", strText.c_str());
	}
	if(WideStrToMultiStr(pDeviceInfo->GetConnectionInfo()->HardwareID, strText))
	{
		LogMessage("    Hardware ID: %s", strText.c_str());
	}
	if(WideStrToMultiStr(pDeviceInfo->GetConnectionInfo()->PhysicalDeviceName, strText))
	{
		LogMessage("    Physical device name: %s", strText.c_str());
	}
	if(WideStrToMultiStr(pDeviceInfo->GetConnectionInfo()->ServiceName, strText))
	{
		LogMessage("    Service name: %s", strText.c_str());
	}
	//LogMessage("    Device description: %S", connectionInfo.DeviceDescription.c_str());
	//LogMessage("    Device path: %S", pDeviceInfo->GetConnectionInfo().DevicePath.c_str());
	//LogMessage("    Friendly name: %S", pDeviceInfo->GetConnectionInfo().FriendlyName.c_str());
	//LogMessage("    Hardware ID: %S", pDeviceInfo->GetConnectionInfo().HardwareID.c_str());
	//LogMessage("    Physical device name: %S", pDeviceInfo->GetConnectionInfo().PhysicalDeviceName.c_str());
	auto iter = devices_.find(pDeviceInfo->GetId());
	if(iter != devices_.end())
	{
		devices_.erase(iter);
	}
	devices_.insert(std::make_pair(pDeviceInfo->GetId(), pDeviceInfo));
	//devices_[pDeviceInfo->GetId()] = deviceInfo;
}

void CTestDeviceWatcherObserver::DisappearedDevice(const CDeviceInfo::Pointer& pDeviceInfo)
{
	LogMessage("\n移除设备");

	CConnectionInfo::Pointer pConnectionInfo = pDeviceInfo->GetConnectionInfo();

	std::string strText;
	if(WideStrToMultiStr(pConnectionInfo->DeviceDescription, strText))
	{
		LogMessage("    Device description: %s", strText.c_str());
	}
	if(WideStrToMultiStr(pDeviceInfo->GetConnectionInfo()->DevicePath, strText))
	{
		LogMessage("    Device path: %s", strText.c_str());
	}
	if(WideStrToMultiStr(pDeviceInfo->GetConnectionInfo()->FriendlyName, strText))
	{
		LogMessage("    Friendly name: %s", strText.c_str());
	}
	if(WideStrToMultiStr(pDeviceInfo->GetConnectionInfo()->HardwareID, strText))
	{
		LogMessage("    Hardware ID: %s", strText.c_str());
	}
	if(WideStrToMultiStr(pDeviceInfo->GetConnectionInfo()->PhysicalDeviceName, strText))
	{
		LogMessage("    Physical device name: %s", strText.c_str());
	}
	if(WideStrToMultiStr(pDeviceInfo->GetConnectionInfo()->ServiceName, strText))
	{
		LogMessage("    Service name: %s", strText.c_str());
	}

	devices_.erase(pDeviceInfo->GetId());
}
