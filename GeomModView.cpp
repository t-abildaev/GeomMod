
// GeomModView.cpp: реализация класса CGeomModView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "GeomMod.h"
#endif

#include "GeomModDoc.h"
#include "GeomModView.h"

#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGeomModView

IMPLEMENT_DYNCREATE(CGeomModView, CView)

BEGIN_MESSAGE_MAP(CGeomModView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// Создание или уничтожение CGeomModView

CGeomModView::CGeomModView() noexcept
{
	// TODO: добавьте код создания

}

CGeomModView::~CGeomModView()
{
}

BOOL CGeomModView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Рисование CGeomModView

void CGeomModView::OnDraw(CDC* pDC)
{
	CGeomModDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	Sample * sample = pDoc->getSample();
	ChiSquare * chiSq = pDoc->getChiSq();
	int numOfTotalStates = chiSq->getNumOfTotalStates();
	int indicator = pDoc->m_mode;

	CRect rc;
	GetClientRect(&rc);

	if (indicator == 0)
	{
		int maxY = chiSq->maximum();
		while (maxY % 5 != 0) ++maxY;
		axisYhist(pDC, rc, maxY, maxY / 5);
		arrowsHist(pDC, rc);
		drawHist(pDC, rc, chiSq, maxY);
		legend(pDC, rc, sample->getProb(), chiSq->getChiSquare(), chiSq->getDegOfFreedom(), chiSq->getPValue());
	}
	else if (indicator == 1)
	{
		axisYpVal(pDC, rc, 1);
		axisXpVal(pDC, rc, 20);
		arrowsPVal(pDC, rc);
		drawPValues(pDC, rc, pDoc->getPValFrequency());
		legendPVal(pDC, rc, pDoc->m_p0, pDoc->m_p1, pDoc->m_pValueSize, pDoc->m_size);
	}
	else if (indicator == 2)
	{
		axisYpVal(pDC, rc, 0);
		arrowsPVal(pDC, rc);
		axisXtime(pDC, rc);
		double temp = max(*(std::max_element(pDoc->getTimeBernoulli(), pDoc->getTimeBernoulli() + 9)), *(std::max_element(pDoc->getTimeInverse(), pDoc->getTimeInverse() + 9)));
		drawTime(pDC, rc, pDoc->getTimeBernoulli(), temp, 54, 204, 255);
		drawTime(pDC, rc, pDoc->getTimeInverse(), temp, /*204*/51, /*204*/51, 255);
		legendTime(pDC, rc);
	}
}

void CGeomModView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CGeomModView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Диагностика CGeomModView

#ifdef _DEBUG
void CGeomModView::AssertValid() const
{
	CView::AssertValid();
}

void CGeomModView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGeomModDoc* CGeomModView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGeomModDoc)));
	return (CGeomModDoc*)m_pDocument;
}

void CGeomModView::arrowsHist(CDC * pDC, CRect rc)
{
	CFont font;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 16;
	strcpy_s((char *) lf.lfFaceName, 16, "Times New Roman");
	font.CreateFontIndirect(&lf);
	CFont * def_font = pDC->SelectObject(&font);
	pDC->MoveTo(0.9 * rc.Width() + 10, 0.9 * rc.Height());
	pDC->LineTo(0.9 * rc.Width() - 10, 0.9 * rc.Height() - 10);
	pDC->MoveTo(0.9 * rc.Width() + 10, 0.9 * rc.Height());
	pDC->LineTo(0.9 * rc.Width() - 10, 0.9 * rc.Height() + 10);
	pDC->MoveTo(0.075 * rc.Width(), 0.1 * rc.Height());
	pDC->LineTo(0.075 * rc.Width() - 10, 0.1 * rc.Height() + 20);
	pDC->MoveTo(0.075 * rc.Width(), 0.1 * rc.Height());
	pDC->LineTo(0.075 * rc.Width() + 10, 0.1 * rc.Height() + 20);
	pDC->SelectObject(def_font);
	font.DeleteObject();
}

