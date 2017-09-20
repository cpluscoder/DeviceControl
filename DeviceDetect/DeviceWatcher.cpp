#include "StdAfx.h"
#include "DeviceWatcher.h"

using namespace DeviceDetectLibrary;
using namespace DeviceDetectLibrary::Connection;

CDeviceWatcher::CDeviceWatcher(IDeviceWatcherObserver* observer) 
	: CAbstractDeviceWatcher(observer)
{
}

CDeviceWatcher::~CDeviceWatcher(void)
{
	Stop();
}

void CDeviceWatcher::InterfaceArrival(const GUID& guid)
{
	m_pUsbEnumerator->TryThis(guid);
}

void CDeviceWatcher::InterfaceRemoved(const std::wstring& devId)
{
	Lost(devId);
}

void CDeviceWatcher::VolumeArrival(const std::wstring& devId)
{
	if(NULL != m_pMsEnumerator.get())
	{
		m_pMsEnumerator->TryThis(devId);
	}
}

void CDeviceWatcher::VolumeRemoved(const std::wstring& devId)
{
	if(NULL != m_pMsEnumerator.get())
	{
		m_pMsEnumerator->RemoveThis(devId);
		Lost(devId);
	}
}

void CDeviceWatcher::StartEnumerators(void)
{
	CNotifyWindow& window = CreateNotifyWindow();

	// Create Enumerators
	m_pUsbEnumerator = CUsbEnumerator::Create(window, *this);
	CDeviceInfo::Pointer pDeviceInfo1 = CDeviceInfo::Create();
	m_pUsbEnumerator->Collect(pDeviceInfo1);

	m_pMsEnumerator = CMSEnumerator::Create(*this);
	CDeviceInfo::Pointer pDeviceInfo2 = CDeviceInfo::Create();
	m_pMsEnumerator->Collect(pDeviceInfo2);
}

void CDeviceWatcher::StopEnumerators(void)
{
	RemoveNotifyWindow();

	// Remove Enumerators
	m_pUsbEnumerator.reset();
	m_pMsEnumerator.reset();
}
