#pragma once


#include <string>
#include <map>
#include <vector>
#include <tuple>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>


/*!< 申明BOOST智能指针 */
#define DECLARE_BOOST_SHARED_PTR(ClassName)	\
public: \
	typedef boost::shared_ptr<ClassName> Pointer;	\
protected:	\
	ClassName(const ClassName &);	\
	ClassName &operator= (const ClassName &);


/*!< 定义BOOST智能指针 */
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
	StartSearchNewDbFile = 0,		/*!< 在已知目录,查找数据库文件 */
	DbFileChanged,					/*!< 发现数据库文件已修改 */
	QueryDbComplete,				/*!< 查询数据库完成 */
	SendData2WebServerComplete,		/*!< 向云端发送销项发票数据完成 */
	StartFileCollect,				/*!< 开始单个文件采集(同步执行) */
	StartFolderCollect,				/*!< 在指定目录,采集发票数据(同步执行) */
	UnknownEvent
}EventType;/// 预定义的事件类型
}
/// 错误码定义
typedef enum _RunInfoCode
{
	SEARCH_NEW_DATABASE_FILE = 0,	///< 查找开票软件(新增数据库)

	FIND_COLLECT_DATABASE_FILE,		///< 查找需要采集的销项发票数据

	START_COLLECT_INVOICE_DATA,		///< 开始采集发票
	COLLECT_DATA_COMPLETE,			///< 采集发票完成

	FindBwPath,						///< 找到百旺开票软件安装路径
	FindHxPath,						///< 找到航信开票软件安装路径

	EnumDbFileFailed,				///< 没有找到开票软件,请联系软件开发厂商工作人员

	StartCheckDbChange,				///< 程序运行中

	BwDbChanged,					///< 百旺开票软件数据库变化
	HxDbChanged,					///< 航信开票软件数据库变化

	SendCompanyListSuccess,			///< 提交可开票企业列表成功
	SendCompanyListFailed,			///< 提交可开票企业列表失败

	RequestBelongSuccess,			///< 请求中小微企业所属代账公司列表成功
	RequestBelongFailed,			///< 请求中小微企业所属代账公司列表失败
	NoBelongCompany,				///< 没有归属的代账公司

	SendInvoiceDataSuccess,			///< 发送销项发票数据成功
	SendInvoiceDataFailed,			///< 发送销项发票数据失败
	ManualSendInvoiceData,			///< 手动发送销项发票数据

	QueryBwDbComplete,				///< 查询百旺开票软件数据库完成
	QueryHxDbComplete,				///< 查询航信开票软件数据库完成

	UpdateBwDbModifyTimeSuccess,	///< 更新百旺数据库文件检查时间成功
	UpdateHxDbModifyTimeSuccess,	///< 更新航信数据库文件检查时间成功
	UpdateDbModifyTimeFailed,		///< 更新数据库文件检查时间失败

	FindDbFilesFailed,				///< 没有找到百旺、航信数据库文件
	ConfirmCompanyFailed,			///< 向云端确认开票企业归属失败
	FindDeviceFailed,				///< 税控盘未连接

	SelectCollectCompany,			///< 弹出窗口,选择需要采集发票的企业
	SelectBelongCompany,			///< 弹出窗口,选择企业所属的代账公司

	UnknownError					///< 未知错误

}RunInfoCode;/// 运行信息码定义
bool GetRunInfoDesc(RunInfoCode emRunInfoCode, std::string &strDesc);

///	图形用户界面----发票数据列表
typedef enum _InvoiceColName
{
	ColCompany = 0,		/*!< 开票企业 */
	ColMakeDate,		/*!< 开票日期 */
	ColInvoiceType,		/*!< 发票类型 */
	ColInvoiceStatus,	/*!< 状态 */
	ColUserName,		/*!< 客户名称*/
	ColProductName,		/*!< 主要商品名称 */
	ColTaxNum,			/*!< 税额 */
	ColPriceSum,		/*!< 合计金额 */
	ColTaxSum,			/*!< 价税合计 */
	ColInvoiceCode,		/*!< 发票代码 */
	ColInvoiceNum,		/*!< 发票号码 */
	ColMakePerson,		/*!< 开票人 */

	ColCountInvoice		/*!< 列数:保持始终是最后定义 */
}InvoiceColName;

