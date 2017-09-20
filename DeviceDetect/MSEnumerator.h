#ifndef _C_MSENUMERATOR_H_
#define _C_MSENUMERATOR_H_

#include "MSChecker.h"
#include "Win_CriticalSection.h"

namespace DeviceDetectLibrary
{
	namespace Connection
	{
		class CMSEnumerator : public IEnumerator
		{
		public:
			typedef boost::shared_ptr<CMSEnumerator> Pointer;
			static CMSEnumerator::Pointer Create(ICollector& collector)
			{
				return CMSEnumerator::Pointer(new CMSEnumerator(collector));
			}
			
			virtual ~CMSEnumerator(void);

		public: // IEnumerator

			void Collect(const CDeviceInfo::Pointer& pDeviceInfo);

			void TryThis(const std::wstring& devicePath);
			void RemoveThis(const std::wstring& devicePath);

		protected:
			explicit CMSEnumerator(ICollector& collector);

		private:
			CCriticalSection sec_;
			ICollector& collector_;
			std::map<std::wstring, CMSChecker::Pointer> checkers_;
		};
		//typedef boost::shared_ptr<CMSEnumerator> MSEnumeratorPtr;
	}
}


#endif //_C_MSENUMERATOR_H_
