// CGMFEEDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CGMFEE.h"
#include "CGMFEEDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MODE_IDLE 0
#define MODE_MANUAL 1
#define MODE_AUTOMATIC 2

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
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
// CCGMFEEDlg dialog

CCGMFEEDlg::CCGMFEEDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCGMFEEDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCGMFEEDlg)
	//}}AFX_DATA_INIT

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nTimerFiFo = 0;

	m_nMode = MODE_IDLE;
	m_nCGMFEExx = 0;
	ZeroMemory(m_nLNAMainOrRedundant, sizeof(m_nLNAMainOrRedundant));
	ZeroMemory(m_nCoarseAttenuation, sizeof(m_nCoarseAttenuation));
	ZeroMemory(m_nFineAttenuation, sizeof(m_nFineAttenuation));
	ZeroMemory(m_nCoarsePhase, sizeof(m_nCoarsePhase));
	ZeroMemory(m_nFinePhase, sizeof(m_nFinePhase));

	GetProfile();
}

void CCGMFEEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCGMFEEDlg)
	DDX_Control(pDX, IDC_SERIAL_DIGITAL_LIST, m_lcSerialDigitalList);
	DDX_Control(pDX, IDC_MEMORY_LOAD_LIST, m_lcMemoryLoadList);
	DDX_Control(pDX, IDC_COMMAND_LIST, m_lcCommandList);
	DDX_Control(pDX, IDC_CGMFEE, m_tabCGMFEE);
	DDX_Control(pDX, IDC_EDEN_CMD, m_edenCMD);
	DDX_Control(pDX, IDC_EDEN_TM, m_edenTM);
	//}}AFX_DATA_MAP

	DDX_Radio(pDX, IDC_MODE_IDLE, m_nMode);

	DDX_Radio(pDX, IDC_LNA_MAIN, m_nLNAMainOrRedundant[m_nCGMFEExx]);
	DDX_Text(pDX, IDC_COARSE_ATTENUATION, m_nCoarseAttenuation[m_nCGMFEExx]);
	DDX_Text(pDX, IDC_FINE_ATTENUATION, m_nFineAttenuation[m_nCGMFEExx]);
	DDX_Text(pDX, IDC_COARSE_PHASE, m_nCoarsePhase[m_nCGMFEExx]);
	DDX_Text(pDX, IDC_FINE_PHASE, m_nFinePhase[m_nCGMFEExx]);
}

void CCGMFEEDlg::CreateEdenSocket()
{
	if(!m_edenCMD.Create(m_nCMDPort))
		AfxMessageBox("Cannot create Eden Socket");

	if(!m_edenTM.Create(m_nTMPort))
		AfxMessageBox("Cannot create Eden Socket");
}

void CCGMFEEDlg::GetProfile()
{
	m_nCMDPort = AfxGetApp()->GetProfileInt("EdenPort", "CMD", 1234);
	m_nTMPort = AfxGetApp()->GetProfileInt("EdenPort", "TM", 5678);

	m_sArchivePath = AfxGetApp()->GetProfileString("Archive", "Path", "");

	m_nMonitorApid = AfxGetApp()->GetProfileInt("Apid", "Monitor", 1234);
}

void CCGMFEEDlg::WriteProfile()
{
	AfxGetApp()->WriteProfileInt("EdenPort", "CMD", m_nCMDPort);
	AfxGetApp()->WriteProfileInt("EdenPort", "TM", m_nTMPort);
	AfxGetApp()->WriteProfileString("Archive", "Path", m_sArchivePath);
	AfxGetApp()->WriteProfileInt("Apid", "Monitor", m_nMonitorApid);
}

void CCGMFEEDlg::SetupCommandList()
{
	m_lcCommandList.InsertColumn(0, "Command", LVCFMT_LEFT, 400);
}

