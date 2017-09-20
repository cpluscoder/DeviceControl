#ifndef _C_MS_CHECKER_H_
#define _C_MS_CHECKER_H_

#include "ICollector.h"
#include "Win_Thread.h"
#include "Win_ManualResetEvent.h"


namespace DeviceDetectLibrary
{
	namespace Connection
	{
		class CMSChecker
		{
		public:
			typedef boost::shared_ptr<CMSChecker> Pointer;
			static CMSChecker::Pointer Create(ICollector& collector, const std::wstring& devicePath)
			{
				return CMSChecker::Pointer(new CMSChecker(collector, devicePath));
			}
			
			virtual ~CMSChecker(void);

			void Start(void);
			void Stop(void);

		private:
			explicit CMSChecker(ICollector& collector, const std::wstring& devicePath);
			CMSChecker(const CMSChecker&);
			CMSChecker& operator=(const CMSChecker&);

			static void ThreadFunction(void* state);
			void WorkThread(void);

		private:
			std::wstring devicePath_;
			ICollector& collector_;

			CThread thread_;
			CManualResetEvent stopEvent_;
		};

		//typedef boost::shared_ptr<CMSChecker> MSCheckerPtr;
	}
}


#endif // _C_MS_CHECKER_H_
