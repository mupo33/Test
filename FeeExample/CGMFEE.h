// CGMFEE.h : main header file for the CGMFEE application
//

#if !defined(AFX_CGMFEE_H__40C32427_47CA_4EA3_BF82_13E24B4FFAFD__INCLUDED_)
#define AFX_CGMFEE_H__40C32427_47CA_4EA3_BF82_13E24B4FFAFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCGMFEEApp:
// See CGMFEE.cpp for the implementation of this class
//

class CCGMFEEApp : public CWinApp
{
public:
	CCGMFEEApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCGMFEEApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCGMFEEApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CGMFEE_H__40C32427_47CA_4EA3_BF82_13E24B4FFAFD__INCLUDED_)
