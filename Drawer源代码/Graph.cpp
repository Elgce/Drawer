#include"pch.h"
#include"Graph.h"

CGraph::CGraph(int nDrawType, CPoint& ptOrigin,CPoint& mid, CPoint& ptEnd, COLORREF cColor, int cSize)
{
	m_nDrawType = nDrawType;
	m_ptOrigin = ptOrigin;
	m_ptEnd = ptEnd;
	m_cColor = cColor;
	m_cSize = cSize;
	m_mid = mid;
}


CGraph::~CGraph(void)
{
}