///	图形用户界面----运行信息列表
typedef enum _RunInfoColName
{
	ColDateTime = 0,	/*!< 信息产生时间 */
	ColContent,			/*!< 信息内容 */

	ColCountRunInfo		/*!< 列数:保持始终是最后定义 */
}RunInfoColName;

typedef enum _DbFileType
{
	HxDbFile = 0,		/*!< 航信数据库文件 */
	BwDbFile,			/*!< 百旺数据库文件 */

	UnknownDbFileType
}DbFileType;

typedef enum _FolderType
{
	HxFolder = 0,		/*!< 航信开票软件路径 */
	BwFolder,			/*!< 百旺开票软件路径 */

	UnknownFolderType
}FolderType;

typedef enum _ModalDlgType
{
	DlgLogin = 0,		/*!< 登录设置 */
	DlgSearch,			/*!< 磁盘文件搜索 */
	DlgFileCollect,		/*!< 按文件采集 */
	DlgFolderCollect,	/*!< 按文件夹采集 */
	DlgBwCollect,		/*!< 百旺销项发票手动采集 */

	UnknownModalDlgType
}ModalDlgType;

/************************************************************************/
/* 定义Windows消息                                                      */
/************************************************************************/
#define WM_SHOW_INVOICE_DATA		(WM_USER + 0x101)	/// 自定义消息----显示发票数据
#define WM_SHOW_RUN_INFO			(WM_USER + 0x102)	/// 自定义消息----显示运行信息
#define WM_POPUP_DLG				(WM_USER + 0x103)	/// 自定义消息----弹出窗口
#define WM_NOTIFY_ICON				(WM_USER + 0x104)	/// 自定义消息----托盘图标
#define WM_RUN_TIME_ERROR			(WM_USER + 0x105)	/// 自定义消息----运行时错误
#define WM_SEARCH_COMPLETE			(WM_USER + 0x106)	/// 自定义消息----完成磁盘搜索
#define WM_FILE_COLLECT_COMPLETE	(WM_USER + 0x107)	/// 自定义消息----完成按文件搜索
#define WM_FOLDER_COLLECT_COMPLETE	(WM_USER + 0x108)	/// 自定义消息----完成按文件夹采集
#define WM_SHOW_SEARCH_PATH			(WM_USER + 0x109)	/// 自定义消息----显示当前搜索路径
#define WM_AUTO_SEARCH				(WM_USER + 0x10A)	/// 自定义消息----执行自动搜索

#define WM_ENUM_USB_DEVICE_COMPLETE	(WM_USER + 0x10B)	/// 自定义消息----枚举USB设备结束

///	消息参数
//#define CLEAR_INVOICE_DATA_LIST		1					/*!< 清空主窗口销项发票数据列表 */
#define COLLECT_INVOICE_DATA		0					/*!< 采集到销项发票数据 */
#define NO_INVOICE_DATA				1					/*!< 没有销项发票数据 */
#define FOLDER_COLLECT_SUCCESS		0					/*!< 按路径采集成功 */
#define NO_DB_FILE_IN_FOLDER		1					/*!< 按路径没有找到数据库文件 */
#define UNKNOWN_DB_FILE				2					/*!< 未知数据库文件类型 */


