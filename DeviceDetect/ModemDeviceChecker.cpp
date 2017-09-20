#include "StdAfx.h"
#include "ModemDeviceChecker.h"

using namespace DeviceDetectLibrary;

CModemDeviceChecker::CModemDeviceChecker(ICollector& collector, const std::vector<std::wstring>& patternNames)
	: collector_(collector)
{
	_patternNames = patternNames;
}

CModemDeviceChecker::~CModemDeviceChecker(void)
{
}


bool CModemDeviceChecker::Check(const CDeviceInfo::Pointer& pDeviceInfo)
{
	int searchCount = 0;
	std::wstring name = pDeviceInfo->GetName();
	for(std::vector<std::wstring>::const_iterator iter = _patternNames.begin(); iter != _patternNames.end(); ++iter)
	{
		if(name.end() != std::search(name.begin(), name.end(), (*iter).begin(), (*iter).end()))
		{
			searchCount++;
		}
	}

	return searchCount == _patternNames.size();
}


void CModemDeviceChecker::Collect(const CDeviceInfo::Pointer& pDeviceInfo, const std::wstring& pluginID)
{
	if(Check(pDeviceInfo))
	{
		CDeviceInfo::Pointer pDeviceInfo = CDeviceInfo::Create(
			pDeviceInfo->GetId(), pDeviceInfo->GetName(), pDeviceInfo->GetDeviceDisplayName(), pDeviceInfo->GetConnectionInfo());
		pDeviceInfo->SetDeviceDisplayName(pluginID);
		collector_.Found(pDeviceInfo);
	}
}
