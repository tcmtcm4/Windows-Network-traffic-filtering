// SafeguardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Safeguard.h"
#include "SafeguardDlg.h"
#include "AddDlg.h"
#include "EditDlg.h"
#include <windows.h>
#include <stdio.h>
//#include <ntddndis.h>
#include "IOCOMMON.h"
#include "ptutils.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include <Winsock2.h>
#pragma comment(lib, "WS2_32.lib")
#define UNICODE
#define _UNICODE

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

vector <PassthruFilter> Passthru;
PassthruFilter vecTemp;
// 用于结构体向量操作的Temp


ULONG IPTemp[4];

ULONG IPTemp_1[4];
ULONG IPTemp_2[4];
ULONG IPTemp_3[4];
ULONG IPTemp_4[4];


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

void CSafeguardDlg::BreakIP(ULONG ip,ULONG ip_temp[4]){	 //用来把ULONG型的IP变成4个数组成的数组  [0].[1].[2].[3] 
	if(ip < 256){
		ip_temp[3] = ip;
		ip_temp[2] = 0;
		ip_temp[1] = 0;
		ip_temp[0] = 0;
	}else if (ip >= 256 && ip < 65536){
		ip_temp[3] = ip % 256;
		ip_temp[2] = ip / 256;
		ip_temp[1] = 0;
		ip_temp[0] = 0;
	}else if (ip >= 65536 && ip < 16777216){
		ip_temp[3] = ip % 256;
		ip_temp[2] = (ip % 65536) / 256;
		ip_temp[1] = ip / 65536;
		ip_temp[0] = 0;
	}else if(ip >= 16777216){
		ip_temp[3] = ip % 256;
		ip_temp[2] = (ip % 65536) / 256;
		ip_temp[1] = (ip % 16777216) / 65536;
		ip_temp[0] = ip / 16777216;
	}
	
}

void CSafeguardDlg::OutTxt(){
	int Size = Passthru.size();
	ofstream SaveList("list.txt");
	int j;
	//输出至文件
	for(int i=0; i < Size; ++i){ 
		for(j = 0; j< 4; ++j){
			BreakIP(Passthru[i].sourceIP,IPTemp);
			if(j != 3){
				SaveList<< IPTemp[j]<<".";
			}else{
				SaveList<< IPTemp[j]<<" ";
			}
		}		
		for(j = 0; j< 4; ++j){
			BreakIP(Passthru[i].sourceMask,IPTemp);
			if(j != 3){
				SaveList<< IPTemp[j]<<".";
			}else{
				SaveList<< IPTemp[j]<<" ";
			}
		}

		SaveList << Passthru[i].sourcePort<<" ";

		for(j = 0; j< 4; ++j){
			BreakIP(Passthru[i].destinationIP,IPTemp);
			if(j != 3){
				SaveList<< IPTemp[j]<<".";
			}else{
				SaveList<< IPTemp[j]<<" ";
			}
		}		
		for(j = 0; j< 4; ++j){
			BreakIP(Passthru[i].destinationMask,IPTemp);
			if(j != 3){
				SaveList<< IPTemp[j]<<".";
			}else{
				SaveList<< IPTemp[j]<<" ";
			}
		}
		SaveList << Passthru[i].destinationPort<<" "<<Passthru[i].protocol<< " " ;//<< Passthru[i].bDrop;

		if(Passthru[i].bDrop == false){  // bDrop为BOOLEAN型 不能直接存入文件...
			SaveList << "0";
		}else{
			SaveList << "1";
		}

		if (i != Size - 1) SaveList <<"\n";
	}
		SaveList.close();
}

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSafeguardDlg dialog

CSafeguardDlg::CSafeguardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSafeguardDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSafeguardDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
	
void CSafeguardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSafeguardDlg)
	DDX_Control(pDX, IDC_LIST1, m_List1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSafeguardDlg, CDialog)
	//{{AFX_MSG_MAP(CSafeguardDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_BN_CLICKED(IDC_Apply, OnApply)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_Exit, OnExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSafeguardDlg message handlers