void CCGMFEEDlg::AddCommandList(LPCSTR szCmd)
{
	int nItem;

	nItem = m_lcCommandList.InsertItem(m_lcCommandList.GetItemCount(), szCmd);

	while (m_lcCommandList.GetItemCount() > 500)
		m_lcCommandList.DeleteItem(0);

	m_lcCommandList.EnsureVisible(m_lcCommandList.GetItemCount()-1, FALSE);

}

void CCGMFEEDlg::SetupMLList()
{
	m_lcMemoryLoadList.InsertColumn(0, "Channel", LVCFMT_LEFT, 60);
	m_lcMemoryLoadList.InsertColumn(1, "Data", LVCFMT_LEFT, 60);
	m_lcMemoryLoadList.InsertColumn(2, "Errors", LVCFMT_LEFT, 160);
}

void CCGMFEEDlg::AddMLList(LPCSTR szChannel, LPCSTR szData, LPCSTR szErrors)
{
	int nItem;

//	CString;
	
	nItem = m_lcMemoryLoadList.InsertItem(m_lcMemoryLoadList.GetItemCount(), szChannel);
	m_lcMemoryLoadList.SetItemText(nItem, 1, szData);
	m_lcMemoryLoadList.SetItemText(nItem, 2, szErrors);

	while (m_lcMemoryLoadList.GetItemCount() > 500)
		m_lcMemoryLoadList.DeleteItem(0);

	m_lcMemoryLoadList.EnsureVisible(m_lcMemoryLoadList.GetItemCount()-1, FALSE);

}

void CCGMFEEDlg::SetupSDList()
{
	m_lcSerialDigitalList.InsertColumn(0, "Datation", LVCFMT_RIGHT, 60);
	m_lcSerialDigitalList.InsertColumn(1, "Channel", LVCFMT_LEFT, 60);
	m_lcSerialDigitalList.InsertColumn(2, "Data", LVCFMT_LEFT, 60);
}

void CCGMFEEDlg::AddSDList(LPCSTR szDatation, LPCSTR szChannel, LPCSTR szData)
{
	int nItem;

	nItem = m_lcSerialDigitalList.InsertItem(m_lcSerialDigitalList.GetItemCount(), szDatation);
	m_lcSerialDigitalList.SetItemText(nItem, 1, szChannel);
	m_lcSerialDigitalList.SetItemText(nItem, 2, szData);

	while (m_lcSerialDigitalList.GetItemCount() > 500)
		m_lcSerialDigitalList.DeleteItem(0);

	m_lcSerialDigitalList.EnsureVisible(m_lcSerialDigitalList.GetItemCount()-1, FALSE);

}

BOOL CCGMFEEDlg::OpenArchive()
{
	CloseArchive();

	CTime time = CTime::GetCurrentTime();
	CString sArchiveName;
	
	sArchiveName = time.Format("CGMFEE_%Y-%m-%d_%H-%M-%S.TXT");

	sArchiveName = ConcatPath(m_sArchivePath, sArchiveName);

	if (!m_fileArchive.Open(sArchiveName, CFile::modeWrite | CFile::modeCreate | CFile::typeText))
	{
//		CString sMsg;
//		sMsg.Format("Failed %s archive file open", (LPCSTR)m_sArchiveName);
//		AddLogbook(sMsg);
		return FALSE;
	}

	m_fileArchive.WriteString("Datation,Type,Channel,Data,Errors\r\n");

	return TRUE;
}

BOOL CCGMFEEDlg::CloseArchive()
{
	if (m_fileArchive.m_hFile == CFile::hFileNull)
		return TRUE;

	m_fileArchive.Close();

	return TRUE;
}

void CCGMFEEDlg::AddSD(LPCSTR szDatation, LPCSTR szChannel, LPCSTR szData)
{
	AddSDList(szDatation, szChannel, szData);

	if (m_fileArchive.m_hFile == CFile::hFileNull)
		return;

	CString s;
	s.Format("%s,SD,%s,0x%s\r\n", szDatation, szChannel, szData);

	m_fileArchive.WriteString(s);
}

