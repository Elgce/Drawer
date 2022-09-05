#pragma once
#include "pch.h"
class CGraph
{
public:
	CGraph(void);
	CGraph(int nDrawType, CPoint& ptOrigin, CPoint& mid,CPoint& ptEnd, COLORREF cColor, int cSize);
	~CGraph(void);
public:
	int m_nDrawType;//记录图形绘制的种类（如线段、三角形等）
	CPoint m_ptOrigin;//记录图形的起始点
	CPoint m_ptEnd;//记录图形的终止点
	CPoint m_mid;//记录任意三角形绘制模式下第二个顶点
	COLORREF m_cColor;//记录绘制图形的颜色
	int m_cSize;//记录绘制图形的画笔粗细
};