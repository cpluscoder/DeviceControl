#pragma once


#include <string>
#include <map>
#include <vector>
#include <tuple>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>


/*!< ����BOOST����ָ�� */
#define DECLARE_BOOST_SHARED_PTR(ClassName)	\
public: \
	typedef boost::shared_ptr<ClassName> Pointer;	\
protected:	\
	ClassName(const ClassName &);	\
	ClassName &operator= (const ClassName &);


/*!< ����BOOST����ָ�� */
#define DEFINE_BOOST_SHARED_PTR(ClassName)	\
public:	\
	typedef boost::shared_ptr<ClassName> Pointer;	\
	static ClassName::Pointer Create()	\
	{	\
		return ClassName::Pointer(new ClassName);	\
	}	\
protected:	\
	ClassName(const ClassName &);	\
	ClassName &operator= (const ClassName &);

namespace Evt {
typedef enum _EventType
{
	StartSearchNewDbFile = 0,		/*!< ����֪Ŀ¼,�������ݿ��ļ� */
	DbFileChanged,					/*!< �������ݿ��ļ����޸� */
	QueryDbComplete,				/*!< ��ѯ���ݿ���� */
	SendData2WebServerComplete,		/*!< ���ƶ˷������Ʊ������� */
	StartFileCollect,				/*!< ��ʼ�����ļ��ɼ�(ͬ��ִ��) */
	StartFolderCollect,				/*!< ��ָ��Ŀ¼,�ɼ���Ʊ����(ͬ��ִ��) */
	UnknownEvent
}EventType;/// Ԥ������¼�����
}
/// �����붨��
typedef enum _RunInfoCode
{
	SEARCH_NEW_DATABASE_FILE = 0,	///< ���ҿ�Ʊ���(�������ݿ�)

	FIND_COLLECT_DATABASE_FILE,		///< ������Ҫ�ɼ������Ʊ����

	START_COLLECT_INVOICE_DATA,		///< ��ʼ�ɼ���Ʊ
	COLLECT_DATA_COMPLETE,			///< �ɼ���Ʊ���

	FindBwPath,						///< �ҵ�������Ʊ�����װ·��
	FindHxPath,						///< �ҵ����ſ�Ʊ�����װ·��

	EnumDbFileFailed,				///< û���ҵ���Ʊ���,����ϵ����������̹�����Ա

	StartCheckDbChange,				///< ����������

	BwDbChanged,					///< ������Ʊ������ݿ�仯
	HxDbChanged,					///< ���ſ�Ʊ������ݿ�仯

	SendCompanyListSuccess,			///< �ύ�ɿ�Ʊ��ҵ�б�ɹ�
	SendCompanyListFailed,			///< �ύ�ɿ�Ʊ��ҵ�б�ʧ��

	RequestBelongSuccess,			///< ������С΢��ҵ�������˹�˾�б�ɹ�
	RequestBelongFailed,			///< ������С΢��ҵ�������˹�˾�б�ʧ��
	NoBelongCompany,				///< û�й����Ĵ��˹�˾

	SendInvoiceDataSuccess,			///< �������Ʊ���ݳɹ�
	SendInvoiceDataFailed,			///< �������Ʊ����ʧ��
	ManualSendInvoiceData,			///< �ֶ��������Ʊ����

	QueryBwDbComplete,				///< ��ѯ������Ʊ������ݿ����
	QueryHxDbComplete,				///< ��ѯ���ſ�Ʊ������ݿ����

	UpdateBwDbModifyTimeSuccess,	///< ���°������ݿ��ļ����ʱ��ɹ�
	UpdateHxDbModifyTimeSuccess,	///< ���º������ݿ��ļ����ʱ��ɹ�
	UpdateDbModifyTimeFailed,		///< �������ݿ��ļ����ʱ��ʧ��

	FindDbFilesFailed,				///< û���ҵ��������������ݿ��ļ�
	ConfirmCompanyFailed,			///< ���ƶ�ȷ�Ͽ�Ʊ��ҵ����ʧ��
	FindDeviceFailed,				///< ˰����δ����

	SelectCollectCompany,			///< ��������,ѡ����Ҫ�ɼ���Ʊ����ҵ
	SelectBelongCompany,			///< ��������,ѡ����ҵ�����Ĵ��˹�˾

	UnknownError					///< δ֪����

}RunInfoCode;/// ������Ϣ�붨��
bool GetRunInfoDesc(RunInfoCode emRunInfoCode, std::string &strDesc);