BOOL CSafeguardDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//初始化列表
	m_List1.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_List1.InsertColumn(0,"源IP地址 ",LVCFMT_LEFT,sizeof("255.255.255.")*8,0);
	m_List1.InsertColumn(1,"源屏蔽位 ",LVCFMT_LEFT,sizeof("255.255.255.")*8,1);
	m_List1.InsertColumn(2,"源端口",LVCFMT_LEFT,sizeof("源端口")*8,2);
	m_List1.InsertColumn(3,"目的IP地址",LVCFMT_LEFT,sizeof("255.255.255.")*8,3);
	m_List1.InsertColumn(4,"目的屏蔽位",LVCFMT_LEFT,sizeof("255.255.255.")*8,4);
	m_List1.InsertColumn(5,"目的端口",LVCFMT_LEFT,sizeof("目的端口")*8,5);
	m_List1.InsertColumn(6,"协议",LVCFMT_LEFT,sizeof("协议")*8,6);
	m_List1.InsertColumn(7,"动作",LVCFMT_LEFT,sizeof("动作")*8,7);
	m_List1.Update;
	
	//读取文件、将文件中的数据存入到创建的struct向量中、显示到列表中
	char buffer[256];
	fstream out;
	out.open("list.txt",ios::in);
	int n = 0;
	char a = '.';
	while(!out.eof()){	
		out.getline(buffer,256,'\n');
		getchar();
		
		string buff(buffer);
		string temp1[8];
		char *temp2[8];
		istringstream stream(buff);
		istringstream stream2(buff);
		m_List1.InsertItem(n,"lalala");
		//输出到主页面的列表中
		for(int i = 0; i < 8; ++i){
			stream >> temp1[i];//分离存储每一组
			temp2[i]=const_cast<char*>(temp1[i].c_str());
			if(i == 7 && temp1[i] == "0"){
				m_List1.SetItemText(n, i, "放行");
			}else if(i == 7 && temp1[i] == "1"){
				m_List1.SetItemText(n, i, "拒绝");
			}else if(i == 6 && temp1[i] == "0"){
				m_List1.SetItemText(n, i, "全部");
			}else if(i == 6 && temp1[i] == "6"){
				m_List1.SetItemText(n, i, "TCP");
			}else if(i == 6 && temp1[i] == "17"){
				m_List1.SetItemText(n, i, "UDP");
			}else if(i == 6 && temp1[i] == "1"){
				m_List1.SetItemText(n, i, "ICMP");
			}else if(i == 2 && temp1[i] == "0"){
				m_List1.SetItemText(n, i, "全部");
			}else if(i == 5 && temp1[i] == "0"){
				m_List1.SetItemText(n, i, "全部");
			}else if(i == 0 && temp1[i] == "0.0.0.0"){
				m_List1.SetItemText(n, i, "全部");
			}else if(i == 3 && temp1[i] == "0.0.0.0"){
				m_List1.SetItemText(n, i, "全部");
			}else{
				m_List1.SetItemText(n, i, temp2[i]);//像界面中列表插入数据
			}
		}


		m_List1.Update;

		//存入vector结构体
		Passthru.push_back(vecTemp);
		stream2 >> IPTemp_1[0] >> a >> IPTemp_1[1] >> a >> IPTemp_1[2] >> a >> IPTemp_1[3]
			>> IPTemp_2[0] >> a >> IPTemp_2[1] >> a >> IPTemp_2[2] >> a >> IPTemp_2[3]
			>> Passthru[n].sourcePort 
			>>IPTemp_3[0] >> a >> IPTemp_3[1] >> a >> IPTemp_3[2] >> a >> IPTemp_3[3] 
			>> IPTemp_4[0] >> a >> IPTemp_4[1] >> a >> IPTemp_4[2] >> a >> IPTemp_4[3] 
			>>Passthru[n].destinationPort
			>>Passthru[n].protocol
			>>Passthru[n].bDrop;

		Passthru[n].sourceIP = IPTemp_1[0] * 16777216 +  IPTemp_1[1] * 65536 + IPTemp_1[2] * 256 + IPTemp_1[3];
		Passthru[n].sourceMask = IPTemp_2[0] * 16777216 +  IPTemp_2[1] * 65536 + IPTemp_2[2] * 256 + IPTemp_2[3];
		Passthru[n].destinationIP = IPTemp_3[0] * 16777216 +  IPTemp_3[1] * 65536 + IPTemp_3[2] * 256 + IPTemp_3[3];
		Passthru[n].destinationMask = IPTemp_4[0] * 16777216 +  IPTemp_4[1] * 65536 + IPTemp_4[2] * 256 + IPTemp_4[3];

		n++;
	}



	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSafeguardDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSafeguardDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSafeguardDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSafeguardDlg::OnAdd() 
{
	AddDlg add_dlg;
	if(add_dlg.DoModal() == IDOK){
//**********************下面的代码用来显示IP列表***********************
			m_List1.DeleteAllItems();
			char buffer[256];
			fstream out;
			out.open("list.txt",ios::in);
			int n = 0;
			while(!out.eof()){	
				out.getline(buffer,256,'\n');
				getchar();
				string buff(buffer);
				string temp1[8];
				char *temp2[8];
				istringstream stream(buff);
				m_List1.InsertItem(n,"lalala");
				//输出到主页面的列表中
				for(int i = 0; i < 8; ++i){
					stream >> temp1[i];//分离存储每一组
					temp2[i]=const_cast<char*>(temp1[i].c_str());
					if(i == 7 && temp1[i] == "0"){
						m_List1.SetItemText(n, i, "放行");
					}else if(i == 7 && temp1[i] == "1"){
						m_List1.SetItemText(n, i, "拒绝");
					}else if(i == 6 && temp1[i] == "0"){
						m_List1.SetItemText(n, i, "全部");
					}else if(i == 6 && temp1[i] == "6"){
						m_List1.SetItemText(n, i, "TCP");
					}else if(i == 6 && temp1[i] == "17"){
						m_List1.SetItemText(n, i, "UDP");
					}else if(i == 6 && temp1[i] == "1"){
						m_List1.SetItemText(n, i, "ICMP");
					}else if(i == 2 && temp1[i] == "0"){
						m_List1.SetItemText(n, i, "全部");
					}else if(i == 5 && temp1[i] == "0"){
						m_List1.SetItemText(n, i, "全部");
					}else if(i == 0 && temp1[i] == "0.0.0.0"){
						m_List1.SetItemText(n, i, "全部");
					}else if(i == 3 && temp1[i] == "0.0.0.0"){
						m_List1.SetItemText(n, i, "全部");
					}else{
						m_List1.SetItemText(n, i, temp2[i]);//像界面中列表插入数据
					}
			}
				m_List1.Update;
				n++;
		}
//**********************上面的代码用来显示IP列表***********************
	}
}

