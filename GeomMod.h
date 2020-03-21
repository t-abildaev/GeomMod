
// GeomMod.h: основной файл заголовка для приложения GeomMod
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // основные символы


// GeomMod:
// Сведения о реализации этого класса: GeomMod.cpp
//

class GeomMod : public CWinAppEx
{
public:
	GeomMod() noexcept;


// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern GeomMod theApp;
