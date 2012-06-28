// EditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "safeguard.h"
#include "EditDlg.h"
#include "SafeguardDlg.h"
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EditDlg dialog
extern vector <PassthruFilter> Passthru;
extern ULONG IPTemp[4];
extern int nItem;//被选取的项目的行号

EditDlg::EditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(EditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(EditDlg)
	//}}AFX_DATA_INIT
}


void EditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EditDlg)
	DDX_Control(pDX, IDC_IPyanto, m_IPyanTo);
	DDX_Control(pDX, IDC_IPyanfrom, m_IPyanFrom);
	DDX_Control(pDX, IDC_IPto, m_IPTo);
	DDX_Control(pDX, IDC_IPfrom, m_IPFrom);
	DDX_Control(pDX, IDC_COMBOtype, m_RuleProtocolType);
	DDX_Control(pDX, IDC_COMBOact, m_RuleAction);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(EditDlg, CDialog)
	//{{AFX_MSG_MAP(EditDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EditDlg message handlers


void EditDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL EditDlg::OnInitDialog() 
{	
	CDialog::OnInitDialog();
	
	CSafeguardDlg dlg;
	//初始化实施动作
	m_RuleAction.SetItemData(m_RuleAction.AddString("放行"), 0);
	m_RuleAction.SetItemData(m_RuleAction.AddString("拒绝"), 1);
	if(Passthru[nItem].bDrop == false){
		m_RuleAction.SetCurSel(0);
	}else{
		m_RuleAction.SetCurSel(1);
	}
	//初始化协议类型
	m_RuleProtocolType.SetItemData(m_RuleProtocolType.AddString("全部"), 0);
	m_RuleProtocolType.SetItemData(m_RuleProtocolType.AddString("TCP"), 1);
	m_RuleProtocolType.SetItemData(m_RuleProtocolType.AddString("UDP"), 2);
	m_RuleProtocolType.SetItemData(m_RuleProtocolType.AddString("ICMP"), 3);
	if(Passthru[nItem].protocol == 0){
		m_RuleProtocolType.SetCurSel(0);		//全部 = 0
	}else if(Passthru[nItem].protocol == 6){
		m_RuleProtocolType.SetCurSel(1);		//TCP = 6
	}else if(Passthru[nItem].protocol == 17){
		m_RuleProtocolType.SetCurSel(2);		//UDP = 17
	}else if(Passthru[nItem].protocol == 1){
		m_RuleProtocolType.SetCurSel(3);		//ICMP = 1 
	}	
	//初始化两个Port（编辑框）
	if(Passthru[nItem].sourcePort!=0){
	SetDlgItemInt(IDC_EDIT_FromPort,Passthru[nItem].sourcePort,true);
	}
	if(Passthru[nItem].destinationPort!=0){
	SetDlgItemInt(IDC_EDIT_ToPort,Passthru[nItem].destinationPort,true);
	}//初始化IP框
	byte Btemp[4];
	int i;
	if(Passthru[nItem].sourceIP == 0){
	}else{
		dlg.BreakIP(Passthru[nItem].sourceIP,IPTemp);
		for ( i = 0; i< 4; i++)
			Btemp[i] = (byte)IPTemp[i];
		m_IPFrom.SetAddress(Btemp[0],Btemp[1],Btemp[2],Btemp[3]);
	}

	dlg.BreakIP(Passthru[nItem].sourceMask,IPTemp);
	for ( i = 0; i< 4; i++)
		Btemp[i] = (byte)IPTemp[i];
	m_IPyanFrom.SetAddress(Btemp[0],Btemp[1],Btemp[2],Btemp[3]);
	
	if(Passthru[nItem].destinationIP == 0 ){
	}else{
		dlg.BreakIP(Passthru[nItem].destinationIP,IPTemp);
		for ( i = 0; i< 4; i++)
			Btemp[i] = (byte)IPTemp[i];
		m_IPTo.SetAddress(Btemp[0],Btemp[1],Btemp[2],Btemp[3]);
	}

	dlg.BreakIP(Passthru[nItem].destinationMask,IPTemp);
	for ( i = 0; i< 4; i++)
		Btemp[i] = (byte)IPTemp[i];
	m_IPyanTo.SetAddress(Btemp[0],Btemp[1],Btemp[2],Btemp[3]);



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void EditDlg::OnOK() 
{
	Passthru[nItem].bDrop = m_RuleAction.GetCurSel();
	Passthru[nItem].protocol = m_RuleProtocolType.GetCurSel();
	if(m_RuleProtocolType.GetCurSel() == 0){
		Passthru[nItem].protocol = 0;	//全部
	}else if(m_RuleProtocolType.GetCurSel() == 1){
		Passthru[nItem].protocol = 6;	//TCP
	}else if(m_RuleProtocolType.GetCurSel() == 2){
		Passthru[nItem].protocol = 17;	//UDP
	}else if(m_RuleProtocolType.GetCurSel() == 3){
		Passthru[nItem].protocol = 1;	//ICMP
	}


	Passthru[nItem].sourcePort = GetDlgItemInt(IDC_EDIT_FromPort,NULL,true);
	Passthru[nItem].destinationPort = GetDlgItemInt(IDC_EDIT_ToPort,NULL,true);


	//获取IP  先存到byte[4] 再存到int[4] 最后加起来存到结构体的ULONG中
	byte a[4];
	m_IPFrom.GetAddress(a[0],a[1],a[2],a[3]);
	for(int i = 0; i < 4; i++)
		IPTemp[i] = (int)a[i];
	Passthru[nItem].sourceIP = IPTemp[0] * pow(2,24) +  IPTemp[1] * pow(2,16) + IPTemp[2] * pow(2,8) + IPTemp[3];

	m_IPyanFrom.GetAddress(a[0],a[1],a[2],a[3]);
	for(i = 0; i < 4; i++)
		IPTemp[i] = (int)a[i];
	Passthru[nItem].sourceMask = IPTemp[0] * pow(2,24) +  IPTemp[1] * pow(2,16) + IPTemp[2] * pow(2,8) + IPTemp[3];

	m_IPTo.GetAddress(a[0],a[1],a[2],a[3]);
	for(i = 0; i < 4; i++)
		IPTemp[i] = (int)a[i];
	Passthru[nItem].destinationIP = IPTemp[0] * pow(2,24) +  IPTemp[1] * pow(2,16) + IPTemp[2] * pow(2,8) + IPTemp[3];

	m_IPyanTo.GetAddress(a[0],a[1],a[2],a[3]);
	for(i = 0; i < 4; i++)
		IPTemp[i] = (int)a[i];
	Passthru[nItem].destinationMask = IPTemp[0] * pow(2,24) +  IPTemp[1] * pow(2,16) + IPTemp[2] * pow(2,8) + IPTemp[3];

	CSafeguardDlg dlg;
	dlg.OutTxt();

	CDialog::OnOK();
}