void CGeomModView::axisXhist(CDC * pDC, CRect rc, double * Frequency, int numOfTotalStates, int maxY, int indent)
{
	int temp = max(ceil(numOfTotalStates / 10), 1);
	CFont font;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 30;
	strcpy_s((char *) lf.lfFaceName, 16, "Times New Roman");
	font.CreateFontIndirect(&lf);
	CFont* def_font = pDC->SelectObject(&font);
	double h = 0.75 * rc.Height() / maxY;
	double hX = 0.75 * rc.Width() / numOfTotalStates;
	for (int i = 0, k = 0; k < numOfTotalStates; i += hX, ++k)
	{
		CString letter;
		letter.Format(L"%1d", k);
		pDC->MoveTo(0.1 * rc.Width() + hX / 2 + i, 0.9 * rc.Height() + 3);
		pDC->LineTo(0.1 * rc.Width() + hX / 2 + i, 0.9 * rc.Height() - 3);
		if (k % temp == 0)
		{
			if (k < 10) pDC->TextOutW(0.1 * rc.Width() + hX / 2 + i - 7, 0.9 * rc.Height() + 6, letter);
			else pDC->TextOutW(0.1 * rc.Width() + hX / 2 + i - 13, 0.9 * rc.Height() + 6, letter);
		}
		pDC->Rectangle(0.1 * rc.Width() + indent + i, 0.9 * rc.Height() - Frequency[k] * h,
			0.1 * rc.Width() + hX - indent + i, 0.9 * rc.Height() + 1);
	}
	pDC->SelectObject(def_font);
	font.DeleteObject();
}

void CGeomModView::axisYhist(CDC * pDC, CRect rc, int maxY, double step)
{
	CPoint axes[3] = { CPoint(0.075 * rc.Width(), 0.1 * rc.Height()),
		CPoint(0.075 * rc.Width(), 0.9 * rc.Height()),
		CPoint(0.9 * rc.Width() + 10, 0.9 * rc.Height()) };
	pDC->Polyline(axes, 3);

	CFont font;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 30;
	strcpy_s((char *) lf.lfFaceName, 16, "Times New Roman");
	font.CreateFontIndirect(&lf);
	CFont * def_font = pDC->SelectObject(&font);
	int j = maxY / step;
	double hY = 0.75 * rc.Height() / j;
	for (int i = 0, k = 0; k <= j; i += hY, ++k)
	{
		CString letter;
		letter.Format(L"%3d", (int)(k * step));
		pDC->MoveTo(0.075 * rc.Width() - 3, 0.9 * rc.Height() - i);
		pDC->LineTo(0.075 * rc.Width() + 3, 0.9 * rc.Height() - i);
		if (k * step < 10) pDC->TextOutW(0.075 * rc.Width() - 33, 0.9 * rc.Height() - i - 13, letter);
		else if (k * step < 100) pDC->TextOutW(0.075 * rc.Width() - 40, 0.9 * rc.Height() - i - 13, letter);
		else if (k * step < 1000) pDC->TextOutW(0.075 * rc.Width() - 47, 0.9 * rc.Height() - i - 13, letter);
		else if (k * step < 10000) pDC->TextOutW(0.075 * rc.Width() - 59, 0.9 * rc.Height() - i - 13, letter);
	}
	pDC->SelectObject(def_font);
	font.DeleteObject();
}