void CCGMFEEDlg::AddML(LPCSTR szChannel, LPCSTR szData, LPCSTR szErrors)
{
	AddMLList(szChannel, szData, szErrors);

	if (m_fileArchive.m_hFile == CFile::hFileNull)
		return;

	CString s;
	s.Format("0,ML,%s,0x%s,%s\r\n", szChannel, szData, szErrors);
	
	m_fileArchive.WriteString(s);
}

BOOL CCGMFEEDlg::SetMode(int nMode)
{
	switch (nMode)
	{
		case MODE_IDLE:
			Setup();
			break;
		case MODE_MANUAL:
			Operative();
			break;
		case MODE_AUTOMATIC:
			Operative();
			break;
		default:
			return FALSE;
	}

	m_nMode = nMode;

	return TRUE;
}

BOOL CCGMFEEDlg::Operative()
{
	return TRUE;//m_wdmMLA.Operative();
}

BOOL CCGMFEEDlg::Setup()
{
	return TRUE;//m_wdmMLA.Setup();
}

BOOL CCGMFEEDlg::SetFineAttenuation(int nLNA, int nValue)
{
/*
	if (!m_wdmMLA.SetFineAttenuation(nLNA, nValue))
		return FALSE;

	m_nFineAttenuation[nLNA] = nValue;
*/
	return TRUE;
}

BOOL CCGMFEEDlg::SetCoarseAttenuation(int nLNA, int nValue)
{
/*
	if (!m_wdmMLA.SetCoarseAttenuation(nLNA, nValue))
		return FALSE;

	m_nCoarseAttenuation[nLNA] = nValue;
*/

	return TRUE;
}

BOOL CCGMFEEDlg::SetFinePhase(int nLNA, int nValue)
{
/*
	if (!m_wdmMLA.SetFinePhase(nLNA, nValue))
		return FALSE;

	m_nFinePhase[nLNA] = nValue;
*/

	return TRUE;
}

BOOL CCGMFEEDlg::SetCoarsePhase(int nLNA, int nValue)
{
/*
	if (!m_wdmMLA.SetCoarsePhase(nLNA, nValue))
		return FALSE;

	m_nCoarsePhase[nLNA] = nValue;
*/

	return TRUE;
}

void CCGMFEEDlg::ReadTMFifo()
{
	if (m_nMode == MODE_IDLE)
		return;

/*
	DWORD dwWord0;
	DWORD dwWord1;

	while (!m_wdmMLA.IsTMFIFOEmpty())
	{
		dwWord0 = m_wdmMLA.ReadTMFIFO();
		dwWord1 = m_wdmMLA.ReadTMFIFO();

		CString sChannel;
		CString sData;

		BOOL bSD = IS_ON(dwWord0, BIT_24);
		WORD wChannel = (WORD)((dwWord0 << 1) >> 29);
		WORD wData = LOWORD(dwWord0);

		sData = HexToText(wData);
		
		switch (wChannel)
		{
		case 0:
			sChannel = "M_M";
			break;
		case 1:
			sChannel = "R_M";
			break;
		case 2:
			sChannel = "M_R";
			break;
		case 3:
			sChannel = "R_R";
			break;
		}

		if (bSD)
		{
			WORD wDatation = LOWORD(dwWord1);
			CString sDatation;
			
			sDatation.Format("%d", wDatation);
	
			AddSD(sDatation, sChannel, sData);
		}
		else
		{
			CString sErrors;
	
			if (IS_ON(dwWord0, BIT_17))
				sErrors += "Parity/";
			if (IS_ON(dwWord0, BIT_18))
				sErrors += "Number Bit out of 16/";
			if (IS_ON(dwWord0, BIT_19))
				sErrors += "Sampling Error/";

			AddML(sChannel, sData, sErrors);
		}
	}
*/
}

