
// DetectNISECDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DetectNISEC.h"
#include "DetectNISECDlg.h"
#include "afxdialogex.h"
#include <dbt.h>
#include "InquireNumber.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace std;

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
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_ENUM_USB_DEVICE_COMPLETE, &CDetectNISECDlg::OnEnumDeviceComplete)
#if SHOW_NOTIFY_ICON
	ON_MESSAGE(WM_NOTIFY_ICON, &CDetectNISECDlg::OnIconNotification)
	ON_BN_CLICKED(IDC_BTN_SHOW_TIP, &CDetectNISECDlg::OnBnClickedBtnShowTip)
#endif
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
	m_pUsbEnumerator = CUsbEnumerator::Create();

	const GUID guidDEVINTERFACE_USB_DEVICE = { 0xA5DCBF10L, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } };
	if(!DoRegisterDeviceInterfaceToHwnd(guidDEVINTERFACE_USB_DEVICE, GetSafeHwnd(), &m_hDeviceNotify))
	{
		OutputDebugString(TEXT("DoRegisterDeviceInterfaceToHwnd failed"));
		return FALSE;
	}
#if	SHOW_NOTIFY_ICON
	/// 托盘图标
	memset(&m_NotifyIconData, '\0', sizeof(m_NotifyIconData));
	m_NotifyIconData.cbSize = sizeof(NOTIFYICONDATA);
	m_NotifyIconData.hIcon = ::AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_NotifyIconData.hWnd = GetSafeHwnd();
	strcpy_s(m_NotifyIconData.szTip, 128, "USB设备检查程序");
	m_NotifyIconData.uFlags = NIF_MESSAGE | NIF_TIP | NIF_ICON;
	m_NotifyIconData.uID = IDR_MAINFRAME;
	m_NotifyIconData.uCallbackMessage = WM_NOTIFY_ICON;
	::Shell_NotifyIcon(NIM_ADD, &m_NotifyIconData);

	/// 载入菜单
	m_popupMenu.LoadMenu(IDR_MENU_POPUP);
#endif
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDetectNISECDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
#if	SHOW_NOTIFY_ICON
	else if(nID == SC_MINIMIZE)
	{
		/// 最小化
		ShowWindow(SW_HIDE);
	}
#endif
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
#if	SHOW_NOTIFY_ICON
	::Shell_NotifyIcon(NIM_DELETE, &m_NotifyIconData);	//程序退出时删除系统托盘图标
#endif
	CDialogEx::OnClose();
}

void CDetectNISECDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if(!UnregisterDeviceNotification(m_hDeviceNotify))
	{
		OutputDebugString(TEXT("UnregisterDeviceNotification failed"));
	}
#if	SHOW_NOTIFY_ICON
	::Shell_NotifyIcon(NIM_DELETE, &m_NotifyIconData);	//程序退出时删除系统托盘图标
#endif
}

LRESULT CDetectNISECDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(message == WM_DEVICECHANGE)
	{
		//TRACE("WM_DEVICECHANGE\n");
/**	@note	
	This is the actual message from the interface via Windows messaging.
	This code includes some additional decoding for this particular device type and some common validation checks.
	Note that not all devices utilize these optional parameters in the same way.
	Refer to the extended information for your particular device type specified by your GUID.
*/
		switch(wParam)
		{
		case DBT_DEVICEARRIVAL:
			{
				OnDeviceArrival(lParam);
			}
			break;
		case DBT_DEVICEREMOVECOMPLETE:
			{
				OnDeviceRemoveComplete(lParam);
			}
			break;
		case DBT_DEVNODES_CHANGED:
			{
				//OutputDebugString("DBT_DEVNODES_CHANGED\n");
			}
			break;
		default:
			{
				TRACE("WM_DEVICECHANGE message received, value %d unhandled.\n", wParam);
			}
			break;
		}
	}

	return CDialogEx::DefWindowProc(message, wParam, lParam);
}

bool CDetectNISECDlg::DoRegisterDeviceInterfaceToHwnd(IN GUID InterfaceClassGuid, IN HWND hWnd, OUT HDEVNOTIFY *hDeviceNotify)
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
		OutputDebugString("RegisterDeviceNotification failed");
		return false;
	}

	return true;
}

#if	SHOW_NOTIFY_ICON
LRESULT CDetectNISECDlg::OnIconNotification(WPARAM wParam, LPARAM lParam)
{
	if(wParam != m_NotifyIconData.uID)
	{
		return FALSE;
	}
	switch(lParam)
	{
	case WM_LBUTTONDOWN:
		{
			BOOL bVisible = ::IsWindowVisible(GetSafeHwnd());
			ShowWindow(bVisible ? SW_HIDE : SW_SHOW);
			if(!bVisible)
			{
				SetForegroundWindow();
			}
		}
		break;
	case WM_RBUTTONDOWN:
		{
			CPoint point;
			::GetCursorPos(&point);
			SetForegroundWindow();
			PopupMenu(TPM_LEFTBUTTON | TPM_RIGHTALIGN, point.x, point.y);
		}
		break;
	default:
		break;
	}

	return 0;
}