void CGeomModView::legend(CDC * pDC, CRect rc, double p, double chiSquare, int degOfFreedom, double pValue)
{
	CFont font1;
	LOGFONT lf1;
	memset(&lf1, 0, sizeof(LOGFONT));
	lf1.lfHeight = 26;
	strcpy_s((char *) lf1.lfFaceName, 16, "Times New Roman");
	font1.CreateFontIndirect(&lf1);
	CFont* def_font1 = pDC->SelectObject(&font1);

	CString letter;
	pDC->TextOutW(0.9 * rc.Width() - 350, 0.1 * rc.Height(), L"Вероятность успеха:");
	letter.Format(L"%.2f", p);
	pDC->TextOutW(0.9 * rc.Width(), 0.1 * rc.Height(), letter);

	pDC->TextOutW(0.9 * rc.Width() - 350, 0.1 * rc.Height() + 25, L"Значение статистики:");
	letter.Format(L"%.4f", chiSquare);
	pDC->TextOutW(0.9 * rc.Width(), 0.1 * rc.Height() + 25, letter);

	pDC->TextOutW(0.9 * rc.Width() - 350, 0.1 * rc.Height() + 50, L"Число степеней свободы:");
	letter.Format(L"%d", degOfFreedom);
	pDC->TextOutW(0.9 * rc.Width(), 0.1 * rc.Height() + 50, letter);

	pDC->TextOutW(0.9 * rc.Width() - 350, 0.1 * rc.Height() + 75, L"P-value:");
	letter.Format(L"%.4f", pValue);
	pDC->TextOutW(0.9 * rc.Width(), 0.1 * rc.Height() + 75, letter);

	pDC->SelectObject(def_font1);
	font1.DeleteObject();

	CFont font2;
	LOGFONT lf2;
	memset(&lf2, 0, sizeof(LOGFONT));
	lf2.lfHeight = 30;
	strcpy_s((char *) lf2.lfFaceName, 16, "Times New Roman");
	font2.CreateFontIndirect(&lf2);
	CFont * def_font2 = pDC->SelectObject(&font2);
	pDC->TextOutW(0.9 * rc.Width() + 10, 0.9 * rc.Height() + 7, L"Значение");
	pDC->TextOutW(0.075 * rc.Width() - 105, 0.1 * rc.Height() - 20, L"Частота");
	
	CBrush brush;
	brush.CreateHatchBrush(6, RGB(54, 204, 255));
	CBrush * pOldBrush = (CBrush *)pDC->SelectObject(&brush);
	pDC->Rectangle(0.9 * rc.Width() - 350, 0.1 * rc.Height() + 145,
		0.9 * rc.Width() - 330, 0.1 * rc.Height() + 165);
	pDC->TextOutW(0.9 * rc.Width() - 320, 0.1 * rc.Height() + 140, L"- теоретические частоты");
	pDC->SelectObject(pOldBrush);
	brush.DeleteObject();
	brush.CreateHatchBrush(6, RGB(204, 204, 255));
	pOldBrush = (CBrush *)pDC->SelectObject(&brush);
	pDC->Rectangle(0.9 * rc.Width() - 350, 0.1 * rc.Height() + 180,
		0.9 * rc.Width() - 330, 0.1 * rc.Height() + 200);
	pDC->TextOutW(0.9 * rc.Width() - 320, 0.1 * rc.Height() + 175, L"- эмпирические частоты");
	brush.DeleteObject();
	
	pDC->SelectObject(def_font2);
	font2.DeleteObject();
}

void CGeomModView::arrowsPVal(CDC * pDC, CRect rc)
{
	CFont font;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 16;
	strcpy_s((char *)lf.lfFaceName, 16, "Times New Roman");
	font.CreateFontIndirect(&lf);
	CFont * def_font = pDC->SelectObject(&font);
	pDC->MoveTo(0.8 * rc.Width(), 0.9 * rc.Height());
	pDC->LineTo(0.8 * rc.Width() - 20, 0.9 * rc.Height() - 10);
	pDC->MoveTo(0.8 * rc.Width(), 0.9 * rc.Height());
	pDC->LineTo(0.8 * rc.Width() - 20, 0.9 * rc.Height() + 10);
	pDC->MoveTo(0.05 * rc.Width(), 0.05 * rc.Height());
	pDC->LineTo(0.05 * rc.Width() - 10, 0.05 * rc.Height() + 20);
	pDC->MoveTo(0.05 * rc.Width(), 0.05 * rc.Height());
	pDC->LineTo(0.05 * rc.Width() + 10, 0.05 * rc.Height() + 20);
	pDC->SelectObject(def_font);
	font.DeleteObject();
}

void CGeomModView::axisXpVal(CDC *pDC, CRect rc, int numOfTotalStates)
{
	CFont font;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 30;
	strcpy_s((char *) lf.lfFaceName, 16, "Times New Roman");
	font.CreateFontIndirect(&lf);
	CFont * def_font = pDC->SelectObject(&font);
	double hX = 0.725 * rc.Width() / numOfTotalStates;
	for (int i = 0, k = 0; k < numOfTotalStates + 1; i += hX, ++k)
	{
		CString letter;
		letter.Format(L"%.2f", k * 0.05);
		pDC->MoveTo(0.05 * rc.Width() + i, 0.9 * rc.Height() + 3);
		pDC->LineTo(0.05 * rc.Width() + i, 0.9 * rc.Height() - 3);
		pDC->TextOutW(0.05 * rc.Width() + i - 20, 0.9 * rc.Height() + 10, letter);
	}
	pDC->SelectObject(def_font);
	font.DeleteObject();
}