///	ͼ���û�����----��Ʊ�����б�
typedef enum _InvoiceColName
{
	ColCompany = 0,		/*!< ��Ʊ��ҵ */
	ColMakeDate,		/*!< ��Ʊ���� */
	ColInvoiceType,		/*!< ��Ʊ���� */
	ColInvoiceStatus,	/*!< ״̬ */
	ColUserName,		/*!< �ͻ�����*/
	ColProductName,		/*!< ��Ҫ��Ʒ���� */
	ColTaxNum,			/*!< ˰�� */
	ColPriceSum,		/*!< �ϼƽ�� */
	ColTaxSum,			/*!< ��˰�ϼ� */
	ColInvoiceCode,		/*!< ��Ʊ���� */
	ColInvoiceNum,		/*!< ��Ʊ���� */
	ColMakePerson,		/*!< ��Ʊ�� */

	ColCountInvoice		/*!< ����:����ʼ��������� */
}InvoiceColName;

///	ͼ���û�����----������Ϣ�б�
typedef enum _RunInfoColName
{
	ColDateTime = 0,	/*!< ��Ϣ����ʱ�� */
	ColContent,			/*!< ��Ϣ���� */

	ColCountRunInfo		/*!< ����:����ʼ��������� */
}RunInfoColName;

typedef enum _DbFileType
{
	HxDbFile = 0,		/*!< �������ݿ��ļ� */
	BwDbFile,			/*!< �������ݿ��ļ� */

	UnknownDbFileType
}DbFileType;

typedef enum _FolderType
{
	HxFolder = 0,		/*!< ���ſ�Ʊ���·�� */
	BwFolder,			/*!< ������Ʊ���·�� */

	UnknownFolderType
}FolderType;

typedef enum _ModalDlgType
{
	DlgLogin = 0,		/*!< ��¼���� */
	DlgSearch,			/*!< �����ļ����� */
	DlgFileCollect,		/*!< ���ļ��ɼ� */
	DlgFolderCollect,	/*!< ���ļ��вɼ� */
	DlgBwCollect,		/*!< �������Ʊ�ֶ��ɼ� */

	UnknownModalDlgType
}ModalDlgType;

/************************************************************************/
/* ����Windows��Ϣ                                                      */
/************************************************************************/
#define WM_SHOW_INVOICE_DATA		(WM_USER + 0x101)	/// �Զ�����Ϣ----��ʾ��Ʊ����
#define WM_SHOW_RUN_INFO			(WM_USER + 0x102)	/// �Զ�����Ϣ----��ʾ������Ϣ
#define WM_POPUP_DLG				(WM_USER + 0x103)	/// �Զ�����Ϣ----��������
#define WM_NOTIFY_ICON				(WM_USER + 0x104)	/// �Զ�����Ϣ----����ͼ��
#define WM_RUN_TIME_ERROR			(WM_USER + 0x105)	/// �Զ�����Ϣ----����ʱ����
#define WM_SEARCH_COMPLETE			(WM_USER + 0x106)	/// �Զ�����Ϣ----��ɴ�������
#define WM_FILE_COLLECT_COMPLETE	(WM_USER + 0x107)	/// �Զ�����Ϣ----��ɰ��ļ�����
#define WM_FOLDER_COLLECT_COMPLETE	(WM_USER + 0x108)	/// �Զ�����Ϣ----��ɰ��ļ��вɼ�
#define WM_SHOW_SEARCH_PATH			(WM_USER + 0x109)	/// �Զ�����Ϣ----��ʾ��ǰ����·��
#define WM_AUTO_SEARCH				(WM_USER + 0x10A)	/// �Զ�����Ϣ----ִ���Զ�����

#define WM_ENUM_USB_DEVICE_COMPLETE	(WM_USER + 0x10B)	/// �Զ�����Ϣ----ö��USB�豸����

