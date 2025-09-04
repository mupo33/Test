// CGMFEEDlg.h : header file
//
//{{AFX_INCLUDES()
#include "edensocket.h"
//}}AFX_INCLUDES

#if !defined(AFX_CGMFEEDLG_H__828D87FB_4C7E_428B_8C0A_2F36057E2F1A__INCLUDED_)
#define AFX_CGMFEEDLG_H__828D87FB_4C7E_428B_8C0A_2F36057E2F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCGMFEEDlg dialog

class CCGMFEEDlg : public CDialog
{
// Construction
public:
	CCGMFEEDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCGMFEEDlg)
	enum { IDD = IDD_CGMFEE_DIALOG };
	CListCtrl	m_lcSerialDigitalList;
	CListCtrl	m_lcMemoryLoadList;
	CListCtrl	m_lcCommandList;
	CTabCtrl	m_tabCGMFEE;
	CEdenSocket	m_edenCMD;
	CEdenSocket	m_edenTM;
	//}}AFX_DATA

	int m_nMode;

	int m_nCGMFEExx;
	int m_nLNAMainOrRedundant[16];
	int m_nCoarseAttenuation[16];
	int m_nFineAttenuation[16];
	int m_nCoarsePhase[16];
	int m_nFinePhase[16];

	UINT m_nCMDPort;
	UINT m_nTMPort;

	CString m_sArchivePath;

	int m_nMonitorApid;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCGMFEEDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	
	CStdioFile m_fileArchive;
	UINT m_nArchiveSize;

	UINT m_nTimerFiFo;

	void SetupCommandList();
	void AddCommandList(LPCSTR szCmd);

	void SetupMLList();
	void AddMLList(LPCSTR szChannel, LPCSTR szData, LPCSTR szErrors);

	void SetupSDList();
	void AddSDList(LPCSTR szDatation, LPCSTR szChannel, LPCSTR szData);

	void CreateEdenSocket();

	void GetProfile();
	void WriteProfile();

	void SendTMMonitor();

	void AddSD(LPCSTR szDatation, LPCSTR szChannel, LPCSTR szData);
	void AddML(LPCSTR szChannel, LPCSTR szData, LPCSTR szErrors);

	BOOL OpenArchive();
	BOOL CloseArchive();

	BOOL SetMode(int nMode);

	BOOL SetFineAttenuation(int nLNA, int nValue);
	BOOL SetCoarseAttenuation(int nLNA, int nValue);
	
	BOOL SetFinePhase(int nLNA, int nValue);
	BOOL SetCoarsePhase(int nLNA, int nValue);

	BOOL Operative();
	BOOL Setup();
	void ReadTMFifo();

	// Generated message map functions
	//{{AFX_MSG(CCGMFEEDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnReceiveCmdExecEdenCmd(LPCTSTR szCmd);
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnSelchangeCGMFEE(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnQuit();
	afx_msg void OnLNAMain();
	afx_msg void OnLNARedundant();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnModeIdle();
	afx_msg void OnModeManual();
	afx_msg void OnModeAutomatic();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CGMFEEDLG_H__828D87FB_4C7E_428B_8C0A_2F36057E2F1A__INCLUDED_)
