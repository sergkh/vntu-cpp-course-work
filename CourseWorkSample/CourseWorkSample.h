// CourseWorkSample.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CCourseWorkSampleApp:
// See CourseWorkSample.cpp for the implementation of this class
//

class CCourseWorkSampleApp : public CWinApp
{
public:
	CCourseWorkSampleApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CCourseWorkSampleApp theApp;