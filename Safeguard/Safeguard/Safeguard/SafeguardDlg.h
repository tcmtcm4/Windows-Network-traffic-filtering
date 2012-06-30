// SafeguardDlg.h : header file
//

#define UNICODE
#define _UNICODE

#if !defined(AFX_SAFEGUARDDLG_H__2F348F86_B1AD_493D_B959_8FF17CDE3186__INCLUDED_)
#define AFX_SAFEGUARDDLG_H__2F348F86_B1AD_493D_B959_8FF17CDE3186__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSafeguardDlg dialog

class CSafeguardDlg : public CDialog
{
// Construction
public:
	CSafeguardDlg(CWnd* pParent = NULL);	// standard constructor
	void CSafeguardDlg::OutTxt();	//输出到文件
	void CSafeguardDlg::BreakIP(ULONG ip,ULONG ip_temp[4]); //将ULONG的IP打断成4个ULONG组成的数组;

// Dialog Data
	//{{AFX_DATA(CSafeguardDlg)
	enum { IDD = IDD_SAFEGUARD_DIALOG };
	CListCtrl	m_List1;

	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSafeguardDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSafeguardDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAdd();
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDel();
	afx_msg void OnApply();
	afx_msg void OnEdit();
	afx_msg void OnExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAFEGUARDDLG_H__2F348F86_B1AD_493D_B959_8FF17CDE3186__INCLUDED_)
