
// DetectNISECDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DetectNISEC.h"
#include "DetectNISECDlg.h"
#include "afxdialogex.h"
#include <dbt.h>
#include "UsbEnumerator.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDetectNISECDlg 对话框




CDetectNISECDlg::CDetectNISECDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDetectNISECDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDetectNISECDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDetectNISECDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_TEST, &CDetectNISECDlg::OnBnClickedBtnTest)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDetectNISECDlg 消息处理程序

BOOL CDetectNISECDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	const GUID guidDEVINTERFACE_USB_DEVICE = { 0xA5DCBF10L, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } };
	if(!DoRegisterDeviceInterfaceToHwnd(guidDEVINTERFACE_USB_DEVICE, GetSafeHwnd(), &m_hDeviceNotify))
	{
		OutputDebugString(TEXT("DoRegisterDeviceInterfaceToHwnd failed"));
		return FALSE;
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDetectNISECDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDetectNISECDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDetectNISECDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDetectNISECDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(!UnregisterDeviceNotification(m_hDeviceNotify))
	{
		OutputDebugString(TEXT("UnregisterDeviceNotification failed"));
	}

	CDialogEx::OnClose();
}


LRESULT CDetectNISECDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(message == WM_DEVICECHANGE)
	{
/**	@note	
	This is the actual message from the interface via Windows messaging.
	This code includes some additional decoding for this particular device type and some common validation checks.
	Note that not all devices utilize these optional parameters in the same way.
	Refer to the extended information for your particular device type specified by your GUID.
*/
		PDEV_BROADCAST_DEVICEINTERFACE b = (PDEV_BROADCAST_DEVICEINTERFACE)lParam;
		switch(wParam)
		{
		case DBT_DEVICEARRIVAL:
			{
				OutputDebugString("DBT_DEVICEARRIVAL\n");
			}
			break;
		case DBT_DEVICEREMOVECOMPLETE:
			{
				OutputDebugString("DBT_DEVICEREMOVECOMPLETE\n");
			}
			break;
		case DBT_DEVNODES_CHANGED:
			{
				OutputDebugString("DBT_DEVNODES_CHANGED\n");
			}
			break;
		default:
			{
				TRACE("WM_DEVICECHANGE message received, value %d unhandled.\n", wParam);
			}
			break;
		}
	}
/*
	switch(message)
	{
	//case WM_PAINT:
	//	{
	//		PAINTSTRUCT ps;
	//		HDC hdc = BeginPaint(hWnd, &ps);
	//		EndPaint(hWnd, &ps);
	//	}
	//	break;
	case WM_DEVICECHANGE:
		{
			if((wParam == DBT_DEVICEARRIVAL) || (wParam == DBT_DEVICEREMOVECOMPLETE))
			{
				try
				{
					DEV_BROADCAST_HDR* header = reinterpret_cast<DEV_BROADCAST_HDR*>(lParam);
					if(header->dbch_devicetype == DBT_DEVTYP_VOLUME)
					{
						DEV_BROADCAST_VOLUME* devNot = reinterpret_cast<DEV_BROADCAST_VOLUME*>(lParam);
						std::vector<std::wstring> drives = DrivesFromMask(devNot->dbcv_unitmask);
						for(std::vector<std::wstring>::const_iterator it = drives.begin(); it != drives.end(); ++it)
						{
							if(DBT_DEVICEARRIVAL == wParam)
							{
								g_NotifyWindow->deviceChanged_.VolumeArrival(*it);
							}
							else
							{
								g_NotifyWindow->deviceChanged_.VolumeRemoved(*it);
							}
						}
					}
					if(header->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
					{
						DEV_BROADCAST_DEVICEINTERFACE* devNot = reinterpret_cast<DEV_BROADCAST_DEVICEINTERFACE*>(lParam);
						if(DBT_DEVICEARRIVAL == wParam)
						{
							g_NotifyWindow->deviceChanged_.InterfaceArrival(devNot->dbcc_classguid);
						}
						else
						{
							g_NotifyWindow->deviceChanged_.InterfaceRemoved(Utilities::StringUpper(devNot->dbcc_name));
						}
					}
				}
				catch(const std::runtime_error& ex)
				{
					ex.what();
					//"ERROR: Processing WM_DEVICECHANGE failed
				}
			}
		}
		break;
	//case WM_DESTROY:
	//	{
	//		PostQuitMessage(0);
	//	}
	//	break;
	default:
		break;
	}
*/

	return CDialogEx::DefWindowProc(message, wParam, lParam);
}

BOOL CDetectNISECDlg::DoRegisterDeviceInterfaceToHwnd(IN GUID InterfaceClassGuid, IN HWND hWnd, OUT HDEVNOTIFY *hDeviceNotify)
{
	DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;

	ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));
	NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	NotificationFilter.dbcc_classguid = InterfaceClassGuid;

	*hDeviceNotify = ::RegisterDeviceNotification( 
		hWnd,                       // events recipient
		&NotificationFilter,        // type of device
		DEVICE_NOTIFY_WINDOW_HANDLE // type of recipient handle
		);

	if(NULL == *hDeviceNotify)
	{
		OutputDebugString(TEXT("RegisterDeviceNotification"));
		return FALSE;
	}

	return TRUE;
}


void CDetectNISECDlg::OnBnClickedBtnTest()
{
	// TODO: 在此添加控件通知处理程序代码
	CUsbEnumerator::Pointer pUsbEnumerator = CUsbEnumerator::Create();
	pUsbEnumerator->Start();
}
