#include "stdafx.h"
#include "CommonDef.h"
#include <boost/algorithm/string.hpp>

using namespace std;


static const struct
{
	RunInfoCode emRunInfoCode;
	const char *pszRunInfoDesc;
}
ErrorItems[] = {

	{SEARCH_NEW_DATABASE_FILE,		"查找开票软件"},

	{FIND_COLLECT_DATABASE_FILE,	"查找需要采集的销项发票数据"},/// (60秒)

	{START_COLLECT_INVOICE_DATA,	"开始采集发票"},
	{COLLECT_DATA_COMPLETE,			"采集发票完成"},

	//{FindBwPath,					"找到百旺开票软件安装路径"},
	//{FindHxPath,					"找到航信开票软件安装路径"},

	{EnumDbFileFailed,				"没有找到开票软件,请联系软件开发厂商工作人员"},

	//{StartCheckDbChange,			"程序运行中(35秒左右后刷新)"},

	{BwDbChanged,					"百旺开票软件数据库变化"},
	{HxDbChanged,					"航信开票软件数据库变化"},

	//{SendCompanyListSuccess,		"提交可开票企业列表成功"},
	//{SendCompanyListFailed,		"提交可开票企业列表失败"},

	{RequestBelongSuccess,			"请求中小微企业所属代账公司列表成功"},
	{RequestBelongFailed,			"请求中小微企业所属代账公司列表失败"},
	{NoBelongCompany,				"没有归属的代账公司"},

	{SendInvoiceDataSuccess,		"发送销项发票数据成功"},
	{SendInvoiceDataFailed,			"发送销项发票数据失败"},
	//{ManualSendInvoiceData,		"手动发送销项发票数据"},

	//{QueryBwDbComplete,			"查询百旺开票软件数据库完成"},
	//{QueryHxDbComplete,			"查询航信开票软件数据库完成"},

	{UpdateBwDbModifyTimeSuccess,	"更新百旺数据库文件检查时间成功"},
	{UpdateHxDbModifyTimeSuccess,	"更新航信数据库文件检查时间成功"},
	{UpdateDbModifyTimeFailed,		"更新数据库文件检查时间失败"},

	{FindDbFilesFailed,				"没有找到百旺、航信数据库文件"},
	{ConfirmCompanyFailed,			"向云端确认开票企业归属失败"},
	{FindDeviceFailed,				"税控盘未连接"},

	//{SelectCollectCompany,	"弹出窗口,选择需要采集发票的企业"},
	//{SelectBelongCompany,		"弹出窗口,选择企业所归属代账公司"},

	{UnknownError,					"未知错误"}
};

bool GetRunInfoDesc(RunInfoCode emRunInfoCode, string &strDesc)
{
	strDesc.clear();

	int i = 0;
	int nSize = COUNT_OF(ErrorItems);
	for(i = 0; i < nSize; i++)
	{
		if(emRunInfoCode == ErrorItems[i].emRunInfoCode)
		{
			strDesc = ErrorItems[i].pszRunInfoDesc;
			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}
