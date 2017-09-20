#ifndef _WIN_WAITOBJECT_H_
#define _WIN_WAITOBJECT_H_

#include "Win_AutoHandle.h"

namespace DeviceDetectLibrary
{
	class CWaitObject : public CAutoHandle
	{
	public:
		CWaitObject(HANDLE handle);
		CWaitObject(HANDLE handle, CAutoHandle::Nothrow);
		virtual bool Wait(unsigned int milliseconds);

	private:
		CWaitObject(const CWaitObject&);
		CWaitObject& operator=(const CWaitObject&);
	};
}

#endif // _WIN_WAITOBJECT_H_