
// GHLPokeMachineDlg.cpp : fichier d'implémentation
//

#include "pch.h"
#include "framework.h"
#include "GHLPokeMachine.h"
#include "GHLPokeMachineDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// boîte de dialogue CAboutDlg utilisée pour la boîte de dialogue 'À propos de' pour votre application

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Données de boîte de dialogue
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

// Implémentation
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


// boîte de dialogue de CGHLPokeMachineDlg



CGHLPokeMachineDlg::CGHLPokeMachineDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GHLPOKEMACHINE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGHLPokeMachineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGHLPokeMachineDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CGHLPokeMachineDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDSTART, &CGHLPokeMachineDlg::OnBnClickedStart)
END_MESSAGE_MAP()


// gestionnaires de messages de CGHLPokeMachineDlg

BOOL CGHLPokeMachineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Ajouter l'élément de menu "À propos de..." au menu Système.

	// IDM_ABOUTBOX doit se trouver dans la plage des commandes système.
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

	// Définir l'icône de cette boîte de dialogue.  L'infrastructure effectue cela automatiquement
	//  lorsque la fenêtre principale de l'application n'est pas une boîte de dialogue
	SetIcon(m_hIcon, TRUE);			// Définir une grande icône
	SetIcon(m_hIcon, FALSE);		// Définir une petite icône

	
									
/*
	// TODO: Modifiier pour minimization
	// Declare NOTIFYICONDATA details. 
	// 
	// Error handling is omitted here for brevity. Do not omit it in your code.

	NOTIFYICONDATA nid = {};
	nid.cbSize = sizeof(nid);
	nid.hWnd = hWnd;
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_GUID;

	// Note: This is an example GUID only and should not be used.
	// Normally, you should use a GUID-generating tool to provide the value to
	// assign to guidItem.
	static const GUID myGUID =
	{ 0x23977b55, 0x10e0, 0x4041, {0xb8, 0x62, 0xb1, 0x95, 0x41, 0x96, 0x36, 0x69} };
	nid.guidItem = myGUID;

	// This text will be shown as the icon's tooltip.
	StringCchCopy(nid.szTip, ARRAYSIZE(nid.szTip), L"Test application");

	// Load the icon for high DPI.
	LoadIconMetric(hInst, MAKEINTRESOURCE(IDI_SMALL), LIM_SMALL, &(nid.hIcon));

	// Show the notification.
	Shell_NotifyIcon(NIM_ADD, &nid) ? S_OK : E_FAIL;
	*/

	return TRUE;  // retourne TRUE, sauf si vous avez défini le focus sur un contrôle
}

/* Lorsqu'on appuis sur qqch?*/
void CGHLPokeMachineDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ((nID & 0xFFF0) == 0xF120)
	{
		/*Maximise*/
		CDialogEx::OnSysCommand(nID, lParam);
	}
	else if ((nID & 0xFFF0) == 0xF020)
	{
		/*Minimize*/
		CDialogEx::OnSysCommand(nID, lParam);
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// Si vous ajoutez un bouton Réduire à votre boîte de dialogue, vous devez utiliser le code ci-dessous
//  pour dessiner l'icône.  Pour les applications MFC utilisant le modèle Document/Vue,
//  cela est fait automatiquement par l'infrastructure.

void CGHLPokeMachineDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // contexte de périphérique pour la peinture

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Centrer l'icône dans le rectangle client
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Dessiner l'icône
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		/* Passer de minimize a grand*/
		CDialogEx::OnPaint();
	}
}

// Le système appelle cette fonction pour obtenir le curseur à afficher lorsque l'utilisateur fait glisser
//  la fenêtre réduite.
HCURSOR CGHLPokeMachineDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL FailureDeviceNotFound;
PDWORD   dwThreadIdArray;
PHANDLE  hThreadArray;
PDEVICE_DATA pDeviceData;


void CGHLPokeMachineDlg::OnBnClickedStart()
{
	StartGHPoke(&FailureDeviceNotFound, &dwThreadIdArray, &hThreadArray, &pDeviceData);
	
	/* Desactiver le bouton Start*/
	CWnd* StartBtn = GetDlgItem(IDSTART);
	if (StartBtn) {
		StartBtn->EnableWindow(FALSE);
	}
}


void CGHLPokeMachineDlg::OnBnClickedCancel()
{
	StopGHPoke(&dwThreadIdArray, &hThreadArray, &pDeviceData);

	CDialogEx::OnCancel();
}