
// DetectNISECDlg.cpp : ʵ���ļ�
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



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CDetectNISECDlg �Ի���




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


// CDetectNISECDlg ��Ϣ�������

BOOL CDetectNISECDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	const GUID guidDEVINTERFACE_USB_DEVICE = { 0xA5DCBF10L, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } };
	if(!DoRegisterDeviceInterfaceToHwnd(guidDEVINTERFACE_USB_DEVICE, GetSafeHwnd(), &m_hDeviceNotify))
	{
		OutputDebugString(TEXT("DoRegisterDeviceInterfaceToHwnd failed"));
		return FALSE;
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDetectNISECDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDetectNISECDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDetectNISECDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(!UnregisterDeviceNotification(m_hDeviceNotify))
	{
		OutputDebugString(TEXT("UnregisterDeviceNotification failed"));
	}

	CDialogEx::OnClose();
}


LRESULT CDetectNISECDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CUsbEnumerator::Pointer pUsbEnumerator = CUsbEnumerator::Create();
	pUsbEnumerator->Start();
}
