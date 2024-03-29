
// CWProjectDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CWProject.h"
#include "CWProjectDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCWProjectDlg 对话框



CCWProjectDlg::CCWProjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CWPROJECT_DIALOG, pParent)
	, m_strRXData(_T(""))
	, m_strTXData(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCWProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM1,m_ctrlComm);
	DDX_Control(pDX, IDC_NUM, m_ComboBox);
	DDX_Text(pDX, IDC_RECIEVE, m_strRXData);
	DDX_Text(pDX, IDC_SEND, m_strTXData);
}

BEGIN_MESSAGE_MAP(CCWProjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN, &CCWProjectDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_SENDBT, &CCWProjectDlg::OnBnClickedSendbt)
END_MESSAGE_MAP()


// CCWProjectDlg 消息处理程序

BOOL CCWProjectDlg::OnInitDialog()
{

	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_ComboBox.SetCurSel(2);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCWProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCWProjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCWProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//串口开关操作
void CCWProjectDlg::OnBnClickedOpen()
{
	//m_Index_int = ((CComboBox*)GetDlgItem(IDC_COMBO_CommSelect))->GetCurSel();//当前选中的
	/***********
	GetCurSel() 函数:用以得到用户选中下拉列表框中数据的索引值.返回的值是重0开始的,如果没有选择任何选项将会返回-1
	************/
	//返回的是打开的端口号
	switch (m_ctrlComm.get_PortOpen())//点击打开或关闭串口按键时，根据当前的串口是否打开经行相应操作
	{
	case 0://当前串口是关闭的，则进行打开串口操作
		m_Index = ((CComboBox*)GetDlgItem(IDC_NUM))->GetCurSel();//当前选中的行
		m_ctrlComm.put_CommPort(m_Index + 1);//如果要打开串口则应先选择哪个串口
		m_ctrlComm.put_PortOpen(TRUE);//打开串口
		UpdateData(FALSE);//更新按键状态

		if (m_ctrlComm.get_PortOpen())//如果已经打开串口，
		{
			SetDlgItemText(IDC_OPEN, _T("关闭串口"));//更改按键提示
			m_ctrlComm.put_Settings(_T("9600,n,8,1"));//打开软件时端口设置默认波特率9600，无校验位，8位数据，1位停止
			m_ctrlComm.put_InputMode(1);//1:表示以二进制方式捡取数据；
										//0：表示以文本方式捡取数据
			m_ctrlComm.put_RThreshold(1);//参数1 表示每当串口接收缓冲区中有多余或等于一个字符时将引发一个接收数据的OnComm事件
										//参数0 表示数据传输事件不会引发OnComm事件，即不响应。
			m_ctrlComm.put_InputLen(0);//0: 缺省值。表示使MSComm控件读取接收缓冲区中的所有内容。
			m_ctrlComm.get_Input();//先预读缓冲区以清除残留数据
			UpdateData(FALSE);
		}
		else
			AfxMessageBox(_T("串口打开失败"));

		break;
	case 1:
		//当前串口是打开的则进行关串口操作
//		m_ctrlComm.SetCommPort(m_Index_int + 1);//如果要打开串口则应先选择哪个串口
		m_ctrlComm.put_PortOpen(FALSE);
		if (!m_ctrlComm.get_PortOpen())//如果已经关闭串口，
		{
			SetDlgItemText(IDC_OPEN, _T("打开串口"));
			UpdateData(FALSE);
		}
		else
			AfxMessageBox(_T("串口关闭失败"));

		break;
	default:
		AfxMessageBox(_T("cannot open Serial Port"));
		break;
	}
	m_ComboBox.SetCurSel(m_Index);//打开软件时串口选择框默认显示COM1  子选项编号的排序是从0开始的。
//	m_BaudRate_M.SetCurSel(m_BaudRate);//打开软件时波特率选择框默认显示9600
//	m_Data_Select_M.SetCurSel(m_Data_Select);//打开软件时数据位选择框默认显示8
//	m_StopBit_M.SetCurSel(m_StopBit);//打开软件时停止位选择框默认显示N  无停止位

}
BEGIN_EVENTSINK_MAP(CCWProjectDlg, CDialogEx)
	ON_EVENT(CCWProjectDlg, IDC_MSCOMM1, 1, CCWProjectDlg::OnCommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()

//接收数据操作
void CCWProjectDlg::OnCommMscomm1()
{
	// TODO: 在此处添加消息处理程序代码
	VARIANT variant_inp;
	COleSafeArray safearry_inp;
	LONG len, k;
	BYTE rxdata[2048];
	CString strtemp;
	int order;
	if (m_ctrlComm.get_CommEvent() == 2)//事件值为2表示接收缓冲区内有数据
	{
		//以下根据自己的通讯协议添加处理代码
		variant_inp = m_ctrlComm.get_Input();//读缓冲区
		safearry_inp = variant_inp;//VARIANT转化为COleSafeArray
		len = safearry_inp.GetOneDimSize();//字符长度
		for (k = 0; k < len; k++)
		{
			safearry_inp.GetElement(&k, rxdata + k);//转化为BYTE型数组
		}
		for (k = 0; k < len; k++)//将数组转化成Cstring型变量
		{
			BYTE bt = *(char*)(rxdata + k);
			//if(m_ctrlHexSend.GetCheck())
			//	strtemp.Format("%02x",bt);
			//else
			strtemp.Format(_T("%c"), bt);//将字符送入临时变量strtemp中存放  
			m_strRXData += strtemp;//加入接收编辑框对应字符串
			/*******************
			以上的语句可以进行对sbuf的读取。
			***********************/
			order = _ttoi(strtemp);//order是字符转化后的int值
		}
		UpdateData(FALSE);//更新编辑框内容（主要是接收编辑框中的）

	}
	m_ComboBox.SetCurSel(m_Index);//打开软件时串口选择框默认显示COM1  子选项编号的排序是从0开始的。
//	m_BaudRate_M.SetCurSel(m_BaudRate);//打开软件时波特率选择框默认显示9600
//	m_Data_Select_M.SetCurSel(m_Data_Select);//打开软件时数据位选择框默认显示8
//	m_StopBit_M.SetCurSel(m_StopBit);//打开软件时停止位选择框默认显示N  无停止位
//	m_ParityCheck_M.SetCurSel(m_ParityCheck);// 奇偶校验

}

//发送数据操作
void CCWProjectDlg::OnBnClickedSendbt()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	long len;
	CByteArray array;
	len = m_strTXData.GetLength();//发送数据的长度

	array.RemoveAll();
	array.SetSize(len);
	for (int i = 0; i < len; i++)
		array.SetAt(i, m_strTXData[i]);
	m_ctrlComm.put_Output(COleVariant(array)); // 发送数据

}

