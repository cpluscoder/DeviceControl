
// ReadUsbNum.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CReadUsbNumApp:
// �йش����ʵ�֣������ ReadUsbNum.cpp
//

class CReadUsbNumApp : public CWinApp
{
public:
	CReadUsbNumApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CReadUsbNumApp theApp;