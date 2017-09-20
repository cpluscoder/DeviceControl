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

	{SEARCH_NEW_DATABASE_FILE,		"���ҿ�Ʊ���"},

	{FIND_COLLECT_DATABASE_FILE,	"������Ҫ�ɼ������Ʊ����"},/// (60��)

	{START_COLLECT_INVOICE_DATA,	"��ʼ�ɼ���Ʊ"},
	{COLLECT_DATA_COMPLETE,			"�ɼ���Ʊ���"},

	//{FindBwPath,					"�ҵ�������Ʊ�����װ·��"},
	//{FindHxPath,					"�ҵ����ſ�Ʊ�����װ·��"},

	{EnumDbFileFailed,				"û���ҵ���Ʊ���,����ϵ����������̹�����Ա"},

	//{StartCheckDbChange,			"����������(35�����Һ�ˢ��)"},

	{BwDbChanged,					"������Ʊ������ݿ�仯"},
	{HxDbChanged,					"���ſ�Ʊ������ݿ�仯"},

	//{SendCompanyListSuccess,		"�ύ�ɿ�Ʊ��ҵ�б�ɹ�"},
	//{SendCompanyListFailed,		"�ύ�ɿ�Ʊ��ҵ�б�ʧ��"},

	{RequestBelongSuccess,			"������С΢��ҵ�������˹�˾�б�ɹ�"},
	{RequestBelongFailed,			"������С΢��ҵ�������˹�˾�б�ʧ��"},
	{NoBelongCompany,				"û�й����Ĵ��˹�˾"},

	{SendInvoiceDataSuccess,		"�������Ʊ���ݳɹ�"},
	{SendInvoiceDataFailed,			"�������Ʊ����ʧ��"},
	//{ManualSendInvoiceData,		"�ֶ��������Ʊ����"},

	//{QueryBwDbComplete,			"��ѯ������Ʊ������ݿ����"},
	//{QueryHxDbComplete,			"��ѯ���ſ�Ʊ������ݿ����"},

	{UpdateBwDbModifyTimeSuccess,	"���°������ݿ��ļ����ʱ��ɹ�"},
	{UpdateHxDbModifyTimeSuccess,	"���º������ݿ��ļ����ʱ��ɹ�"},
	{UpdateDbModifyTimeFailed,		"�������ݿ��ļ����ʱ��ʧ��"},

	{FindDbFilesFailed,				"û���ҵ��������������ݿ��ļ�"},
	{ConfirmCompanyFailed,			"���ƶ�ȷ�Ͽ�Ʊ��ҵ����ʧ��"},
	{FindDeviceFailed,				"˰����δ����"},

	//{SelectCollectCompany,	"��������,ѡ����Ҫ�ɼ���Ʊ����ҵ"},
	//{SelectBelongCompany,		"��������,ѡ����ҵ���������˹�˾"},

	{UnknownError,					"δ֪����"}
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
