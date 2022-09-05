#pragma once
#include"pch.h"
class CSaveHelper
{
public:
	CSaveHelper() {};
	~CSaveHelper() {};
public:
	int Save(CWnd* pWnd, CString fillpath);
};