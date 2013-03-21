/*
* =====================================================================
*
* FileName:    RubicCubeDlg.cpp
* Author:      Buck
* Email:       Buck88@gmail.com
* Created:     2013.3.21
* Environment: Visual Studio 2008
*
* Description: Dialogʵ����
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CRubicCubeDlg �Ի���




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


// CRubicCubeDlg ��Ϣ�������

BOOL CRubicCubeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	int i;
	CString strText;
	for(i=0; i<RANK_MAX-1; ++i)
	{
		strText.Format(L"%d��", i+2);
		m_comboRank.AddString(strText);
	}
	m_comboRank.SetCurSel(1);

	m_pCube3D = new CCube3D;
	m_pCube3D->Init(m_hWnd, 640, 480);

	SetTimer(1, 10, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CRubicCubeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CRubicCubeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CRubicCubeDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CDialog::PreTranslateMessage(pMsg);
}

void CRubicCubeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	static int iTime = GetTickCount();
	int iTimeNow = GetTickCount();
	m_pCube3D->Tick(iTimeNow-iTime);
	iTime = iTimeNow;

	m_pCube3D->Render();

	CDialog::OnTimer(nIDEvent);
}

void CRubicCubeDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_pCube3D->KeyDown(nChar);

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CRubicCubeDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_pCube3D->OnLButtonDown(nFlags, point);

	CDialog::OnLButtonDown(nFlags, point);
}

void CRubicCubeDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_pCube3D->OnLButtonUp(nFlags, point);

	CDialog::OnLButtonUp(nFlags, point);
}

void CRubicCubeDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_pCube3D->OnMouseMove(nFlags, point);

	CDialog::OnMouseMove(nFlags, point);
}

BOOL CRubicCubeDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CRubicCubeDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	m_pCube3D->Release();
	delete m_pCube3D;
}

void CRubicCubeDlg::OnBnClickedButtonReset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pCube3D->Reset();
}

void CRubicCubeDlg::OnCbnSelchangeComboRank()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pCube3D->ChangeCube(m_comboRank.GetCurSel()+2);
}

void CRubicCubeDlg::OnBnClickedButtonUpset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pCube3D->GetCube()->Upset();
}

void CRubicCubeDlg::OnBnClickedButtonExecute()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_pCube3D->GetCube()->ExeFormula(m_strFormula, m_iExecuteNum);
}
