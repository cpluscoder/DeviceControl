#ifndef _WIN_THREAD_H_
#define _WIN_THREAD_H_

#include "Win_WaitObject.h"

namespace DeviceDetectLibrary
{
	class CThread : public CWaitObject
	{
	public:
		typedef void (*ThreadFunction)(void * state);

		explicit CThread(ThreadFunction routine);
		virtual ~CThread(void);

	public:
		void Start(void * state);
		bool Wait(unsigned int milliseconds);

	private:
		static unsigned int __stdcall Routine(void * state);

		CThread(const CThread&);
		CThread& operator=(const CThread&);

	private:
		ThreadFunction routine_;
		void * currentState_;
	};
}

#endif // _WIN_THREAD_H_