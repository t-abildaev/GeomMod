
// GeomModDoc.cpp: реализация класса CGeomModDoc 
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "GeomMod.h"
#endif

#include "Dialog.h"

#include "GeomModDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGeomModDoc

IMPLEMENT_DYNCREATE(CGeomModDoc, CDocument)

BEGIN_MESSAGE_MAP(CGeomModDoc, CDocument)
	ON_COMMAND(ID_PARAMETRS, &CGeomModDoc::OnParametrs)
	ON_COMMAND(ID_HISTOGRAM, &CGeomModDoc::OnHistogram)
	ON_COMMAND(ID_PVALUE, &CGeomModDoc::OnPvalue)
	ON_COMMAND(ID_TIME, &CGeomModDoc::OnTime)
END_MESSAGE_MAP()


// Создание или уничтожение CGeomModDoc

CGeomModDoc::CGeomModDoc() noexcept
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	mtinit(seed);
	m_p0 = 0.5;
	m_p1 = 0.5;
	m_size = 100;
	m_modMethod = 1;
	m_mode = 0;
	m_pValueSize = 1000;
	pValues = new double[m_pValueSize];
	pValFrequency = new double[20];
	timeBernoulli = new double[9];
	timeInverse = new double[9];
	sample = new Bernoulli(m_size);
	sample->setProb(m_p1);
	sample->simulate();
	dist0.setProb(m_p0);
	dist1.setProb(m_p1);
	chiSq.set(dist1, sample);
	chiSq.calculate();
}

CGeomModDoc::~CGeomModDoc()
{
	delete[] sample;
	delete[] pValues;
	delete[] pValFrequency;
	delete[] timeBernoulli;
	delete[] timeInverse;
}

BOOL CGeomModDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: добавьте код повторной инициализации
	// (Документы SDI будут повторно использовать этот документ)

	return TRUE;
}




// Сериализация CGeomModDoc

void CGeomModDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: добавьте код сохранения
	}
	else
	{
		// TODO: добавьте код загрузки
	}
}

#ifdef SHARED_HANDLERS

// Поддержка для эскизов
void CGeomModDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Измените этот код для отображения данных документа
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Поддержка обработчиков поиска
void CGeomModDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Задание содержимого поиска из данных документа.
	// Части содержимого должны разделяться точкой с запятой ";"

	// Например:  strSearchContent = _T("точка;прямоугольник;круг;объект ole;");
	SetSearchContent(strSearchContent);
}

void CGeomModDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Диагностика CGeomModDoc

#ifdef _DEBUG
void CGeomModDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGeomModDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Команды CGeomModDoc


void CGeomModDoc::calcPValFreq()
{
	delete[] pValFrequency;
	pValFrequency = new double[20];
	int i = 0;
	int j = 0;
	double step = 0.05;
	double h = 0.05;
	std::sort(pValues, pValues + m_pValueSize);
	while (j < m_pValueSize)
	{
		if (pValues[j] > h)
		{
			pValFrequency[i] = (double)(j) / m_pValueSize;
			h += step;
			++i;
		}
		++j;
	}
	while (pValFrequency[i] < 0 && i < 20)
	{
		pValFrequency[i] = 1;
		++i;
	}
}

void CGeomModDoc::calcTime(int method)
{
	int j = 0;
	double * time = new double[9];
	for (double p = 0.1; p <= 0.9; p += 0.1)
	{
		sample->setProb(p);
		double mean = 0;
		for (int i = 0; i < 1000; ++i)
		{
			clock_t start = clock();
			sample->simulate();
			clock_t end = clock();
			mean += (double)(end - start);
		}
		time[j] = mean / 1000;
		++j;
	}
	if (method == 0)
		for (int i = 0; i < 9; ++i)
			timeBernoulli[i] = time[i];
	else
		for (int i = 0; i < 9; ++i)
			timeInverse[i] = time[i];
	delete[] time;
	UpdateAllViews(0);
}

void CGeomModDoc::OnParametrs()
{
	Dialog d;
	d.m_p0 = m_p0;
	d.m_p1 = m_p1;
	d.m_size = m_size;
	d.m_modMethod = m_modMethod;
	d.m_mode = m_mode;
	d.m_pValueSize = m_pValueSize;
	if (d.DoModal() == IDOK)
	{
		m_p0 = d.m_p0;
		m_p1 = d.m_p1;
		m_size = d.m_size;
		m_modMethod = d.m_modMethod;
		m_mode = d.m_mode;
		m_pValueSize = d.m_pValueSize;
		delete[] sample;
		dist0.setProb(d.m_p0);
		dist1.setProb(d.m_p1);
		if (d.m_modMethod) sample = new Inverse(d.m_size);
		else sample = new Bernoulli(d.m_size);
		sample->setProb(m_p1);
		switch (m_mode)
		{
		case 0:
			sample->simulate();
			chiSq.set(dist1, sample);
			chiSq.calculate();
			break;
		case 1:
			delete[] pValues;
			pValues = new double[d.m_pValueSize];
			sample->simulate();
			chiSq.set(dist0, sample);
			chiSq.calculate();
			pValues[0] = chiSq.getPValue();
			for (int i = 1; i < d.m_pValueSize; ++i)
			{
				sample->simulate();
				chiSq.set(dist0, sample);
				chiSq.calculate();
				pValues[i] = chiSq.getPValue();
			}
			calcPValFreq();
			break;
		case 2:
			delete[] timeBernoulli;
			delete[] timeInverse;
			timeBernoulli = new double[9];
			timeInverse = new double[9];
			delete[] sample;
			sample = new Bernoulli(d.m_size);
			calcTime(0);
			delete[] sample;
			sample = new Inverse(d.m_size);
			calcTime(1);
			break;
		}
		UpdateAllViews(0);
	}
}

void CGeomModDoc::OnHistogram()
{
	Dialog d;
	m_mode = 0;
	d.m_mode = 0;
	delete[] sample;
	if (m_modMethod) sample = new Inverse(m_size);
	else sample = new Bernoulli(m_size);
	sample->setProb(m_p1);
	sample->simulate();
	chiSq.set(dist1, sample);
	chiSq.calculate();
	UpdateAllViews(0);
}

void CGeomModDoc::OnPvalue()
{
	Dialog d;
	m_mode = 1;
	d.m_mode = 1;
	delete[] pValues;
	pValues = new double[m_pValueSize];
	delete[] sample;
	if (m_modMethod) sample = new Inverse(m_size);
	else sample = new Bernoulli(m_size);
	sample->setProb(m_p1);
	sample->simulate();
	chiSq.set(dist0, sample);
	chiSq.calculate();
	pValues[0] = chiSq.getPValue();
	for (int i = 1; i < m_pValueSize; ++i)
	{
		sample->simulate();
		chiSq.set(dist0, sample);
		chiSq.calculate();
		pValues[i] = chiSq.getPValue();
	}
	calcPValFreq();
	UpdateAllViews(0);
}


void CGeomModDoc::OnTime()
{
	Dialog d;
	m_mode = 2;
	m_mode = 2;
	delete[] timeBernoulli;
	delete[] timeInverse;
	timeBernoulli = new double[9];
	timeInverse = new double[9];
	delete[] sample;
	sample = new Bernoulli(m_size);
	calcTime(0);
	delete[] sample;
	sample = new Inverse(m_size);
	calcTime(1);
	UpdateAllViews(0);
}