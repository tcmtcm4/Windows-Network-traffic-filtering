#if !defined(AFX_ADDDLG_H__A4A5A0B4_0D2C_44F7_85AE_ABAC80D10828__INCLUDED_)
#define AFX_ADDDLG_H__A4A5A0B4_0D2C_44F7_85AE_ABAC80D10828__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddDlg.h : header file
//

#define UNICODE
#define _UNICODE



/////////////////////////////////////////////////////////////////////////////
// AddDlg dialog

class AddDlg : public CDialog
{
// Construction
public:
	AddDlg(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(AddDlg)
	enum { IDD = IDD_AddDialog };
	CEdit	m_ToPort;
	CEdit	m_FromPort;
	CIPAddressCtrl	m_IPyanTo;
	CIPAddressCtrl	m_IPyanFrom;
	CIPAddressCtrl	m_IPTo;
	CIPAddressCtrl	m_IPFrom;
	CComboBox	m_RuleProtocolType;
	CComboBox	m_RuleAction;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(AddDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(AddDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDDLG_H__A4A5A0B4_0D2C_44F7_85AE_ABAC80D10828__INCLUDED_)
