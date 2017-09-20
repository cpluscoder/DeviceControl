#include "StdAfx.h"
#include "Win_ManualResetEvent.h"

using namespace DeviceDetectLibrary;

CManualResetEvent::CManualResetEvent(void)
	: CWaitObject(::CreateEvent(NULL, TRUE, FALSE, NULL))
{
}

void CManualResetEvent::Set(void)
{
	::SetEvent(*this);
}

void CManualResetEvent::Reset(void)
{
	::ResetEvent(*this);
}