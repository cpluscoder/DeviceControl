#ifndef _C_BLACKBERRYUSBENUMERATOR_H_
#define _C_BLACKBERRYUSBENUMERATOR_H_

#include "AbstractUsbEnumerator.h"

namespace DeviceDetectLibrary
{
	namespace Connection
	{
		class CBlackBerryUsbEnumerator : public CAbstractUsbEnumerator
		{
		public:
			explicit CBlackBerryUsbEnumerator(const CNotifyWindow& window, ICollector& collector);

			virtual ~CBlackBerryUsbEnumerator(void) {}
		};
	}
}

#endif _C_BLACKBERRYUSBENUMERATOR_H_