void CGeomModView::axisYpVal(CDC * pDC, CRect rc, int pValInd)
{
	CPoint axes[3] = { CPoint(0.05 * rc.Width(), 0.05 * rc.Height()),
		CPoint(0.05 * rc.Width(), 0.9 * rc.Height()),
		CPoint(0.8 * rc.Width(), 0.9 * rc.Height()) };
	pDC->Polyline(axes, 3);
	if (pValInd)
	{
		pDC->MoveTo(0.05 * rc.Width(), 0.9 * rc.Height());
		pDC->LineTo(0.775 * rc.Width(), 0.1 * rc.Height());
	}
	CFont font;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 25;
	strcpy_s((char *) lf.lfFaceName, 16, "Times New Roman");
	font.CreateFontIndirect(&lf);
	CFont* def_font = pDC->SelectObject(&font);
	int j = 20;
	double hY = 0.8 * rc.Height() / j;
	for (int i = 0, k = 0; k <= j; i += hY, ++k)
	{
		CString letter;
		letter.Format(L"%0.2f", k * 0.05);
		pDC->MoveTo(0.05 * rc.Width() - 3, 0.9 * rc.Height() - i);
		pDC->LineTo(0.05 * rc.Width() + 3, 0.9 * rc.Height() - i);
		pDC->TextOutW(0.05 * rc.Width() - 47, 0.9 * rc.Height() - i - 12, letter);
	}
	pDC->SelectObject(def_font);
	font.DeleteObject();
}

void CGeomModView::drawPValues(CDC * pDC, CRect rc, double array[20])
{
	double x = 0;
	double y = 0;
	CPen pen;
	pen.CreatePen(BS_SOLID, 3, RGB(51, 204, 255));
	CPen * OldPen = (CPen *) pDC->SelectObject(&pen);
	pDC->MoveTo(0.05 * rc.Width(), 0.9 * rc.Height());
	for (int i = 0; i < 20; ++i)
	{
		x = (0.05 + (i + 1) * 0.03625) * rc.Width();
		y = (0.9 - 0.8 * array[i]) * rc.Height();
		pDC->LineTo(x, y);
	}
	pDC->SelectObject(OldPen);
	pen.DeleteObject();
}

void CGeomModView::legendPVal(CDC * pDC, CRect rc, double p0, double p1, int pValueSize, int size)
{
	CFont font;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 30;
	strcpy_s((char *)lf.lfFaceName, 16, "Times New Roman");
	font.CreateFontIndirect(&lf);
	CFont* def_font = pDC->SelectObject(&font);

	CString letter;
	pDC->TextOutW( rc.Width() - 350, 0.2 * rc.Height(), L"Параметр p0:");
	letter.Format(L"%.2f", p0);
	pDC->TextOutW(rc.Width() - 350, 0.2 * rc.Height() + 25, letter);

	pDC->TextOutW(rc.Width() - 350, 0.2 * rc.Height() + 125, L"Параметр p1:");
	letter.Format(L"%.2f", p1);
	pDC->TextOutW(rc.Width() - 350, 0.2 * rc.Height() + 150, letter);

	pDC->TextOutW(rc.Width() - 350, 0.2 * rc.Height() + 250, L"Объём выборки p-value:");
	letter.Format(L"%d", pValueSize);
	pDC->TextOutW(rc.Width() - 350, 0.2 * rc.Height() + 275, letter);

	pDC->TextOutW(rc.Width() - 350, 0.2 * rc.Height() + 375, L"Объём выборки:");
	letter.Format(L"%d", size);
	pDC->TextOutW(rc.Width() - 350, 0.2 * rc.Height() + 400, letter);

	pDC->TextOutW(0.775 * rc.Width() + 50, 0.9 * rc.Height(), L"x");
	pDC->TextOutW(0.05 * rc.Width() - 60, 0.1 * rc.Height() - 60, L"F(x)");

	pDC->SelectObject(def_font);
	font.DeleteObject();
}

