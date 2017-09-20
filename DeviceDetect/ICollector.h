#ifndef _ICOLLECTOR_H_
#define _ICOLLECTOR_H_

#include "IEnumerator.h"

namespace DeviceDetectLibrary
{
	class ICollector
	{
	public:
		explicit ICollector(void) {}
		virtual ~ICollector(void) {}

		virtual void Found(const CDeviceInfo::Pointer& pDeviceInfo) = 0;
		virtual void Lost(const std::wstring& diveceId) = 0;

	private:
		ICollector(const ICollector &);
		ICollector &operator= (const ICollector &);
	};
}

#endif // _ICOLLECTOR_H_