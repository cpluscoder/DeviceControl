
// DetectNISECDlg.h : 头文件
//

#pragma once


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
	BOOL CDetectNISECDlg::DoRegisterDeviceInterfaceToHwnd(IN GUID InterfaceClassGuid, IN HWND hWnd, OUT HDEVNOTIFY *hDeviceNotify);

protected:
	HDEVNOTIFY m_hDeviceNotify;

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
	afx_msg void OnBnClickedBtnTest();
	afx_msg void OnClose();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
