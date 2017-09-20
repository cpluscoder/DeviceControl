#ifndef _IENUMERATOR_H_
#define _IENUMERATOR_H_

#include <boost/noncopyable.hpp>
#include "DeviceInfo.h"

namespace DeviceDetectLibrary
{
	class CDeviceInfo;

	class IEnumerator : public boost::noncopyable
	{
	public:
		typedef boost::shared_ptr<IEnumerator> Pointer;
		
		virtual ~IEnumerator(void) {}

		virtual void Collect(const CDeviceInfo::Pointer& pDeviceInfo) = 0;

	protected:
		explicit IEnumerator(void) {}
	};
}

#endif // _IENUMERATOR_H_