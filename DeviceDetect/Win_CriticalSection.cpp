#include "stdafx.h"
#include "Win_CriticalSection.h"

using namespace DeviceDetectLibrary;

CCriticalSection::CCriticalSection(void)
{
	::InitializeCriticalSection(&criticalSection_);
}

CCriticalSection::~CCriticalSection(void)
{
	::DeleteCriticalSection(&criticalSection_);
}

void CCriticalSection::Enter(void)
{
	::EnterCriticalSection(&criticalSection_);
}

void CCriticalSection::Leave(void)
{
	::LeaveCriticalSection(&criticalSection_);
}