void CGeomModView::axisXtime(CDC * pDC, CRect rc)
{
	CFont font;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 30;
	strcpy_s((char *) lf.lfFaceName, 16, "Times New Roman");
	font.CreateFontIndirect(&lf);
	CFont * def_font = pDC->SelectObject(&font);
	double hX = 0.0725 * rc.Width();
	for (int i = 0, k = 0; k < 11; i += hX, ++k)
	{
		CString letter;
		letter.Format(L"%.2f", k * 0.1);
		pDC->MoveTo(0.05 * rc.Width() + i, 0.9 * rc.Height() + 3);
		pDC->LineTo(0.05 * rc.Width() + i, 0.9 * rc.Height() - 3);
		pDC->TextOutW(0.05 * rc.Width() + i - 20, 0.9 * rc.Height() + 10, letter);
	}
	pDC->TextOutW(0.775 * rc.Width() + 50, 0.9 * rc.Height(), L"Параметр p");
	pDC->TextOutW(0.05 * rc.Width() - 85, 0.1 * rc.Height() - 60, L"Время");
	pDC->SelectObject(def_font);
	font.DeleteObject();
}

void CGeomModView::drawTime(CDC * pDC, CRect rc, double * time, double maxY, int r, int g, int b)
{
	CPen pen;
	pen.CreatePen(BS_SOLID, 3, RGB(r, g, b));
	CPen * OldPen = (CPen *) pDC->SelectObject(&pen);
	double hY = 0.8 * rc.Height() / maxY;
	double hX = 0.725 * rc.Width() / 10;
	pDC->MoveTo(0.05 * rc.Width() + hX, 0.9 * rc.Height() - time[0] * hY);
	for (int i = 1, k = hX; i < 9; ++i, k += hX)
	{
		double x = 0.05 * rc.Width() + k;
		double y = 0.9 * rc.Height() - hY * time[i];
		pDC->LineTo(x + hX, y);
	}
	pDC->SelectObject(OldPen);
	pen.DeleteObject();
}

void CGeomModView::drawHist(CDC * pDC, CRect rc, ChiSquare * chiSq, double maxY)
{
	CBrush brush;
	brush.CreateHatchBrush(6, RGB(51, 204, 255));
	CBrush * pOldBrush = (CBrush *)pDC->SelectObject(&brush);
	axisXhist(pDC, rc, chiSq->getExpFrequency(), chiSq->getNumOfTotalStates(), maxY, 1);
	pDC->SelectObject(pOldBrush);
	brush.DeleteObject();

	brush.CreateHatchBrush(6, RGB(204, 204, 255));
	pOldBrush = (CBrush *)pDC->SelectObject(&brush);
	axisXhist(pDC, rc, chiSq->getObsFrequency(), chiSq->getNumOfTotalStates(), maxY, 9);
	brush.DeleteObject();
}

void CGeomModView::legendTime(CDC *pDC, CRect rc)
{
	CFont font;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 30;
	strcpy_s((char *) lf.lfFaceName, 16, "Times New Roman");
	font.CreateFontIndirect(&lf);
	CFont* def_font = pDC->SelectObject(&font);

	CBrush brush;
	brush.CreateHatchBrush(6, RGB(54, 204, 255));
	CBrush * pOldBrush = (CBrush *)pDC->SelectObject(&brush);
	pDC->Rectangle(0.8 * rc.Width(), 0.1 * rc.Height(),
		0.8 * rc.Width() - 20, 0.1 * rc.Height() + 20);
	pDC->TextOutW(0.8 * rc.Width() + 10, 0.1 * rc.Height() - 5, L"- метод Бернулли");
	pDC->SelectObject(pOldBrush);
	brush.DeleteObject();
	brush.CreateHatchBrush(6, RGB(51, 51, 255));
	pOldBrush = (CBrush *)pDC->SelectObject(&brush);
	pDC->Rectangle(0.8 * rc.Width(), 0.1 * rc.Height() + 35,
		0.8 * rc.Width() - 20, 0.1 * rc.Height() + 55);
	pDC->TextOutW(0.8 * rc.Width() + 10, 0.1 * rc.Height() + 30, L"- метод обратных функций");
	brush.DeleteObject();
	pDC->SelectObject(def_font);
	font.DeleteObject();
}

#endif //_DEBUG


// Обработчики сообщений CGeomModView