#ifndef _C_MODEMDEVICECHECKER_H_
#define _C_MODEMDEVICECHECKER_H_

#include "ICollector.h"

namespace DeviceDetectLibrary
{
	class CModemDeviceChecker
	{
	public:
		typedef boost::shared_ptr<CModemDeviceChecker> Pointer;
		static CModemDeviceChecker::Pointer Create(ICollector& collector, const std::vector<std::wstring>& patternNames)
		{
			return CModemDeviceChecker::Pointer(new CModemDeviceChecker(collector, patternNames));
		}
		
		virtual ~CModemDeviceChecker(void);

		void virtual Collect(const CDeviceInfo::Pointer& pDeviceInfo, const std::wstring& pluginID);

	protected:
		bool virtual Check(const CDeviceInfo::Pointer& pDeviceInfo);

	private:
		explicit CModemDeviceChecker(ICollector& collector, const std::vector<std::wstring>& patternNames);
		CModemDeviceChecker(const CModemDeviceChecker&);
		CModemDeviceChecker& operator=(const CModemDeviceChecker&);

	protected:
		ICollector& collector_;
		std::vector<std::wstring> _patternNames;
	};
	//typedef boost::shared_ptr<CModemDeviceChecker> ModemDeviceNameParserPtr;
	//typedef std::vector<ModemDeviceNameParserPtr> ModemDeviceNameParsers;
}


#endif // _C_MODEMDEVICECHECKER_H_
