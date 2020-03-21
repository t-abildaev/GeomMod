
// GeomModView.h: интерфейс класса CGeomModView
//

#pragma once


class CGeomModView : public CView
{
protected: // создать только из сериализации
	CGeomModView() noexcept;
	DECLARE_DYNCREATE(CGeomModView)

// Атрибуты
public:
	CGeomModDoc* GetDocument() const;

	void arrowsHist(CDC * pDC, CRect rc);
	void arrowsPVal(CDC * pDC, CRect rc);

	void axisXhist(CDC * pDC, CRect rc, double * Frequency, int numOfTotalStates, int maxY, int indent);      
	void axisYhist(CDC * pDC, CRect rc, int maxY, double step);
	void drawHist(CDC * pDC, CRect rc, ChiSquare * chiSq, double maxY);
	void legend(CDC * pDC, CRect rc, double p, double chiSquare, int degOfFreedom, double pValue);
	
	void axisXpVal(CDC * pDC, CRect rc, int numOfTotalStates);
	void axisYpVal(CDC * pDC, CRect rc, int pValInd);                                                                     
	void drawPValues(CDC * pDC, CRect rc, double array[20]);
	void legendPVal(CDC * pDC, CRect rc, double p0, double p1, int pValueSize, int size);

	void axisXtime(CDC * pDC, CRect rc);
	void drawTime(CDC * pDC, CRect rc, double * time, double maxY, int r, int g, int b);
	void legendTime(CDC * pDC, CRect rc);
	
// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Реализация
public:
	virtual ~CGeomModView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // версия отладки в GeomModView.cpp
inline CGeomModDoc* CGeomModView::GetDocument() const
   { return reinterpret_cast<CGeomModDoc*>(m_pDocument); }
#endif

