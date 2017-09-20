#include "StdAfx.h"
#include "Win_WaitObject.h"

using namespace DeviceDetectLibrary;

CWaitObject::CWaitObject(HANDLE handle) : CAutoHandle(handle)
{
}

CWaitObject::CWaitObject(HANDLE handle, CAutoHandle::Nothrow nothrow) : CAutoHandle(handle, nothrow)
{
}

bool CWaitObject::Wait(unsigned int milliseconds)
{
	if((HANDLE)(*this) == INVALID_HANDLE_VALUE)
	{
		return true;
	}

	return ::WaitForSingleObject(*this, milliseconds) == WAIT_OBJECT_0;
}

