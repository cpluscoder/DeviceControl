#include "stdafx.h"
#include "AutoCriticalSection.h"

using namespace DeviceDetectLibrary;

CAutoCriticalSection::CAutoCriticalSection(CCriticalSection& section)
	: section_(section)
{
	section_.Enter();
}

CAutoCriticalSection::~CAutoCriticalSection(void)
{
	section_.Leave();
}
