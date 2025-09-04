// CGMFEE.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CGMFEE.h"
#include "CGMFEEDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCGMFEEApp

BEGIN_MESSAGE_MAP(CCGMFEEApp, CWinApp)
	//{{AFX_MSG_MAP(CCGMFEEApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelpIndex)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCGMFEEApp construction

CCGMFEEApp::CCGMFEEApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCGMFEEApp object

CCGMFEEApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCGMFEEApp initialization

BOOL CCGMFEEApp::InitInstance()
{
	ChangeProfileName();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	CCGMFEEDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