/************************************************************************/
/* 定义时间                                                             */
/************************************************************************/
#define EVENT_CHECK_INTERVAL			100		/*!< 检查未处理事件时间间隔(毫秒) */
#define MAX_REQUEST_SERVER_INTERVAL		60		/*!< 向云端请求任务最大时间间隔(秒) */
#define NET_DEFAULT_TIMEOUT				30		/*!< 默认网络连接客户端超时时间(秒) */
#define EXECUTE_TASK_DEFAULT_TIMEOUT	300		/*!< 默认执行任务客户端超时时间(秒) */
#define DEFAULT_CHECK_UPDATE_INTERVAL	60		/*!< 默认检查更新时间间隔(秒) */
#define MAX_RETRY_SEND_DATA_COUNT		3		/*!< 发送数据给云端的最大重试次数 */
#define RETRY_SEND_DATA_INTERVAL		15		/*!< 发送任务结果给云端的重试时间间隔(秒) */
#define MAX_SEARCH_INTERVAL				15		/*!< 磁盘搜索最大间隔时间(天)(详见:需求14) */

/************************************************************************/
/* 定义程序运行配置                                                     */
/************************************************************************/
#define CONFIGURATION_FILE_NAME				"OutInvoiceService.xml"		/*!< 默认配置文件名 */
#define CACHE_FILE_NAME						"Cache.dat"					/*!< 默认缓存文件名 */
#define DEFAULT_CHECK_UPDATE_URL			"http://interface.yunzhangfang.com:8082/interfacefilemanage/filemanage/api/update"
#define DEFAULT_CHECK_UPDATE_START_HOUR		2							/*!< 默认检查更新时间段:开始时间 */
#define DEFAULT_CHECK_UPDATE_END_HOUR		3							/*!< 默认检查更新时间段:结束时间 */


/************************************************************************/
/* 定义常用操作                                                         */
/************************************************************************/
#define COUNT_OF(arr)   (sizeof(arr) / sizeof(arr[0]))

/************************************************************************/
/* 定义字符串常量                                                       */
/************************************************************************/
#define START_COLLECT_SERVICE			"启动采集服务"
#define STOP_COLLECT_SERVICE			"停止采集服务"
#define SHOW_MAIN_WND					"显示程序"
#define HIDE_MAIN_WND					"隐藏程序"
#define CANCEL_WND						"取消"
#define CLOSE_WND						"关闭"
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
#define SEARCH_COMPLETE_SET_FILE_TIME	"2015-03-10 00:00:00"		/*!< 全盘搜索完成,设置数据库文件修改时间 */
#define COMPANY_CODE_LENGTH1			15							/*!< 有效纳税人识别号长度1 */
#define COMPANY_CODE_LENGTH2			18							/*!< 有效纳税人识别号长度2 */
#define MAX_INVOICE_DATA_ROW			150							/*!< 消息发票数据列表最大行数 */
#define MAX_SHOW_INFO_LIST_ROW			50							/*!< 运行信息显示列表最大行数 */
#define STRING_TRUE						"TRUE"
#define STRING_FALSE					"FALSE"
#define MANUAL_COLLECT					"手动采集"
#define AUTO_COLLECT					"自动采集"

#define COLLECT_RST_NO_DATA				"采集期间,无销项发票数据"
#define COLLECT_RST_SUCCESS				"采集成功"
#define COLLECT_RST_SEND_FAILED			"向云端提交销项发票数据失败"

#define USB_DEVICE_HARDWARE_ID			"USBSTOR\\DiskNISEC___TCG-01__________5.00"
#define USB_DEVICE_FRIENDLY_NAME		"NISEC TCG-01 USB Device"


//////////////////////////////////////////////////////////////////////////
///	XML文件协议
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
/* 定义单例对象                                                         */
/************************************************************************/
#define AutoStartCtrl	CAutoStart::Instance()
#define ErrorHandle		CErrorHandle::Instance()


/************************************************************************/
/* 界面列表显示数据                                                     */
/************************************************************************/
typedef enum _ColName
{
	ColCollectType = 0,	/// 采集方式
	ColTaxCode,			/// 纳税人识别号
	ColCompanyName,		/// 企业名称
	ColLastCollectTime,	///	最后一次采集成功时间
	ColCollectResult,	/// 采集结果
	ColDbFile			/// 数据库文件
};

/// 列表显示数据,顺序同ColName
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
/* 错误处理                                                             */
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
/* 信号量                                                               */
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