void CDetectNISECDlg::PopupMenu(UINT nFlags, int x, int y)
{
	CMenu * pMenu = m_popupMenu.GetSubMenu(0);
	if(pMenu != nullptr)
	{
		pMenu->TrackPopupMenu(nFlags, x, y, this);
	}
}

bool CDetectNISECDlg::ShowBalloonTip(LPCTSTR szTitle, LPCTSTR szMsg, UINT uTimeout, DWORD dwInfoFlags)
{
	m_NotifyIconData.cbSize = sizeof(NOTIFYICONDATA);
	m_NotifyIconData.uFlags = NIF_INFO;
	m_NotifyIconData.uTimeout = uTimeout;
	m_NotifyIconData.dwInfoFlags = dwInfoFlags;

	strcpy_s(m_NotifyIconData.szInfoTitle, szTitle);
	strcpy_s(m_NotifyIconData.szInfo, szMsg);

	return Shell_NotifyIcon(NIM_MODIFY, &m_NotifyIconData) == TRUE;
}

void CDetectNISECDlg::OnBnClickedBtnShowTip()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowBalloonTip("提示", "发现新硬件");
}

#endif


void CDetectNISECDlg::OnDeviceArrival(LPARAM lParam)
{
	//OutputDebugString("CDetectNISECDlg::OnDeviceArrival\n");

	string strGuid, strDeviceName;
	if(GetChangeDeviceInfo(lParam, strGuid, strDeviceName))
	{
		if(m_pUsbEnumerator)
		{
			m_pUsbEnumerator->Start(GetSafeHwnd(), USB_DEVICE_HARDWARE_ID, USB_DEVICE_FRIENDLY_NAME);
		}
	}
}

void CDetectNISECDlg::OnDeviceRemoveComplete(LPARAM lParam)
{
	OutputDebugString("CDetectNISECDlg::OnDeviceRemoveComplete\n");

	string strGuid, strDeviceName;
	GetChangeDeviceInfo(lParam, strGuid, strDeviceName);

	//if(m_pUsbEnumerator)
	//{
	//	m_pUsbEnumerator->Start(GetSafeHwnd(), USB_DEVICE_HARDWARE_ID, USB_DEVICE_FRIENDLY_NAME);
	//}
}

bool CDetectNISECDlg::GetChangeDeviceInfo(LPARAM lParam, string &strGuid, string &strDeviceName)
{
	bool bSuccess = false;

	do 
	{
		try
		{
			DEV_BROADCAST_HDR* pDeviceHeader = reinterpret_cast<DEV_BROADCAST_HDR *>(lParam);
			if(pDeviceHeader == nullptr)
			{
				break;
			}
			if(DBT_DEVTYP_DEVICEINTERFACE != pDeviceHeader->dbch_devicetype)
			{
				break;
			}

			PDEV_BROADCAST_DEVICEINTERFACE pDeviceBroadcast = (PDEV_BROADCAST_DEVICEINTERFACE)lParam;
			if(pDeviceBroadcast == nullptr)
			{
				break;
			}
			strDeviceName = boost::algorithm::to_upper_copy(string(pDeviceBroadcast->dbcc_name));

			char szBuf[256];
			ZeroMemory(szBuf, sizeof(szBuf));
			sprintf_s(
				szBuf, 
				sizeof(szBuf), 
				"{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}",
				pDeviceBroadcast->dbcc_classguid.Data1,
				pDeviceBroadcast->dbcc_classguid.Data2,
				pDeviceBroadcast->dbcc_classguid.Data3,
				pDeviceBroadcast->dbcc_classguid.Data4[0],
				pDeviceBroadcast->dbcc_classguid.Data4[1],
				pDeviceBroadcast->dbcc_classguid.Data4[2],
				pDeviceBroadcast->dbcc_classguid.Data4[3],
				pDeviceBroadcast->dbcc_classguid.Data4[4],
				pDeviceBroadcast->dbcc_classguid.Data4[5],
				pDeviceBroadcast->dbcc_classguid.Data4[6],
				pDeviceBroadcast->dbcc_classguid.Data4[7]);

			strGuid = boost::algorithm::to_upper_copy(string(szBuf));

			TRACE("name:%s\nguid:%s\n", strDeviceName.c_str(), strGuid.c_str());
		}
		catch(const std::runtime_error& ex)
		{
			TRACE("CDetectNISECDlg::GetChangeDeviceInfo catch exception:%s\n", ex.what());
			return false;
		}

		bSuccess = true;

	} while (false);
	
	return bSuccess;
}

LRESULT CDetectNISECDlg::OnEnumDeviceComplete(WPARAM wParam, LPARAM lParam)
{
	if(m_pUsbEnumerator)
	{
		string strDeviceName = m_pUsbEnumerator->GetDeviceName();
		if(!strDeviceName.empty())
		{
			/// 查询税控盘编号
			TRACE("查询税控盘编号:%s\n", strDeviceName.c_str());
			string strDeviceNumber;
			CInquireNumber inquireNumber;
			if(inquireNumber.GetDeviceNumber(strDeviceName, strDeviceNumber))
			{
				TRACE("税控盘编号:%s\n", strDeviceNumber.c_str());
			}
		}
	}

	return 0;
}