int nItem;//点击获得行号
void CSafeguardDlg::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    nItem = pNMListView->iItem;
	*pResult = 0;
}


void CSafeguardDlg::OnDel() 
{
	//删除
	m_List1.DeleteItem(nItem);
	Passthru.erase(Passthru.begin()+nItem);	
	OutTxt();
}

void CSafeguardDlg::OnEdit() 
{
	//修改

	EditDlg edit_dlg;
	if(edit_dlg.DoModal() == IDOK){
	//**********************下面的代码用来显示IP列表***********************
			m_List1.DeleteAllItems();
			char buffer[256];
			fstream out;
			out.open("list.txt",ios::in);
			int n = 0;
			while(!out.eof()){	
				out.getline(buffer,256,'\n');
				getchar();
				string buff(buffer);
				string temp1[8];
				char *temp2[8];
				istringstream stream(buff);
				m_List1.InsertItem(n,"lalala");
				//输出到主页面的列表中
				for(int i = 0; i < 8; ++i){
					stream >> temp1[i];//分离存储每一组
					temp2[i]=const_cast<char*>(temp1[i].c_str());
					if(i == 7 && temp1[i] == "0"){
						m_List1.SetItemText(n, i, "放行");
					}else if(i == 7 && temp1[i] == "1"){
						m_List1.SetItemText(n, i, "拒绝");
					}else if(i == 6 && temp1[i] == "0"){
						m_List1.SetItemText(n, i, "全部");
					}else if(i == 6 && temp1[i] == "6"){
						m_List1.SetItemText(n, i, "TCP");
					}else if(i == 6 && temp1[i] == "17"){
						m_List1.SetItemText(n, i, "UDP");
					}else if(i == 6 && temp1[i] == "1"){
						m_List1.SetItemText(n, i, "ICMP");
					}else if(i == 2 && temp1[i] == "0"){
						m_List1.SetItemText(n, i, "全部");
					}else if(i == 5 && temp1[i] == "0"){
						m_List1.SetItemText(n, i, "全部");
					}else if(i == 0 && temp1[i] == "0.0.0.0"){
						m_List1.SetItemText(n, i, "全部");
					}else if(i == 3 && temp1[i] == "0.0.0.0"){
						m_List1.SetItemText(n, i, "全部");
					}else{
						m_List1.SetItemText(n, i, temp2[i]);//像界面中列表插入数据
					}
			}
				m_List1.Update;
				n++;
		}
	//**********************上面的代码用来显示IP列表***********************
	}
}

void CSafeguardDlg::OnApply() 
{
	IMClearRules();
	PassthruFilter temp;
	for(unsigned int i=0;i<Passthru.size();i++)
	{
		temp.sourcePort=htons(Passthru[i].sourcePort);
		temp.sourceIP=htonl(Passthru[i].sourceIP);
		temp.sourceMask=htonl(Passthru[i].sourceMask);
		if(Passthru[i].sourceMask==0)
			temp.sourceMask=0xffffffff;
		temp.destinationPort=htons(Passthru[i].destinationPort);
		temp.destinationIP=htonl(Passthru[i].destinationIP);
		temp.destinationMask=htonl(Passthru[i].destinationMask);
		if(Passthru[i].destinationMask==0)
			temp.destinationMask=0xffffffff;
		temp.bDrop=Passthru[i].bDrop;
		temp.protocol = Passthru[i].protocol;
		IMSetRules(&temp,1);
	}
}

void CSafeguardDlg::OnExit()
{
	//退出
	CDialog::OnCancel();
}
