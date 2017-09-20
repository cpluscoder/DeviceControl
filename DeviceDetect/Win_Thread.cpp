#include "StdAfx.h"
#include <atlbase.h>
#include "Win_Thread.h"

using namespace DeviceDetectLibrary;

CThread::CThread(ThreadFunction routine)
	: CWaitObject(INVALID_HANDLE_VALUE, CAutoHandle::Nothrow())
	, routine_(routine)
	, currentState_(NULL)
{
}

CThread::~CThread(void)
{
	Wait(INFINITE);
}

void CThread::Start(void * state)
{
	if((HANDLE)(*this) != INVALID_HANDLE_VALUE)
	{
		throw CAutoHandleException("Thread is already started");
	}

	currentState_ = state;

	Reset((HANDLE)_beginthreadex(NULL, 0, CThread::Routine, this, 0, NULL));

	if(((HANDLE)(*this) == INVALID_HANDLE_VALUE) || ((HANDLE)(*this) == NULL))
	{
		Reset();
		throw CAutoHandleException("Cannot create thread");
	}
}

unsigned int __stdcall CThread::Routine(void * state)
{
	CThread* pThis = static_cast<CThread*>(state);
	pThis->routine_(pThis->currentState_);
	return 0;
}

bool CThread::Wait(unsigned int milliseconds)
{
	bool result = CWaitObject::Wait(milliseconds);
	if(result)
	{
		Reset();
	}
	return result;
}
