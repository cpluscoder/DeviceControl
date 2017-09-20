#include "StdAfx.h"
#include "DeviceInfo.h"
#include "Utilities.h"

using namespace DeviceDetectLibrary;

CDeviceInfo::CDeviceInfo(void)
	: deviceDisplayName_(L"Unknown device")
{
	id_.clear();
	deviceDisplayName_.clear();
	name_.clear();
	m_pConnInfo = CConnectionInfo::Create();
}

CDeviceInfo::CDeviceInfo(const std::wstring& id, const std::wstring& name, 
	const std::wstring& deviceDisplayName, const CConnectionInfo::Pointer& pConnInfo)
	: id_(Utilities::StringUpper(id))
	, name_(name)
	, deviceDisplayName_(deviceDisplayName)
	, m_pConnInfo(pConnInfo)
{
}

CDeviceInfo::~CDeviceInfo(void)
{
}

