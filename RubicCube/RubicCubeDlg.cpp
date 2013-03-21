/*
* =====================================================================
*
* FileName:    RubicCubeDlg.cpp
* Author:      Buck
* Email:       Buck88@gmail.com
* Created:     2013.3.21
* Environment: Visual Studio 2008
*
* Description: Dialog实现类
*
* ======================================================================
*/

#include "stdafx.h"
#include "RubicCube.h"
#include "RubicCubeDlg.h"

#include "Cube3D.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CRubicCubeDlg 对话框




CRubicCubeDlg::CRubicCubeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRubicCubeDlg::IDD, pParent)
	, m_strFormula(_T(""))
	, m_iExecuteNum(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRubicCubeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_RANK, m_comboRank);
	DDX_Text(pDX, IDC_EDIT_FORMULA, m_strFormula);
	DDX_Text(pDX, IDC_EDIT_NUM, m_iExecuteNum);
}

BEGIN_MESSAGE_MAP(CRubicCubeDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CRubicCubeDlg::OnBnClickedButtonReset)
	ON_CBN_SELCHANGE(IDC_COMBO_RANK, &CRubicCubeDlg::OnCbnSelchangeComboRank)
	ON_BN_CLICKED(IDC_BUTTON_UPSET, &CRubicCubeDlg::OnBnClickedButtonUpset)
	ON_BN_CLICKED(IDC_BUTTON_EXECUTE, &CRubicCubeDlg::OnBnClickedButtonExecute)
END_MESSAGE_MAP()


// CRubicCubeDlg 消息处理程序

BOOL CRubicCubeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	int i;
	CString strText;
	for(i=0; i<RANK_MAX-1; ++i)
	{
		strText.Format(L"%d阶", i+2);
		m_comboRank.AddString(strText);
	}
	m_comboRank.SetCurSel(1);

	m_pCube3D = new CCube3D;
	m_pCube3D->Init(m_hWnd, 640, 480);

	SetTimer(1, 10, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRubicCubeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRubicCubeDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRubicCubeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CRubicCubeDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialog::PreTranslateMessage(pMsg);
}

void CRubicCubeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	static int iTime = GetTickCount();
	int iTimeNow = GetTickCount();
	m_pCube3D->Tick(iTimeNow-iTime);
	iTime = iTimeNow;

	m_pCube3D->Render();

	CDialog::OnTimer(nIDEvent);
}

void CRubicCubeDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_pCube3D->KeyDown(nChar);

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CRubicCubeDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_pCube3D->OnLButtonDown(nFlags, point);

	CDialog::OnLButtonDown(nFlags, point);
}

void CRubicCubeDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_pCube3D->OnLButtonUp(nFlags, point);

	CDialog::OnLButtonUp(nFlags, point);
}

void CRubicCubeDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_pCube3D->OnMouseMove(nFlags, point);

	CDialog::OnMouseMove(nFlags, point);
}

BOOL CRubicCubeDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CRubicCubeDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_pCube3D->Release();
	delete m_pCube3D;
}

void CRubicCubeDlg::OnBnClickedButtonReset()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pCube3D->Reset();
}

void CRubicCubeDlg::OnCbnSelchangeComboRank()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pCube3D->ChangeCube(m_comboRank.GetCurSel()+2);
}

void CRubicCubeDlg::OnBnClickedButtonUpset()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pCube3D->GetCube()->Upset();
}

void CRubicCubeDlg::OnBnClickedButtonExecute()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_pCube3D->GetCube()->ExeFormula(m_strFormula, m_iExecuteNum);
}
