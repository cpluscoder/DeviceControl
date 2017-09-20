#ifndef _C_MODEM_ENUMERATOR_H_
#define _C_MODEM_ENUMERATOR_H_

#include "ModemDeviceChecker.h"

namespace DeviceDetectLibrary
{
	namespace Connection
	{
		class CModemEnumeraror : public IEnumerator
		{
		public:
			typedef boost::shared_ptr<CModemEnumeraror> Pointer;
			static CModemEnumeraror::Pointer Create(ICollector& collector)
			{
				return CModemEnumeraror::Pointer(new CModemEnumeraror(collector));
			}

			explicit CModemEnumeraror(ICollector& collector);
			virtual ~CModemEnumeraror(void);

		public: // IEnumerator
			void Collect(const CDeviceInfo::Pointer& pDeviceInfo);

		private:
			std::map<std::wstring, std::vector<DeviceDetectLibrary::CModemDeviceChecker::Pointer> > modems_;
			ICollector& collector_;
		};
	}
}

#endif // _C_MODEM_ENUMERATOR_H_