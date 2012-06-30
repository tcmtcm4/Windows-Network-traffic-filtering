// AddDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Safeguard.h"
#include "AddDlg.h"
#include "SafeguardDlg.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>

#define UNICODE
#define _UNICODE
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AddDlg dialog


extern vector <PassthruFilter> Passthru;
extern ULONG IPTemp[4];

AddDlg::AddDlg(CWnd* pParent /*=NULL*/)
	: CDialog(AddDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(AddDlg)
	//}}AFX_DATA_INIT
}


void AddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AddDlg)
	DDX_Control(pDX, IDC_IPyanto, m_IPyanTo);
	DDX_Control(pDX, IDC_IPyanfrom, m_IPyanFrom);
	DDX_Control(pDX, IDC_IPto, m_IPTo);
	DDX_Control(pDX, IDC_IPfrom, m_IPFrom);
	DDX_Control(pDX, IDC_COMBOtype, m_RuleProtocolType);
	DDX_Control(pDX, IDC_COMBOact, m_RuleAction);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(AddDlg, CDialog)
	//{{AFX_MSG_MAP(AddDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AddDlg message handlers

BOOL AddDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//初始化实施动作
	m_RuleAction.SetItemData(m_RuleAction.AddString("放行"), 0);
	m_RuleAction.SetItemData(m_RuleAction.AddString("拒绝"), 1);
	m_RuleAction.SetCurSel(0);
	//初始化协议类型
	m_RuleProtocolType.SetItemData(m_RuleProtocolType.AddString("全部"), 0);
	m_RuleProtocolType.SetItemData(m_RuleProtocolType.AddString("TCP"), 1);
	m_RuleProtocolType.SetItemData(m_RuleProtocolType.AddString("UDP"), 2);
	m_RuleProtocolType.SetItemData(m_RuleProtocolType.AddString("ICMP"), 3); // IPPROTO在stdafx中定义
	m_RuleProtocolType.SetCurSel(0);
	//0全部0   1TCP是6   2UDP是17   3ICMP是1  
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void AddDlg::OnOK() 
{
	// TODO: Add extra validation here
	PassthruFilter NewPass;
	NewPass.bDrop = m_RuleAction.GetCurSel();

	if( m_RuleProtocolType.GetCurSel() == 0){
		NewPass.protocol = 0;   //全部 = 0
	}else if( m_RuleProtocolType.GetCurSel() == 1){
		NewPass.protocol = 6;   //TCP = 6		
	}else if( m_RuleProtocolType.GetCurSel() == 2){
		NewPass.protocol = 17;   //UDP = 17
	}else if( m_RuleProtocolType.GetCurSel() == 3){
		NewPass.protocol = 1;   //ICMP = 1
	}

	NewPass.sourcePort = GetDlgItemInt(IDC_EDIT_FromPort,NULL,true);
	NewPass.destinationPort = GetDlgItemInt(IDC_EDIT_ToPort,NULL,true);
	//获取IP (还是存成(int)[4]数组)
	byte a[4];
	m_IPFrom.GetAddress(a[0],a[1],a[2],a[3]);
	for(int i = 0; i < 4; i++)
		IPTemp[i] = (int)a[i];
	NewPass.sourceIP = IPTemp[0] * pow(2,24) +  IPTemp[1] * pow(2,16) + IPTemp[2] * pow(2,8) + IPTemp[3];

	m_IPyanFrom.GetAddress(a[0],a[1],a[2],a[3]);
	for(i = 0; i < 4; i++)
		IPTemp[i] = (int)a[i];
	NewPass.sourceMask = IPTemp[0] * pow(2,24) +  IPTemp[1] * pow(2,16) + IPTemp[2] * pow(2,8) + IPTemp[3];

	m_IPTo.GetAddress(a[0],a[1],a[2],a[3]);
	for(i = 0; i < 4; i++)
		IPTemp[i] = (int)a[i];
	NewPass.destinationIP = IPTemp[0] * pow(2,24) +  IPTemp[1] * pow(2,16) + IPTemp[2] * pow(2,8) + IPTemp[3];

	m_IPyanTo.GetAddress(a[0],a[1],a[2],a[3]);
	for(i = 0; i < 4; i++)
		IPTemp[i] = (int)a[i];
	NewPass.destinationMask = IPTemp[0] * pow(2,24) +  IPTemp[1] * pow(2,16) + IPTemp[2] * pow(2,8) + IPTemp[3];
	
	Passthru.push_back(NewPass);
	
	CSafeguardDlg dlg;
	dlg.OutTxt();
	CDialog::OnOK();
}
