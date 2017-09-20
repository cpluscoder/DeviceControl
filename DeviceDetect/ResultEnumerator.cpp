#include "StdAfx.h"
#include "ResultEnumerator.h"

using namespace DeviceDetectLibrary;

CResultEnumerator::CResultEnumerator(ICollector& collector, std::wstring deviceDisplayName, const std::wstring& friendlyName)
	: deviceID_(deviceDisplayName), friendlyName_(friendlyName), collector_(collector)
{
}

CResultEnumerator::~CResultEnumerator(void)
{
}

void CResultEnumerator::Collect(const CDeviceInfo::Pointer& pDeviceInfo)
{
	CDeviceInfo::Pointer pDevice = CDeviceInfo::Create(pDeviceInfo->GetId(), pDeviceInfo->GetName(), pDeviceInfo->GetDeviceDisplayName(), pDeviceInfo->GetConnectionInfo());
	pDevice->SetName(friendlyName_);
	pDevice->SetDeviceDisplayName(deviceID_);
	collector_.Found(pDevice);
}
