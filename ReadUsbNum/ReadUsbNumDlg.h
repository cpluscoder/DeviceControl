
// ReadUsbNumDlg.h : ͷ�ļ�
//

#pragma once


// CReadUsbNumDlg �Ի���
class CReadUsbNumDlg : public CDialogEx
{
public:
	DWORD InquiryDisk(HANDLE hDisk);
// ����
public:
	CReadUsbNumDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_READUSBNUM_DIALOG };

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
	afx_msg void OnBnClickedInquire();
	afx_msg void OnBnClickedBtnEnum();
};
