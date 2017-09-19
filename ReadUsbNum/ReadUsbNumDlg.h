
// ReadUsbNumDlg.h : 头文件
//

#pragma once


// CReadUsbNumDlg 对话框
class CReadUsbNumDlg : public CDialogEx
{
public:
	DWORD InquiryDisk(HANDLE hDisk);
// 构造
public:
	CReadUsbNumDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_READUSBNUM_DIALOG };

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
	afx_msg void OnBnClickedInquire();
	afx_msg void OnBnClickedBtnEnum();
};
