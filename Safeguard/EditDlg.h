#if !defined(AFX_EDITDLG_H__965C1C49_CB6D_42D8_8FD6_4FB555B275C4__INCLUDED_)
#define AFX_EDITDLG_H__965C1C49_CB6D_42D8_8FD6_4FB555B275C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// EditDlg dialog

class EditDlg : public CDialog
{
// Construction
public:
	EditDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(EditDlg)
	enum { IDD = IDD_EditDialog };
	CIPAddressCtrl	m_IPyanTo;
	CIPAddressCtrl	m_IPyanFrom;
	CIPAddressCtrl	m_IPTo;
	CIPAddressCtrl	m_IPFrom;
	CComboBox	m_RuleProtocolType;
	CComboBox	m_RuleAction;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(EditDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITDLG_H__965C1C49_CB6D_42D8_8FD6_4FB555B275C4__INCLUDED_)
