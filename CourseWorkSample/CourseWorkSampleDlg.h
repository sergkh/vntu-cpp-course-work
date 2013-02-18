// CourseWorkSampleDlg.h : header file
//

#pragma once
#include "ProblemSolver.h"

// CCourseWorkSampleDlg dialog
class CCourseWorkSampleDlg : public CDialog
{
// Construction
public:
	CCourseWorkSampleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_COURSEWORKSAMPLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	ProblemSolver solver;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSolve();
	afx_msg void OnExit();
	afx_msg void OnAbout();
};
