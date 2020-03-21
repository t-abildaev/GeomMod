
// GeomModDoc.h: интерфейс класса CGeomModDoc 
//
#pragma once

#include "ChiSquare.h"
#include "Random.h"
#include <ctime>
#include <chrono>


class CGeomModDoc : public CDocument
{
protected: // создать только из сериализации
	CGeomModDoc() noexcept;
	DECLARE_DYNCREATE(CGeomModDoc)

// Атрибуты
public:

// Операции
public:

// Переопределение
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Реализация
public:
	virtual ~CGeomModDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Вспомогательная функция, задающая содержимое поиска для обработчика поиска
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
protected:
	Distribution dist0, dist1;
	Sample * sample;
	ChiSquare chiSq;
	double * pValues;
	double * pValFrequency;
	double * timeBernoulli;
	double * timeInverse;
public:
	double m_p0;
	double m_p1;
	int m_size;
	int m_modMethod;
	int m_mode;
	int m_pValueSize;
	Sample * getSample() { return(sample); };
	ChiSquare * getChiSq() { return(&chiSq); };
	void calcPValFreq();
	double * getPValFrequency() { return(pValFrequency); };
	double * getTimeBernoulli() { return(timeBernoulli); };
	double * getTimeInverse() { return(timeInverse); };
	void calcTime(int method);
	afx_msg void OnParametrs();
	afx_msg void OnHistogram();
	afx_msg void OnPvalue();
	afx_msg void OnTime();
};