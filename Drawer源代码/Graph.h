#pragma once
#include "pch.h"
class CGraph
{
public:
	CGraph(void);
	CGraph(int nDrawType, CPoint& ptOrigin, CPoint& mid,CPoint& ptEnd, COLORREF cColor, int cSize);
	~CGraph(void);
public:
	int m_nDrawType;//��¼ͼ�λ��Ƶ����ࣨ���߶Ρ������εȣ�
	CPoint m_ptOrigin;//��¼ͼ�ε���ʼ��
	CPoint m_ptEnd;//��¼ͼ�ε���ֹ��
	CPoint m_mid;//��¼���������λ���ģʽ�µڶ�������
	COLORREF m_cColor;//��¼����ͼ�ε���ɫ
	int m_cSize;//��¼����ͼ�εĻ��ʴ�ϸ
};