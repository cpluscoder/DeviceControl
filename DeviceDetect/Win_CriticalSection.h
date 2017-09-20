#ifndef _WIN_CRITICAL_SECTION_H_
#define _WIN_CRITICAL_SECTION_H_

#include <Windows.h>

namespace DeviceDetectLibrary
{
	class CCriticalSection
	{
	public:
		explicit CCriticalSection(void);
		virtual ~CCriticalSection(void);

		void Enter(void);
		void Leave(void);

	private:
		CCriticalSection(const CCriticalSection&);
		CCriticalSection& operator=(const CCriticalSection&);

	private:
		CRITICAL_SECTION criticalSection_;
	};
}

#endif // _WIN_CRITICAL_SECTION_H_
