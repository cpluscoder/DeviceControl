#ifndef _WIN_MANUAL_RESET_EVENT_
#define _WIN_MANUAL_RESET_EVENT_

#include "Win_WaitObject.h"

namespace DeviceDetectLibrary
{
	class CManualResetEvent : public CWaitObject
	{
	public:
		explicit CManualResetEvent(void);
		virtual ~CManualResetEvent(void) {}

		void Set(void);
		void Reset(void);
	};
}

#endif //_WIN_MANUAL_RESET_EVENT_
