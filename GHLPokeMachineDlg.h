
// GHLPokeMachineDlg.h : fichier d'en-tête
//

#pragma once


// boîte de dialogue de CGHLPokeMachineDlg
class CGHLPokeMachineDlg : public CDialogEx
{
// Construction
public:
	CGHLPokeMachineDlg(CWnd* pParent = nullptr);	// constructeur standard

// Données de boîte de dialogue
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GHLPOKEMACHINE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// Prise en charge de DDX/DDV


// Implémentation
protected:
	HICON m_hIcon;

	// Fonctions générées de la table des messages
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedStart();
};