///	��Ϣ����
//#define CLEAR_INVOICE_DATA_LIST		1					/*!< ������������Ʊ�����б� */
#define COLLECT_INVOICE_DATA		0					/*!< �ɼ������Ʊ���� */
#define NO_INVOICE_DATA				1					/*!< û�����Ʊ���� */
#define FOLDER_COLLECT_SUCCESS		0					/*!< ��·���ɼ��ɹ� */
#define NO_DB_FILE_IN_FOLDER		1					/*!< ��·��û���ҵ����ݿ��ļ� */
#define UNKNOWN_DB_FILE				2					/*!< δ֪���ݿ��ļ����� */


/************************************************************************/
/* ����ʱ��                                                             */
/************************************************************************/
#define EVENT_CHECK_INTERVAL			100		/*!< ���δ�����¼�ʱ����(����) */
#define MAX_REQUEST_SERVER_INTERVAL		60		/*!< ���ƶ������������ʱ����(��) */
#define NET_DEFAULT_TIMEOUT				30		/*!< Ĭ���������ӿͻ��˳�ʱʱ��(��) */
#define EXECUTE_TASK_DEFAULT_TIMEOUT	300		/*!< Ĭ��ִ������ͻ��˳�ʱʱ��(��) */
#define DEFAULT_CHECK_UPDATE_INTERVAL	60		/*!< Ĭ�ϼ�����ʱ����(��) */
#define MAX_RETRY_SEND_DATA_COUNT		3		/*!< �������ݸ��ƶ˵�������Դ��� */
#define RETRY_SEND_DATA_INTERVAL		15		/*!< �������������ƶ˵�����ʱ����(��) */
#define MAX_SEARCH_INTERVAL				15		/*!< �������������ʱ��(��)(���:����14) */

/************************************************************************/
/* ���������������                                                     */
/************************************************************************/
#define CONFIGURATION_FILE_NAME				"OutInvoiceService.xml"		/*!< Ĭ�������ļ��� */
#define CACHE_FILE_NAME						"Cache.dat"					/*!< Ĭ�ϻ����ļ��� */
#define DEFAULT_CHECK_UPDATE_URL			"http://interface.yunzhangfang.com:8082/interfacefilemanage/filemanage/api/update"
#define DEFAULT_CHECK_UPDATE_START_HOUR		2							/*!< Ĭ�ϼ�����ʱ���:��ʼʱ�� */
#define DEFAULT_CHECK_UPDATE_END_HOUR		3							/*!< Ĭ�ϼ�����ʱ���:����ʱ�� */


/************************************************************************/
/* ���峣�ò���                                                         */
/************************************************************************/
#define COUNT_OF(arr)   (sizeof(arr) / sizeof(arr[0]))

/************************************************************************/
/* �����ַ�������                                                       */
/************************************************************************/
#define START_COLLECT_SERVICE			"�����ɼ�����"
#define STOP_COLLECT_SERVICE			"ֹͣ�ɼ�����"
#define SHOW_MAIN_WND					"��ʾ����"
#define HIDE_MAIN_WND					"���س���"
#define CANCEL_WND						"ȡ��"
#define CLOSE_WND						"�ر�"
#define HX_SEARCH_KEY					"MAINEXECUTE.EXE"
#define BW_SEARCH_KEY					"KP.EXE"
#define SEARCH_FILTER_PATH				"C:\\WINDOWS"
#define HX_DB_FILE_NAME					"cc3268.dll"
#define HX_DLL_FILE_NAME				"SQLite.Interop.dll"
#define HX_DB_FILE_PARENT				"BIN"
#define BW_DB_FILE_EXT					".DAT"
#define BW_FILTER_FILE1					"LOG"
#define BW_FILTER_FILE2					"SYSTEM"
#define BW_DLL_FILE_NAME				"sqlite3.dll"
#define SEARCH_COMPLETE_SET_FILE_TIME	"2015-03-10 00:00:00"		/*!< ȫ���������,�������ݿ��ļ��޸�ʱ�� */
#define COMPANY_CODE_LENGTH1			15							/*!< ��Ч��˰��ʶ��ų���1 */
#define COMPANY_CODE_LENGTH2			18							/*!< ��Ч��˰��ʶ��ų���2 */
#define MAX_INVOICE_DATA_ROW			150							/*!< ��Ϣ��Ʊ�����б�������� */
#define MAX_SHOW_INFO_LIST_ROW			50							/*!< ������Ϣ��ʾ�б�������� */
#define STRING_TRUE						"TRUE"
#define STRING_FALSE					"FALSE"
#define MANUAL_COLLECT					"�ֶ��ɼ�"
#define AUTO_COLLECT					"�Զ��ɼ�"

