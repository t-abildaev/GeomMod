// Dialog.cpp: файл реализации
//

#include "pch.h"
#include "GeomMod.h"
#include "Dialog.h"
#include "afxdialogex.h"


// Диалоговое окно Dialog

IMPLEMENT_DYNAMIC(Dialog, CDialog)

Dialog::Dialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG, pParent)
	, m_modMethod(0)
	, m_p0(0)
	, m_p1(0)
	, m_size(0)
	, m_mode(0)
	, m_pValueSize(0)
{

}

Dialog::~Dialog()
{
}

void Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_BERNOULLI, m_modMethod);
	DDX_Text(pDX, IDC_EDITH0, m_p0);
	DDX_Text(pDX, IDC_EDITH1, m_p1);
	DDX_Text(pDX, IDC_EDITSIZE, m_size);
	DDX_Radio(pDX, IDC_MODE, m_mode);
	DDX_Text(pDX, IDC_EDITPVALUESIZE, m_pValueSize);
}


BEGIN_MESSAGE_MAP(Dialog, CDialog)
//	ON_EN_CHANGE(IDC_EDITSIZE, &Dialog::OnChangeEditsize)
//	ON_EN_CHANGE(IDC_EDITH0, &Dialog::OnChangeEdith0)
//	ON_EN_CHANGE(IDC_EDITH1, &Dialog::OnChangeEdith1)
	ON_BN_CLICKED(IDOK, &Dialog::OnBnClickedOk)
END_MESSAGE_MAP()


// Обработчики сообщений Dialog


//void Dialog::OnChangeEditsize()
//{
//	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
//	// send this notification unless you override the CDialog::OnInitDialog()
//	// функция и вызов CRichEditCtrl().SetEventMask()
//	// with the ENM_CHANGE flag ORed into the mask.
//
//	// TODO:  Добавьте код элемента управления
//	UpdateData(1);
//	if (m_size < 0)
//	{
//		AfxMessageBox(L"Размер выборки должен быть натуральным числом");
//	}
//}


//void Dialog::OnChangeEdith0()
//{
//	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
//	// send this notification unless you override the CDialog::OnInitDialog()
//	// функция и вызов CRichEditCtrl().SetEventMask()
//	// with the ENM_CHANGE flag ORed into the mask.
//
//	// TODO:  Добавьте код элемента управления
//	UpdateData(1);
//	if (m_p0 < 0 || m_p0 > 1)
//	{
//		AfxMessageBox(L"Этот параметр должен быть числом в диапазоне от 0 до 1");
//	}
//}


//void Dialog::OnChangeEdith1()
//{
//	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
//	// send this notification unless you override the CDialog::OnInitDialog()
//	// функция и вызов CRichEditCtrl().SetEventMask()
//	// with the ENM_CHANGE flag ORed into the mask.
//
//	// TODO:  Добавьте код элемента управления
//	UpdateData(1);
//	if (m_p1 < 0 || m_p1 > 1)
//	{
//		AfxMessageBox(L"Этот параметр должен быть числом в диапазоне от 0 до 1");
//	}
//}


void Dialog::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(1);
	if (m_p1 < 0 || m_p1 > 1 || m_size < 0)
		AfxMessageBox(L"Введите корректные параметры. Размеры выборок должны быть натуральными числами, параметры p0 и p1 вещественными числами на интервале (0,1)");
	CDialog::OnOK();
}
