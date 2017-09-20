#include "stdafx.h"
#include "Win_AutoHandle.h"


CAutoHandle::CAutoHandle(HANDLE handle) 
	:handle_(handle)
{
	if(handle == INVALID_HANDLE_VALUE)
	{
		throw CAutoHandleException("[CAutoHandle]: Handle is invalid");
	}
}

CAutoHandle::CAutoHandle(HANDLE handle, Nothrow)
	:handle_(handle)
{
}

CAutoHandle::~CAutoHandle(void)
{
	Reset();
}

CAutoHandle::operator HANDLE(void) const
{
	return handle_;
}

HANDLE CAutoHandle::GetHandle(void) const
{
	return handle_;
}

HANDLE CAutoHandle::Release(void)
{
	HANDLE result = handle_;
	handle_ = INVALID_HANDLE_VALUE;
	return result;
}

void CAutoHandle::Reset(void)
{
	Reset(INVALID_HANDLE_VALUE);
}

void CAutoHandle::Reset(HANDLE newHandle/* = INVALID_HANDLE_VALUE*/)
{
	if(handle_ != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(handle_);
	}
	handle_ = newHandle;
}
