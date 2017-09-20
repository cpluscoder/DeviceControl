#include "StdAfx.h"
#include "AbstractUsbEnumerator.h"
#include "NotifyWindow.h"
#include "EnumDevices.h"


using namespace DeviceDetectLibrary;
using namespace DeviceDetectLibrary::Connection;

CAbstractUsbEnumerator::CAbstractUsbEnumerator(ICollector& collector) : collector_(collector)
{
}

CAbstractUsbEnumerator::~CAbstractUsbEnumerator(void)
{
}

void CAbstractUsbEnumerator::AddCollector(const GUID& guid, IEnumerator::Pointer pEnumerator, const CNotifyWindow& window)
{
	GTPair::Pointer pGTPair = GTPair::Create(guid, pEnumerator, window);
	table_.push_back(pGTPair);
}

void CAbstractUsbEnumerator::TryThis(const GUID& guid)
{
	class GuidEqual
	{
	public:
		explicit GuidEqual(const GUID& guid) : guid_(guid) {}
		virtual ~GuidEqual(void) {}

		bool operator()(const GTPair::Pointer& pItem) const
		{
			return memcmp(&(pItem->GetGuid()), &guid_, sizeof(guid_)) == 0;
		}

	private:
		const GUID& guid_;
	};

	try
	{
		std::vector<GTPair::Pointer>::const_iterator guidIter = std::find_if(table_.begin(), table_.end(), GuidEqual(guid));
		if(guidIter != table_.end())
		{
			ConnectionInfo_vt vctResult;
			GetDevicesByGuid(guid, vctResult);
			for(ConnectionInfo_vt::const_iterator iter = vctResult.begin(); iter != vctResult.end(); ++iter)
			{
				const CConnectionInfo::Pointer &pConnectionInfo = *iter;
				CDeviceInfo::Pointer pDeviceInfo = CDeviceInfo::Create(pConnectionInfo->DevicePath, pConnectionInfo->FriendlyName, L"Unknown device", pConnectionInfo);
				const GTPair::Pointer & pGTPair = *guidIter;
				if(NULL != pGTPair.get())
				{
					pGTPair->Collect(pDeviceInfo);
				}
			}
		}
	}
	catch(const std::exception& ex)
	{
		ex.what();
		// ERROR: in detection
	}
}


void CAbstractUsbEnumerator::Collect(const CDeviceInfo::Pointer& pDeviceInfo)
{
	class Caller
	{
	public:
		explicit Caller(ICollector& collector, CAbstractUsbEnumerator& enumerator)
			: collector_(collector), enumerator_(enumerator)
		{}
		
		virtual ~Caller(void) {}

		void operator() (const std::vector<GTPair::Pointer>::const_iterator::value_type& pair)
		{
			enumerator_.TryThis(pair->GetGuid());
		}

	private:
		ICollector& collector_;
		CAbstractUsbEnumerator& enumerator_;
	};

	for_each(table_.begin(), table_.end(), Caller(collector_, *this));
}


CAbstractUsbEnumerator::GTPair::GTPair(const GUID& guid, IEnumerator::Pointer pEnumerator, const CNotifyWindow& window) 
	: Guid_(guid), m_pEnumerator(pEnumerator), devNotify_(RegisterUSBNotify(guid, window.GetHWND()))
{
}

CAbstractUsbEnumerator::GTPair::~GTPair(void)
{
	try
	{
		UnRegisterUSBNotify(devNotify_);
	}
	catch (const std::exception& ex)
	{
		ex.what();
		// ERROR: GTPair::~GTPair fails (%s)", e.what());
	}
}

void CAbstractUsbEnumerator::GTPair::Collect(const CDeviceInfo::Pointer pDeviceInfo)
{
	if(NULL != m_pEnumerator.get())
	{
		m_pEnumerator->Collect(pDeviceInfo);
	}
}
