#include "StdAfx.h"
#include "BlackBerryUsbEnumerator.h"
#include "ResultEnumerator.h"

using namespace DeviceDetectLibrary;
using namespace DeviceDetectLibrary::Connection;

CBlackBerryUsbEnumerator::CBlackBerryUsbEnumerator(
	const DeviceDetectLibrary::CNotifyWindow &window, 
	DeviceDetectLibrary::ICollector &collector)
	: CAbstractUsbEnumerator(collector)
{
	GUID guidBB = { 0x80375827, 0x83b8, 0x4a51, { 0xb3, 0x9b, 0x90, 0x5f, 0xed, 0xd4, 0xf1, 0x18 } };
	CResultEnumerator::Pointer pResultEnumerator = CResultEnumerator::Create(collector, L"BlackBerry", L"BlackBerry USB Device");
	AddCollector(guidBB, pResultEnumerator, window);
}