#define COLLECT_RST_NO_DATA				"�ɼ��ڼ�,�����Ʊ����"
#define COLLECT_RST_SUCCESS				"�ɼ��ɹ�"
#define COLLECT_RST_SEND_FAILED			"���ƶ��ύ���Ʊ����ʧ��"

#define USB_DEVICE_HARDWARE_ID			"USBSTOR\\DiskNISEC___TCG-01__________5.00"
#define USB_DEVICE_FRIENDLY_NAME		"NISEC TCG-01 USB Device"


//////////////////////////////////////////////////////////////////////////
///	XML�ļ�Э��
#define Cache_DbFiles			"DbFiles"
#define Cache_LastSearchTime	"LastSearchTime"
#define Cache_InstallPath		"InstallPath"
#define Cache_Path				"Path"
#define Cache_HxServerPath		"HxServerInstallPath"
#define Cache_DbFileInfo		"DbFileInfo"

#define Cache_FileName			"FileName"
#define Cache_TaxCode			"TaxCode"
#define Cache_CompanyName		"CompanyName"
#define Cache_ModifyTime		"ModifyTime"
#define Cache_DllFileName		"DllFileName"
#define Cache_LastCollectTime	"LastCollectTime"
#define Cache_CollectResult		"CollectResult"


/************************************************************************/
/* ���嵥������                                                         */
/************************************************************************/
#define AutoStartCtrl	CAutoStart::Instance()
#define ErrorHandle		CErrorHandle::Instance()


/************************************************************************/
/* �����б���ʾ����                                                     */
/************************************************************************/
typedef enum _ColName
{
	ColCollectType = 0,	/// �ɼ���ʽ
	ColTaxCode,			/// ��˰��ʶ���
	ColCompanyName,		/// ��ҵ����
	ColLastCollectTime,	///	���һ�βɼ��ɹ�ʱ��
	ColCollectResult,	/// �ɼ����
	ColDbFile			/// ���ݿ��ļ�
};

/// �б���ʾ����,˳��ͬColName
/// 0:AutoCollect 1:TaxCode; 2:CompanyName 3:LastCollectTime 4:CollectResult 5:DbFileName;
typedef std::tuple<bool, std::string, std::string, std::string, std::string, std::string> ListRowData;

class CListShowData
{
	DEFINE_BOOST_SHARED_PTR(CListShowData)
public:

	virtual ~CListShowData(void)
	{
		m_vctListData.clear();
	}

protected:
	explicit CListShowData(void)
	{
		m_vctListData.clear();
	}

public:
	std::vector<std::map<int, std::string>> m_vctListData;
};


/************************************************************************/
/* ������                                                             */
/************************************************************************/
class CErrorHandle
{
public:
	static CErrorHandle *Instance(void)
	{
		static CErrorHandle obj;
		return &obj;
	}

	virtual ~CErrorHandle(void) { m_strLastError.clear(); }

	const std::string & GetLastError(void) { return m_strLastError; }

	void SetLastError(const std::string &strError) { m_strLastError = strError; }

protected:
	explicit CErrorHandle(void) { m_strLastError.clear(); }

protected:
	std::string m_strLastError;
};


/************************************************************************/
/* �ź���                                                               */
/************************************************************************/
class CMySemaphore
{
public:
	typedef boost::shared_ptr<CMySemaphore> pointer;
	static CMySemaphore::pointer Create(unsigned int nInitialCount = 0)
	{
		return CMySemaphore::pointer(new CMySemaphore(nInitialCount));
	}

	virtual ~CMySemaphore(void) {}

	void Post(void)
	{
		m_semaphore.post();
	}

	void Wait(void)
	{
		m_semaphore.wait();
	}

protected:
	explicit CMySemaphore(unsigned int nInitialCount = 0) : m_semaphore(nInitialCount)
	{
	}

protected:
	boost::interprocess::interprocess_semaphore m_semaphore;
};
