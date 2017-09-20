#include "StdAfx.h"
#include "ModemEnumeraror.h"

using namespace DeviceDetectLibrary;
using namespace DeviceDetectLibrary::Connection;

CModemEnumeraror::CModemEnumeraror(ICollector& collector) : collector_(collector)
{
	std::vector<std::wstring> motorolaPattern;
	// for example connected mobile phone Motorola
	motorolaPattern.push_back(L"Motorola USB");
	std::vector<CModemDeviceChecker::Pointer> motorolaParsers;
	CModemDeviceChecker::Pointer pModemDeviceChecker = CModemDeviceChecker::Create(collector, motorolaPattern);
	motorolaParsers.push_back(pModemDeviceChecker);
	modems_[L"Motorola Modem device"] = motorolaParsers;
}

CModemEnumeraror::~CModemEnumeraror(void)
{
}

void CModemEnumeraror::Collect(const CDeviceInfo::Pointer& pDeviceInfo)
{
	for(auto iter = modems_.begin(); iter != modems_.end(); ++iter)
	{
		for(auto parserIterator = iter->second.begin(); parserIterator != iter->second.end(); ++parserIterator)
		{
			CModemDeviceChecker::Pointer &pParser = *parserIterator;
			pParser->Collect(pDeviceInfo, iter->first);
		}
	}
}
