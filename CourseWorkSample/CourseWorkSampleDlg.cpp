// CourseWorkSampleDlg.cpp : implementation file
//
#include "stdafx.h"
#include "CourseWorkSample.h"
#include "CourseWorkSampleDlg.h"
#include "ProblemSolver.h"
#include "DCGraphicsBridge.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAbout();
	afx_msg void OnSolve();
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


// CCourseWorkSampleDlg dialog


CCourseWorkSampleDlg::CCourseWorkSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCourseWorkSampleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCourseWorkSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCourseWorkSampleDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_CMD_SOLVE, &CCourseWorkSampleDlg::OnSolve)
	ON_COMMAND(ID_CMD_EXIT,  &CCourseWorkSampleDlg::OnExit)
	ON_COMMAND(ID_CMD_ABOUT, &CCourseWorkSampleDlg::OnAbout)
END_MESSAGE_MAP()


// CCourseWorkSampleDlg message handlers

BOOL CCourseWorkSampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCourseWorkSampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// Викликається кожен раз, коли вікно необхідно перемалювати, наприклад 
// при зміні розмірів вікна, звертанні, зміни області перекриття вікон.
// Все малювання має відбуватись в рамках цього методу, інакше при будь-якій
// з вищеописаних операцій графіка буде затерта.
void CCourseWorkSampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CPaintDC dc(this); // device context for painting

		// Відобразити фігури (якщо задачу вже вирішено)
		solver.draw(CDCGraphicsBridge(dc));

		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCourseWorkSampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCourseWorkSampleDlg::OnSolve()
{
	solver.readData(_T("test.txt"));
	solver.solve();
	
	SetDlgItemTextW(IDC_STATIC_RES, solver.getResultText());

	// перемалювати вікно
	Invalidate();
}

void CCourseWorkSampleDlg::OnExit()
{
	// same as double-clicking on main window close box
    ASSERT(AfxGetApp()->m_pMainWnd != NULL);
    AfxGetApp()->m_pMainWnd->SendMessage(WM_CLOSE);
}

void CCourseWorkSampleDlg::OnAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
