
// DetectNISECDlg.h : ͷ�ļ�
//

#pragma once


// CDetectNISECDlg �Ի���
class CDetectNISECDlg : public CDialogEx
{
protected:

	/**	
		@brief		Registers an HWND for notification of changes in the device interfaces for the specified interface class GUID
		@param[in]	InterfaceClassGuid	The interface class GUID for the device interfaces
		@param[in]	hWnd				Window handle to receive notifications.
		@param[out]	hDeviceNotify		Receives the device notification handle. On failure, this value is NULL
		@retval		����ִ�гɹ�
		@remark		RegisterDeviceNotification also allows a service handle be used, so a similar wrapper function to this one supporting that scenario could be made from this template.
	*/
	BOOL CDetectNISECDlg::DoRegisterDeviceInterfaceToHwnd(IN GUID InterfaceClassGuid, IN HWND hWnd, OUT HDEVNOTIFY *hDeviceNotify);

protected:
	HDEVNOTIFY m_hDeviceNotify;

// ����
public:
	CDetectNISECDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DETECTNISEC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
