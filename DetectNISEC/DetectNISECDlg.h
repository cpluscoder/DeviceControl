
// DetectNISECDlg.h : 头文件
//

#pragma once

#define SHOW_NOTIFY_ICON	0

#include "UsbEnumerator.h"

// CDetectNISECDlg 对话框
class CDetectNISECDlg : public CDialogEx
{
protected:

	/**	
		@brief		Registers an HWND for notification of changes in the device interfaces for the specified interface class GUID
		@param[in]	InterfaceClassGuid	The interface class GUID for the device interfaces
		@param[in]	hWnd				Window handle to receive notifications.
		@param[out]	hDeviceNotify		Receives the device notification handle. On failure, this value is NULL
		@retval		函数执行成功
		@remark		RegisterDeviceNotification also allows a service handle be used, so a similar wrapper function to this one supporting that scenario could be made from this template.
	*/
	bool DoRegisterDeviceInterfaceToHwnd(IN GUID InterfaceClassGuid, IN HWND hWnd, OUT HDEVNOTIFY *hDeviceNotify);

	void OnDeviceArrival(LPARAM lParam);
	void OnDeviceRemoveComplete(LPARAM lParam);

	bool GetChangeDeviceInfo(LPARAM lParam, std::string &strGuid, std::string &strDeviceName);

protected:
	HDEVNOTIFY m_hDeviceNotify;

	CUsbEnumerator::Pointer m_pUsbEnumerator;

#if	SHOW_NOTIFY_ICON
	void PopupMenu(UINT nFlags, int x, int y);
	bool ShowBalloonTip(LPCTSTR szTitle, LPCTSTR szMsg, UINT uTimeout = 3000, DWORD dwInfoFlags = NIIF_INFO);

	CMenu m_popupMenu;/// 系统托盘弹出菜单
	NOTIFYICONDATA m_NotifyIconData;/// 系统托盘控制
#endif

// 构造
public:
	CDetectNISECDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DETECTNISEC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
#if SHOW_NOTIFY_ICON
	afx_msg LRESULT OnIconNotification(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnShowTip();
#endif
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnEnumDeviceComplete(WPARAM wParam, LPARAM lParam);
};
