#ifndef _RESULT_ENUMERATOR_H_
#define _RESULT_ENUMERATOR_H_

#include "ICollector.h"

namespace DeviceDetectLibrary
{
	class CResultEnumerator : public IEnumerator
	{
	public:
		typedef boost::shared_ptr<CResultEnumerator> Pointer;
		static CResultEnumerator::Pointer Create(ICollector& collector, std::wstring deviceDisplayName, const std::wstring& friendlyName)
		{
			return CResultEnumerator::Pointer(new CResultEnumerator(collector, deviceDisplayName, friendlyName));
		}

		explicit CResultEnumerator(ICollector& collector, std::wstring deviceDisplayName, const std::wstring& friendlyName);
		virtual ~CResultEnumerator(void);

		void Collect(const CDeviceInfo::Pointer& pDeviceInfo);

	private:
		std::wstring deviceID_;
		std::wstring friendlyName_;
		ICollector& collector_;
	};
}

#endif // _RESULT_ENUMERATOR_H_