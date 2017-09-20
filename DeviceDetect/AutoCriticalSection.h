#ifndef _AUTO_CRITICAL_SECTION_H_
#define _AUTO_CRITICAL_SECTION_H_

#include "Win_CriticalSection.h"

namespace DeviceDetectLibrary
{
	class CAutoCriticalSection
	{
	public:
		explicit CAutoCriticalSection(CCriticalSection& section);
		virtual ~CAutoCriticalSection(void);

	private:
		CAutoCriticalSection(const CAutoCriticalSection&);
		CAutoCriticalSection& operator=(const CAutoCriticalSection&);

	private:
		CCriticalSection& section_;
	};
}

#endif // __AUTO_CRITICAL_SECTION_H_INCLUDED__