void CCGMFEEDlg::SendTMMonitor()
{
	VARIANT vv;

	vv.vt = VT_I1 | VT_BYREF;
	vv.pbVal = (BYTE*)&m_nMode;
	COleVariant var(vv);

	m_edenTM.SendTmScoe(m_nMonitorApid, var, sizeof(m_nMode));
}


BEGIN_MESSAGE_MAP(CCGMFEEDlg, CDialog)
	//{{AFX_MSG_MAP(CCGMFEEDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_CGMFEE, OnSelchangeCGMFEE)
	ON_BN_CLICKED(IDC_QUIT, OnQuit)
	ON_BN_CLICKED(IDC_LNA_MAIN, OnLNAMain)
	ON_BN_CLICKED(IDC_LNA_REDUNDANT, OnLNARedundant)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_MODE_IDLE, OnModeIdle)
	ON_BN_CLICKED(IDC_MODE_MANUAL, OnModeManual)
	ON_BN_CLICKED(IDC_MODE_AUTOMATIC, OnModeAutomatic)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCGMFEEDlg message handlers
BOOL CCGMFEEDlg::OnInitDialog()
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


	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetupCommandList();	
	SetupMLList();	
	SetupSDList();	
	CreateEdenSocket();
	
	m_tabCGMFEE.InsertItem(0, "LNA 0");
	m_tabCGMFEE.InsertItem(1, "LNA 1");
	m_tabCGMFEE.InsertItem(2, "LNA 2");
	m_tabCGMFEE.InsertItem(3, "LNA 3");
	m_tabCGMFEE.InsertItem(4, "LNA 4");
	m_tabCGMFEE.InsertItem(5, "LNA 5");
	m_tabCGMFEE.InsertItem(6, "LNA 6");
	m_tabCGMFEE.InsertItem(7, "LNA 7");
	m_tabCGMFEE.InsertItem(8, "LNA 8");
	m_tabCGMFEE.InsertItem(9, "LNA 9");
	m_tabCGMFEE.InsertItem(10, "LNA 10");
	m_tabCGMFEE.InsertItem(11, "LNA 11");
	m_tabCGMFEE.InsertItem(12, "LNA 12");
	m_tabCGMFEE.InsertItem(13, "LNA 13");
	m_tabCGMFEE.InsertItem(14, "LNA 14");
	m_tabCGMFEE.InsertItem(15, "LNA 15");
	
	m_tabCGMFEE.SetCurSel(m_nCGMFEExx);
/*
	m_wdmMLA.ResetBoard();
	m_wdmMLA.ResetTCFIFO();
	m_wdmMLA.ResetTMFIFO();

	m_wdmMLA.DisableBank(0);
	m_wdmMLA.DisableBank(1);
	m_wdmMLA.DisableBank(2);
	m_wdmMLA.DisableBank(3);
	m_wdmMLA.DisableBank(4);

	m_wdmMLA.EnableBank(5);
	m_wdmMLA.EnableBank(6);
	m_wdmMLA.EnableBank(7);

	m_wdmMLA.EnableSD(0);
	m_wdmMLA.EnableSD(1);
	m_wdmMLA.EnableSD(2);
	m_wdmMLA.EnableSD(3);

	m_wdmMLA.EnableML(0);
	m_wdmMLA.EnableML(1);
	m_wdmMLA.EnableML(2);
	m_wdmMLA.EnableML(3);

//	m_wdmMLA.EnableStartAutoCheck();
//	m_wdmMLA.Operative();
*/
	m_nTimerFiFo = SetTimer(100, 1000, NULL);

	//OnOnReceiveCmdExecEdenCmd("MODE AUTO");


	return TRUE;
}

void CCGMFEEDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCGMFEEDlg::OnDestroy()
{
	if (m_nTimerFiFo != 0)
		KillTimer(m_nTimerFiFo);

	WinHelp(0L, HELP_QUIT);

	CDialog::OnDestroy();

	WriteProfile();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCGMFEEDlg::OnPaint() 
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
HCURSOR CCGMFEEDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BEGIN_EVENTSINK_MAP(CCGMFEEDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CCGMFEEDlg)
	ON_EVENT(CCGMFEEDlg, IDC_EDEN_CMD, 3 /* OnReceiveCmdExec */, OnReceiveCmdExecEdenCmd, VTS_BSTR)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()



void CCGMFEEDlg::OnReceiveCmdExecEdenCmd(LPCTSTR szCmd) 
{
	CString sCmd;
	CString s;
	BOOL bAck = FALSE; 

	AddCommandList(szCmd);
	
	if (!ExtractSeparatorString(szCmd, sCmd, ' ', TRUE))
		return;

	if (sCmd == "ARCHIVE")
	{
		if (!ExtractSeparatorString(szCmd, sCmd, ' ', TRUE))
			return;

		if (sCmd == "ON")
		{
			bAck = OpenArchive();
		}
		else if (sCmd == "OFF")
		{
			bAck = CloseArchive();
		}
	}
	else if (sCmd == "ATT")
	{
		if (m_nMode == MODE_MANUAL)
		{
			int nLNA;
			if (!ExtractSeparatorInt(szCmd, nLNA, ' '))
				return;

			CString sFineCoarse;
			if (!ExtractSeparatorString(szCmd, sFineCoarse, ' ', TRUE))
				return;

			int nValue;
			if (!ExtractSeparatorInt(szCmd, nValue, ' '))
				return;

			if (sFineCoarse == "F")
				bAck = SetFineAttenuation(nLNA, nValue);
			else if (sFineCoarse == "C")
				bAck = SetCoarseAttenuation(nLNA, nValue);
		}
	}
	else if (sCmd == "ERR")
	{
		if (m_nMode == MODE_MANUAL)
		{
			int nLNA;
			if (!ExtractSeparatorInt(szCmd, nLNA, ' '))
				return;

			CString sControllerReceiver;
			if (!ExtractSeparatorString(szCmd, sControllerReceiver, ' ', TRUE))
				return;

/*
			int nBit0;
			if (!ExtractSeparatorInt(szCmd, nBit0, ' '))
				return;
			int nBit1;
			if (!ExtractSeparatorInt(szCmd, nBit1, ' '))
				return;
			int nBit2;
			if (!ExtractSeparatorInt(szCmd, nBit2, ' '))
				return;
			int nBit3;
			if (!ExtractSeparatorInt(szCmd, nBit3, ' '))
				return;
*/
			CString sValue;
			if (!ExtractSeparatorString(szCmd, sValue, ' ', TRUE))
				return;
			
			int nValue = BitStringToWord(sValue);

//			if (sControllerReceiver == "C")
//				bAck = m_wdmMLA.SetControllerStatus(nLNA, nValue);
//			else
//				bAck = m_wdmMLA.SetReceiverStatus(nLNA, nValue);
		}
	}
	else if (sCmd == "MODE")
	{
		if (!ExtractSeparatorString(szCmd, sCmd, ' ', TRUE))
			return;
			
		if (sCmd == "AUTO")
		{
			bAck = SetMode(MODE_AUTOMATIC);
		}
		else if (sCmd == "MAN")
		{
			bAck = SetMode(MODE_MANUAL);
		}
		else if (sCmd == "IDLE")
		{
			bAck = SetMode(MODE_IDLE);
		}
	}
/*
	else if (sCmd == "OPERATIVE")
	{
		if (m_nMode != MODE_IDLE)
		{
			if (!ExtractSeparatorString(szCmd, sCmd, ' ', TRUE))
				return;
				
			if (sCmd == "ON")
			{
				Operative();
				bNack = FALSE;
			}
			else if (sCmd == "OFF")
			{
				Setup();
				bNack = FALSE;
			}
		}
	}
*/
	else if (sCmd == "PHASE")
	{
		if (m_nMode == MODE_MANUAL)
		{
			int nLNA;
			if (!ExtractSeparatorInt(szCmd, nLNA, ' '))
				return;

			CString sFineCoarse;
			if (!ExtractSeparatorString(szCmd, sFineCoarse, ' ', TRUE))
				return;

			int nValue;
			if (!ExtractSeparatorInt(szCmd, nValue, ' '))
				return;

			if (sFineCoarse == "F")
				bAck = SetFinePhase(nLNA, nValue);
			else if (sFineCoarse == "C")
				bAck = SetCoarsePhase(nLNA, nValue);
		}
	}
	else if (sCmd == "POWER")
	{
		if (m_nMode == MODE_MANUAL)
		{
			int nLNA;
			if (!ExtractSeparatorInt(szCmd, nLNA, ' '))
				return;

			CString sMainRedundant;
			if (!ExtractSeparatorString(szCmd, sMainRedundant, ' ', TRUE))
				return;

			if (!ExtractSeparatorString(szCmd, sCmd, ' ', TRUE))
				return;

	
			if (sMainRedundant == "M")
			{
//				if (sCmd == "ON")
//					bAck = m_wdmMLA.SetPowerMain(nLNA, TRUE);
//				else if (sCmd == "OFF")
//					bAck = m_wdmMLA.SetPowerMain(nLNA, FALSE);
			}
			else 
			{
//				if (sCmd == "ON")
//					bAck = m_wdmMLA.SetPowerRedundant(nLNA, TRUE);
//				else if (sCmd == "OFF")
//					bAck = m_wdmMLA.SetPowerRedundant(nLNA, FALSE);
			}
			
		}
	}
	else if (sCmd == "TEMP")
	{
		if (m_nMode == MODE_MANUAL)
		{
			int nLNA;
			if (!ExtractSeparatorInt(szCmd, nLNA, ' '))
				return;

			int nValue;
			if (!ExtractSeparatorInt(szCmd, nValue, ' '))
				return;

//			bAck = m_wdmMLA.SetTemperature(nLNA, nValue);
		}
	}

	UpdateData(FALSE);
	SendTMMonitor();

//	ZeroMemory(m_nCoarseAttenuation, sizeof(m_nCoarseAttenuation));
//	ZeroMemory(m_nFineAttenuation, sizeof(m_nFineAttenuation));
//	ZeroMemory(m_nCoarsePhase, sizeof(m_nCoarsePhase));
//	ZeroMemory(m_nFinePhase, sizeof(m_nFinePhase));

	m_edenCMD.SendCmdAnsw(!bAck, szCmd);	
}

void CCGMFEEDlg::OnCancel() 
{
}

void CCGMFEEDlg::OnOK() 
{
}

void CCGMFEEDlg::OnClose() 
{
	EndDialog(0);
}

void CCGMFEEDlg::OnSelchangeCGMFEE(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);

	m_nCGMFEExx = m_tabCGMFEE.GetCurSel();

	UpdateData(FALSE);

	*pResult = 0;
}

void CCGMFEEDlg::OnQuit() 
{
	EndDialog(0);
}

void CCGMFEEDlg::OnLNAMain() 
{
//	UpdateData(TRUE);
}

void CCGMFEEDlg::OnLNARedundant() 
{
//	UpdateData(TRUE);
}

void CCGMFEEDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == m_nTimerFiFo)
	{
		ReadTMFifo();
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CCGMFEEDlg::OnModeIdle() 
{
	SetMode(MODE_IDLE);
}

void CCGMFEEDlg::OnModeManual() 
{
	SetMode(MODE_MANUAL);
}

void CCGMFEEDlg::OnModeAutomatic() 
{
	SetMode(MODE_AUTOMATIC);
}
