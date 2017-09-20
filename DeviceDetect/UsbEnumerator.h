#ifndef _C_USBENUMERATOR_H_
#define _C_USBENUMERATOR_H_

#include "AbstractUsbEnumerator.h"

namespace DeviceDetectLibrary
{
	class CNotifyWindow;

	namespace Connection
	{
		class CUsbEnumerator : public CAbstractUsbEnumerator
		{
		public:
			typedef boost::shared_ptr<CUsbEnumerator> Pointer;
			static CUsbEnumerator::Pointer Create(const CNotifyWindow& window, ICollector& collector)
			{
				return CUsbEnumerator::Pointer(new CUsbEnumerator(window, collector));
			}

			virtual ~CUsbEnumerator(void) {}

		private:
			explicit CUsbEnumerator(const CNotifyWindow& window, ICollector& collector);

			CUsbEnumerator(const CUsbEnumerator&);
			CUsbEnumerator& operator=(const CUsbEnumerator&);
		};
	}
}


#endif //  _C_TESTENUMERATOR_H_
