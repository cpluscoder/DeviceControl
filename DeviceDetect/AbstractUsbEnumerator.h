#ifndef _C_AUSBENUMERATOR_H_
#define _C_AUSBENUMERATOR_H_

#include "ICollector.h"

namespace DeviceDetectLibrary
{
	class CNotifyWindow;

	namespace Connection
	{
		class CAbstractUsbEnumerator : public IEnumerator
		{
		public:
			typedef boost::shared_ptr<CAbstractUsbEnumerator> Pointer;
			static CAbstractUsbEnumerator::Pointer Create(ICollector& collector)
			{
				return CAbstractUsbEnumerator::Pointer(new CAbstractUsbEnumerator(collector));
			}
			
			virtual ~CAbstractUsbEnumerator(void);

			virtual void TryThis(const GUID& guid);

			virtual void AddCollector(const GUID& guid, IEnumerator::Pointer pEnumerator, const CNotifyWindow& window);

			// IEnumerator
			virtual void Collect(const CDeviceInfo::Pointer& pDeviceInfo);

		protected:
			explicit CAbstractUsbEnumerator(ICollector& collector);

		private:
			class GTPair : boost::noncopyable
			{
			public:
				typedef boost::shared_ptr<GTPair> Pointer;
				static GTPair::Pointer Create(const GUID& guid, IEnumerator::Pointer pEnumerator, const CNotifyWindow& window)
				{
					return GTPair::Pointer(new GTPair(guid, pEnumerator, window));
				}
				
				virtual ~GTPair(void);

				void Collect(const CDeviceInfo::Pointer pDeviceInfo);

				const GUID &GetGuid(void) const { return Guid_; }

			protected:
				explicit GTPair(const GUID& guid, IEnumerator::Pointer pEnumerator, const CNotifyWindow& window);

			private:
				GUID Guid_;
				IEnumerator::Pointer m_pEnumerator;
				HDEVNOTIFY devNotify_;
			};

			std::vector<GTPair::Pointer> table_;
			ICollector& collector_;
		};
		//typedef boost::shared_ptr<CAbstractUsbEnumerator> UsbEnumeratorPtr;
	}
}

#endif _C_AUSBENUMERATOR_H_