#pragma once


// Диалоговое окно Dialog

class Dialog : public CDialog
{
	DECLARE_DYNAMIC(Dialog)

public:
	Dialog(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~Dialog();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	int m_modMethod;
	double m_p0;
	double m_p1;
	int m_size;
	int m_mode;
	int m_pValueSize;
	afx_msg void OnBnClickedOk();
};
