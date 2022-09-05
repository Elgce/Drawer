
// ChildView.cpp: CChildView 类的实现
//

#include "pch.h"
#include "framework.h"
#include "Drawer.h"
#include "ChildView.h"
#include<cmath>
#include "ChooseClr.h"
#include <vector>
#include "Graph.h"
#include "CWidth.h"
#include "CSaveHelper.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define DRAW_LINE 1
#define DRAW_ELLIPSE 2
#define DRAW_RECTANGLE 3
#define DRAW_TTRIANGLE 4
#define DRAW_DTRIANGLE 5
using namespace std;

// CChildView

CChildView::CChildView()
{
	flag = -1;
	do_flag = -1;
	trnum = 0;
	number = 0;
	pen_size = 0;
	pen_color = RGB(0, 0, 0);
	mid.x = 0; mid.y = 0;
	a = 0;
	cho_pen[0].CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_Line, &CChildView::OnLine)
	ON_COMMAND(ID_Rectangle, &CChildView::OnRectangle)
	ON_COMMAND(ID_Oval, &CChildView::OnOval)
ON_COMMAND(ID_RED, &CChildView::OnRed)
ON_COMMAND(ID_BLACK, &CChildView::OnBlack)
ON_COMMAND(ID_BLUE, &CChildView::OnBlue)
ON_COMMAND(ID_GREEN, &CChildView::OnGreen)
ON_COMMAND(ID_RGB, &CChildView::OnChooseRgb)
ON_COMMAND(ID_Pencil, &CChildView::OnPencil)
ON_COMMAND(ID_Eraser, &CChildView::OnEraser)
ON_COMMAND(ID_CANCEL, &CChildView::OnCancel)
ON_COMMAND(ID_TTriangle, &CChildView::OnTtriangle)
ON_COMMAND(ID_DTriangle, &CChildView::OnDtriangle)
ON_COMMAND(ID_CHWidth, &CChildView::OnChwidth)
ON_COMMAND(ID_FILE_OPEN, &CChildView::OnFileOpen)
ON_COMMAND(ID_Save, &CChildView::OnSave)
ON_COMMAND(ID_APP_ABOUT, &CChildView::OnAppAbout)
END_MESSAGE_MAP()

COLORREF m_bgcolor;

// CChildView 消息处理程序


CPoint downPoint; 
int  R = 0, G = 0, B = 0;

bool start;
bool go = false;

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint()
{
	
	CPaintDC dc(this);
	CRect rect;

	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255));
	OnDraw(&dc);
}

BOOL CChildView::showJpgmap(CDC* pDC, CString strPath)
{
	CFileStatus fstatus;
	CFile file;
	ULONGLONG cb;
	//打开文件并检测文件的有效性
	if (!file.Open(strPath, CFile::modeRead))
	{
		return FALSE;
	}
	if (!file.GetStatus(strPath, fstatus))
	{
		return FALSE;
	}
	if ((cb = fstatus.m_size) <= 0)
	{
		return FALSE;
	}
	// 根据文件大小分配内存空间,记得释放内存
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, (unsigned int)cb);
	if (hGlobal == NULL)
	{
		return FALSE;
	}

	// 锁定刚才分配的内存空间
	LPVOID pvData = NULL;
	pvData = GlobalLock(hGlobal);
	if (pvData == NULL)
	{
		GlobalFree(hGlobal);  // 记得释放内存
		return FALSE;
	}

	// 将文件放到流中
	IStream* pStm;
	file.Read(pvData, (unsigned int)cb);
	GlobalUnlock(hGlobal);
	CreateStreamOnHGlobal(hGlobal, TRUE, &pStm);

	// 从流中加载图片
	// 显示JPEG和GIF格式的图片，GIF只能显示一帧，还不能显示动画，
	// 要显示动画GIF请使用ACTIVE控件。
	IPicture* pPic;
	if (OleLoadPicture(pStm, (LONG)fstatus.m_size, TRUE, IID_IPicture, (LPVOID*)&pPic) != S_OK)
	{
		GlobalFree(hGlobal);  // 记得释放内存
		return FALSE;
	}

	//获取图像宽和高,注意这里的宽和高不是图像的分辨率
	OLE_XSIZE_HIMETRIC hmWidth;
	OLE_YSIZE_HIMETRIC hmHeight;
	pPic->get_Width(&hmWidth);
	pPic->get_Height(&hmHeight);

	RECT rtWnd;
	pDC->GetWindow()->GetWindowRect(&rtWnd);
	int iWndWidth = rtWnd.right - rtWnd.left;
	int iWndHeight = rtWnd.bottom - rtWnd.top;

	if (pPic->Render(*pDC, 0, 0, iWndWidth, iWndHeight, 0, hmHeight, hmWidth, -hmHeight, NULL)==0)
	{
		pPic->Release();
		GlobalFree(hGlobal);  // 记得释放内存
		return false;
	}

	pPic->Release();
	GlobalFree(hGlobal);  // 记得释放内存
	return true;
	
}
void CChildView::showBitmap(CDC* pDC, CString BmpName)
{
	HBITMAP m_hBitmap;
	m_hBitmap = (HBITMAP)LoadImage(NULL, BmpName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);

	if (m_bitmap.m_hObject)
	{
		m_bitmap.Detach();
	}
	m_bitmap.Attach(m_hBitmap);

	CRect rect;
	GetClientRect(&rect);
	int m_showX = 0;
	int m_showY = 0;
	int m_nWindowWidth = rect.right - rect.left;
	int m_nWindowHeight = rect.bottom - rect.top;

	CDC dcBmp;
	if (!dcBmp.CreateCompatibleDC(pDC))
		return;
	BITMAP m_bmp;
	m_bitmap.GetBitmap(&m_bmp);
	CBitmap* pbmpOld = NULL;
	dcBmp.SelectObject(&m_bitmap);
	pDC->StretchBlt(0, 0, m_bmp.bmWidth, m_bmp.bmHeight, &dcBmp, 0, 0, m_bmp.bmWidth, m_bmp.bmHeight, SRCCOPY);
	dcBmp.SelectObject(pbmpOld);
	DeleteObject(&m_bitmap);
	dcBmp.DeleteDC();
}
BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}
void CChildView::OnDraw(CDC* pDC)
{
	if (EntName.Compare(_T("bmp")) == 0)
	{
		showBitmap(pDC, BmpName);
	}
	else if (EntName.Compare(_T("jpg")) == 0)
	{
		showJpgmap(pDC, BmpName);
	}


	int n = ptr.GetSize();
	for (int j = 0; j < n; j++)
	{
		CPen pen; pen.CreatePen(PS_SOLID, ((CGraph*)ptr.GetAt(j))->m_cSize, ((CGraph*)ptr.GetAt(j))->m_cColor);
		pDC->SelectObject(&pen);
		switch (((CGraph*)ptr.GetAt(j))->m_nDrawType)
		{
		case DRAW_LINE:   
			//直线、涂鸦、橡皮的重绘
			pDC->MoveTo(((CGraph*)ptr.GetAt(j))->m_ptOrigin);
			pDC->LineTo(((CGraph*)ptr.GetAt(j))->m_ptEnd);
			break;
		case DRAW_TTRIANGLE:
			pDC->MoveTo(((CGraph*)ptr.GetAt(j))->m_ptOrigin);
			pDC->LineTo(((CGraph*)ptr.GetAt(j))->m_mid);
			pDC->MoveTo(((CGraph*)ptr.GetAt(j))->m_mid);
			pDC->LineTo(((CGraph*)ptr.GetAt(j))->m_ptEnd);
			pDC->MoveTo(((CGraph*)ptr.GetAt(j))->m_ptEnd);
			pDC->LineTo(((CGraph*)ptr.GetAt(j))->m_ptOrigin);
			break;
		case DRAW_DTRIANGLE:
			double len;
			int dx = (((CGraph*)ptr.GetAt(j))->m_ptOrigin.x - ((CGraph*)ptr.GetAt(j))->m_ptEnd.x) * (((CGraph*)ptr.GetAt(j))->m_ptOrigin.x - ((CGraph*)ptr.GetAt(j))->m_ptEnd.x);
			int dy= (((CGraph*)ptr.GetAt(j))->m_ptOrigin.y - ((CGraph*)ptr.GetAt(j))->m_ptEnd.y) * (((CGraph*)ptr.GetAt(j))->m_ptOrigin.y - ((CGraph*)ptr.GetAt(j))->m_ptEnd.y);
			len = sqrt(dx + dy);
			pDC->MoveTo(((CGraph*)ptr.GetAt(j))->m_ptOrigin.x, ((CGraph*)ptr.GetAt(j))->m_ptOrigin.y-len);
			pDC->LineTo(((CGraph*)ptr.GetAt(j))->m_ptOrigin.x-((len)*sqrt(3))/2, ((CGraph*)ptr.GetAt(j))->m_ptOrigin.y+len/2);
			pDC->MoveTo(((CGraph*)ptr.GetAt(j))->m_ptOrigin.x, ((CGraph*)ptr.GetAt(j))->m_ptOrigin.y - len);
			pDC->LineTo(((CGraph*)ptr.GetAt(j))->m_ptOrigin.x+((len) * sqrt(3))/2, ((CGraph*)ptr.GetAt(j))->m_ptOrigin.y + len / 2);
		    pDC->MoveTo(((CGraph*)ptr.GetAt(j))->m_ptOrigin.x - ((len) * sqrt(3))/2, ((CGraph*)ptr.GetAt(j))->m_ptOrigin.y + len / 2);
			pDC->LineTo(((CGraph*)ptr.GetAt(j))->m_ptOrigin.x + ((len) *sqrt(3))/2, ((CGraph*)ptr.GetAt(j))->m_ptOrigin.y + len / 2);
			break;
		}
	}
	for (int j = 0; j < n; j++)
	{
		CPen pen; pen.CreatePen(PS_SOLID, ((CGraph*)ptr.GetAt(j))->m_cSize, ((CGraph*)ptr.GetAt(j))->m_cColor);
		pDC->SelectObject(&pen);
		switch (((CGraph*)ptr.GetAt(j))->m_nDrawType)
		{
		case DRAW_ELLIPSE:                                               //椭圆重绘
			pDC->SetROP2(R2_NOTXORPEN);//调用反色笔
			pDC->Ellipse(((CGraph*)ptr.GetAt(j))->m_ptOrigin.x, ((CGraph*)ptr.GetAt(j))->m_ptOrigin.y,
				((CGraph*)ptr.GetAt(j))->m_ptEnd.x, ((CGraph*)ptr.GetAt(j))->m_ptEnd.y);
			break;
		case DRAW_RECTANGLE:
			pDC->SetROP2(R2_NOTXORPEN);//调用反色笔
			pDC->Rectangle(((CGraph*)ptr.GetAt(j))->m_ptOrigin.x, ((CGraph*)ptr.GetAt(j))->m_ptOrigin.y,
				((CGraph*)ptr.GetAt(j))->m_ptEnd.x, ((CGraph*)ptr.GetAt(j))->m_ptEnd.y);
			break;
		}
	}

}

int type;

void CChildView::OnLine()
{
	// TODO: 在此添加命令处理程序代码
	//type = 1;
	flag = 0; do_flag = 0;
}



void CChildView::OnRectangle()
{
	// TODO: 在此添加命令处理程序代码
	//type = 2;
	flag = 3; do_flag = 0;
}


void CChildView::OnOval()
{
	// TODO: 在此添加命令处理程序代码
	//type=3;
	flag = 2; do_flag = 0;
}


void CChildView::OnPencil()
{
	// TODO: 在此添加命令处理程序代码
	//type = 4;
	flag = 1; do_flag = 0;
}
void CChildView::OnEraser()
{
	// TODO: 在此添加命令处理程序代码
	//type = 5;
	flag = 4; do_flag = 0;
	if (a == 0)
	{
		w_cho_pen.CreatePen(PS_SOLID, 10, RGB(255, 255, 255));
		a = 1;
	}
}
void CChildView::OnTtriangle()
{
	// TODO: 在此添加命令处理程序代码
	flag = 5; do_flag = 0;
}



void CChildView::OnDtriangle()
{
	// TODO: 在此添加命令处理程序代码
	flag = 6; do_flag = 0;
}





void CChildView::OnRed()
{
	// TODO: 在此添加命令处理程序代码
	R = 255; G = B = 0;
	CreatePen(pen_size, RGB(R, G, B));
	pen_color = RGB(R, G, B);
}


void CChildView::OnBlack()
{
	// TODO: 在此添加命令处理程序代码
	R = G = B = 0;
	CreatePen(pen_size, RGB(R, G, B));
	pen_color = RGB(R, G, B);
}


void CChildView::OnBlue()
{
	// TODO: 在此添加命令处理程序代码
	R = G = 0; B = 255;
	CreatePen(pen_size, RGB(R, G, B));
	pen_color = RGB(R, G, B);
}


void CChildView::OnGreen()
{
	// TODO: 在此添加命令处理程序代码
	G = 255; R = B = 0;
	CreatePen(pen_size, RGB(R, G, B));
	pen_color = RGB(R, G, B);
}

void CChildView::OnCancel()
{
	// TODO: 在此添加命令处理程序代码
	int nn = ptr.GetCount();
	if (nn <= 0) return;
	ptr.RemoveAt(nn - 1);
	CDC* pDC = GetDC();
	OnDraw(pDC);
	RedrawWindow();
}






void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	if (do_flag == 0 && flag!=5)
	{
		p.x = point.x;
		p.y = point.y;
		do_flag = 1;
	}
	else if (do_flag == 0)
	{
		if (trnum == 0)
		{
			trnum++;
			p.x = point.x;
			p.y = point.y;
		}
		else if (trnum == 1)
		{
			trnum++;mid.x = point.x;
			mid.y = point.y;
		}
		else if (trnum == 2)
		{

			trnum = 0;
			HDC hdc = GetDC()->m_hDC;
			CDC* dc = GetDC();
			CPen* pen = dc->SelectObject(&cho_pen[number]);
				dc->MoveTo(p);
				dc->LineTo(mid);
				dc->MoveTo(mid);
				dc->LineTo(point);
				dc->MoveTo(point);
				dc->LineTo(p);
				dc->TextOutW(0, 0, _T(" "));
				dc->SelectObject(pen);
				CGraph* pGraph = new CGraph(DRAW_TTRIANGLE, p, mid, point, pen_color, pen_size);
				ptr.Add(pGraph);
		}
	}
	
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (flag == 0 && do_flag == 1)
	{
		CDC* dc = GetDC();
		CPen* pen = dc->SelectObject(&cho_pen[number]);
		if (go)
		{
			dc->SetROP2(R2_NOTXORPEN);//调用反色笔
			dc->MoveTo(p);
			dc->LineTo(term);
		}
		else go = true;
		term.x = point.x; term.y = point.y;
		dc->MoveTo(p);
		dc->LineTo(term);
		dc->TextOutW(0, 0, _T(" "));
	}
	if (flag == 1 && do_flag == 1)                                                 //涂鸦，需要调用mousemove消息的响应函数
	{
		CDC* dc = GetDC(); CPen* pen = dc->SelectObject(&cho_pen[number]);
		dc->MoveTo(p);
		dc->LineTo(point); dc->TextOutW(0, 0, _T(" "));
		CGraph* pGraph = new CGraph(DRAW_LINE, p, mid,point, pen_color, pen_size);
		ptr.Add(pGraph);
		p.x = point.x; p.y = point.y;
	}
	if (flag == 2 && do_flag == 1)
	{
		CDC* dc = GetDC();
		CPen* pen = dc->SelectObject(&cho_pen[number]);
		if (go)
			{
				dc->SetROP2(R2_NOTXORPEN);//调用反色笔
				dc->Ellipse(p.x, p.y, term.x, term.y);
			}
		else go = true;
		term = point;
		if (nFlags & MK_CONTROL)
		{
			int maxx = max(p.x, term.x);
			int minx = min(p.x, term.x);
			int miny = min(p.y, term.y);
			int maxy = max(p.y, term.y);
			int dl = min(maxx - minx, maxy - miny);
			if (p.x > term.x)
				term.x = p.x - dl;
			else
				term.x = p.x + dl;
			if (p.y > term.y)
				term.y = p.y - dl;
			else
				term.y = p.y + dl;
		}
		dc->Ellipse(p.x, p.y, term.x, term.y);
		dc->TextOutW(0, 0, _T(" "));

	}
	if (flag == 3 && do_flag == 1)
	{
		CDC* dc = GetDC();	
		CPen* pen = dc->SelectObject(&cho_pen[number]);
		if (go)
		{
			dc->SetROP2(R2_NOTXORPEN);//调用反色笔
			dc->Rectangle(p.x, p.y, term.x, term.y);
		}
		else go = true;
		term = point;
		if (nFlags & MK_CONTROL)
		{
			int maxx = max(p.x, term.x);
			int minx = min(p.x, term.x);
			int miny = min(p.y, term.y);
			int maxy = max(p.y, term.y);
			int dl = min(maxx - minx, maxy - miny);
			if (p.x > term.x)
				term.x = p.x - dl;
			else
				term.x = p.x + dl;
			if (p.y > term.y)
				term.y = p.y - dl;
			else
				term.y = p.y + dl;
			
		}
		dc->Rectangle(p.x, p.y, term.x, term.y);
		dc->TextOutW(0, 0, _T(" "));
	}
	if (flag == 4 && do_flag == 1)                                                   //橡皮操作，类似白色画笔涂鸦
	{
		CDC* dc = GetDC(); CPen* pen = dc->SelectObject(&w_cho_pen);
		dc->MoveTo(p);
		dc->LineTo(point); dc->TextOutW(0, 0, _T(" "));
		CGraph* pGraph = new CGraph(DRAW_LINE, p, mid,point, RGB(255, 255, 255), 10);
		ptr.Add(pGraph);
		p.x = point.x; p.y = point.y;
	}
	if (flag == 6 && do_flag == 1)
	{
		double len; int dx, dy;
		CDC* pDC = GetDC(); CPen* pen = pDC->SelectObject(&cho_pen[number]);
		if (go)
		{
			pDC->SetROP2(R2_NOTXORPEN);//调用反色笔
			
			dx = (p.x - term.x) * (p.x - term.x);
			dy = (p.y -term.y) * (p.y - term.y);
			len = sqrt(dx + dy);
			pDC->MoveTo(p.x, p.y - len);
			pDC->LineTo(p.x - ((len) * sqrt(3))/2, p.y + len / 2);
			pDC->MoveTo(p.x, p.y - len);
			pDC->LineTo(p.x + ((len) * sqrt(3))/2, p.y + len / 2);
			pDC->MoveTo(p.x - ((len) * sqrt(3))/2, p.y + len / 2);
			pDC->LineTo(p.x + ((len) * sqrt(3))/2, p.y + len / 2);
		}
		else go = true;
		term = point;
		dx = (p.x - term.x) * (p.x - term.x);
		dy = (p.y - term.y) * (p.y - term.y);
		len = sqrt(dx + dy);
		pDC->MoveTo(p.x, p.y - len);
		pDC->LineTo(p.x - ((len)*sqrt(3)) / 2, p.y + len / 2);
		pDC->MoveTo(p.x, p.y - len);
		pDC->LineTo(p.x + ((len)*sqrt(3)) / 2, p.y + len / 2);
		pDC->MoveTo(p.x - ((len)*sqrt(3)) / 2, p.y + len / 2);
		pDC->LineTo(p.x + ((len)*sqrt(3)) / 2, p.y + len / 2);
	}
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	HDC hdc = GetDC()->m_hDC;
	CDC* dc = GetDC();
	CPen* pen = dc->SelectObject(&cho_pen[number]);
	if (flag == 0 && do_flag == 1)                                                      //直线绘制，需要刷新窗口才能即时显示
	{
		dc->MoveTo(p);
		dc->LineTo(point);
		dc->TextOutW(0, 0, _T(" "));
		dc->SelectObject(pen);
		CGraph* pGraph = new CGraph(DRAW_LINE, p, mid,point, pen_color, pen_size);
		ptr.Add(pGraph);
	}
	if (flag == 1 && do_flag == 1)                                                     //涂鸦绘制
	{
		dc->MoveTo(p);
		dc->LineTo(point);
		CGraph* pGraph = new CGraph(DRAW_LINE, p,mid, point, pen_color, pen_size);
		ptr.Add(pGraph);
	}
	if (flag == 2 && do_flag == 1)                                                     //椭圆绘制
	{
		if (go)
		{
			{
				dc->SetROP2(R2_NOTXORPEN);//调用反色笔
				dc->Ellipse(p.x, p.y, term.x, term.y);
			}
		}
		else go = true;
		term = point;
		if (nFlags & MK_CONTROL)
		{
			int maxx = max(p.x, term.x);
			int minx = min(p.x, term.x);
			int miny = min(p.y, term.y);
			int maxy = max(p.y, term.y);
			int dl = min(maxx - minx, maxy - miny);
			if (p.x > term.x)
				term.x = p.x - dl;
			else
				term.x = p.x + dl;
			if (p.y > term.y)
				term.y = p.y - dl;
			else
				term.y = p.y + dl;
		}

		dc->Ellipse(p.x, p.y, term.x, term.y);
		CGraph* pGraph = new CGraph(DRAW_ELLIPSE, p,mid, term, pen_color, pen_size);
		ptr.Add(pGraph);
	}
	if (flag == 3 && do_flag == 1)                                                       //矩形绘制
	{
		if (go)
		{
			dc->SetROP2(R2_NOTXORPEN);//调用反色笔
			dc->Rectangle(p.x, p.y, term.x, term.y);
		}
		else go = true;
		term = point;
		if (nFlags & MK_CONTROL)
		{
			int maxx = max(p.x, term.x);
			int minx = min(p.x, term.x);
			int miny = min(p.y, term.y);
			int maxy = max(p.y, term.y);
			int dl = min(maxx - minx, maxy - miny);
			if (p.x > term.x)
				term.x = p.x - dl;
			else
				term.x = p.x + dl;
			if (p.y > term.y)
				term.y = p.y - dl;
			else
				term.y = p.y + dl;

		}
		dc->Rectangle(p.x, p.y, term.x, term.y);
		CGraph* pGraph = new CGraph(DRAW_RECTANGLE, p,mid, term, pen_color, pen_size);
		ptr.Add(pGraph);
	}
	if (flag == 4 && do_flag == 1)                                                     //橡皮绘制，存储的画笔信息为固定的
	{
		pen = dc->SelectObject(&w_cho_pen);
		dc->MoveTo(p);
		dc->LineTo(point);
		CGraph* pGraph = new CGraph(DRAW_LINE, p,mid, point, RGB(255, 255, 255), 10);
		ptr.Add(pGraph);
	}
	
	if (flag == 6 && do_flag == 1)
	{
		term = point;
		int dx = (p.x - term.x) * (p.x - term.x);
		int dy = (p.y - term.y) * (p.y - term.y);
		double len = sqrt(dx + dy);
		dc->MoveTo(p.x, p.y - len);
		dc->LineTo(p.x - ((len)*sqrt(3)) / 2, p.y + len / 2);
		dc->MoveTo(p.x, p.y - len);
		dc->LineTo(p.x + ((len)*sqrt(3)) / 2, p.y + len / 2);
		dc->MoveTo(p.x - ((len)*sqrt(3)) / 2, p.y + len / 2);
		dc->LineTo(p.x + ((len)*sqrt(3)) / 2, p.y + len / 2);
		CGraph* pGraph = new CGraph(DRAW_DTRIANGLE, p, mid, term, pen_color, pen_size);
		ptr.Add(pGraph);
	}
	::ReleaseDC(GetActiveWindow()->m_hWnd, hdc); do_flag = 0;
	go = false;
	if (flag==6) Invalidate();
}




void CChildView::OnChooseRgb()
{
	// TODO: 在此添加命令处理程序代码
	ChooseClr c;
	c.DoModal();
	int len = 0; R = G = B = 0;
	len=c.greenVal.GetLength();
	for (int i = 1; i <= len; i++)
	{
		G = G * 10 + int(c.greenVal[i-1]-'0');
	}
	len = c.blueVal.GetLength();
	for (int i = 1; i <= len; i++)
	{
		B = B * 10 + int(c.blueVal[ i-1]-'0');
	}
	len = c.redVal.GetLength();
	for (int i = 1; i <= len; i++)
	{
		R= R * 10 + int(c.redVal[i-1]-'0');
	}
	CreatePen(pen_size, RGB(R, G, B));
	TRACE(_T("%ld %ld %ld\n"), R, G, B);
	pen_color = RGB(R, G, B);
}
BOOL CChildView::CreatePen(int pensize, COLORREF pencolor)  //当画笔信息更改时创建新画笔，橡皮操作不在此列
{
	if (pensize != pen_size || pencolor != pen_color)
	{
		number++;
		cho_pen[number].CreatePen(PS_SOLID, pensize, pencolor);
		pen_size = pensize;
	}
	return true;
}

void CChildView::OnChwidth()
{
	// TODO: 在此添加命令处理程序代码
	CWidth w;
	w.DoModal(); pen_size = 0;
	int wid = 0;
	int len = 0;
	len = w.width.GetLength();
	for (int i = 1; i <= len; i++)
	{
		pen_size = pen_size * 10 + int(w.width[i-1] - '0');
	}
	number++;cho_pen[number]. CreatePen(PS_SOLID,pen_size, pen_color);
}

//文件打开
void CChildView::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	CString filter;
	filter = "所有文件（*.bmp,*.jpg,*gif,*tiff）|*.bmp;*.jpg;*gif;*.tiff| BMP(*.bmp)|*.bmp|JPG(*.jpg)|*.jpg|GIF(*gif)|*.gif|TIFF(*tiff)|*.tiff||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter, NULL);
	if (dlg.DoModal() == IDOK)
	{
		BmpName = dlg.GetPathName();//获取文件路径名
		EntName = dlg.GetFileExt();//获取文件扩展名
		EntName.MakeLower();//将文件扩展名转换为一个小写字符
		Invalidate();
	}
}


void CChildView::OnSave()
{
	// TODO: 在此添加命令处理程序代码
	CString fileName;fileName = ".bmp";
	CString strFilter;strFilter =  "所有文件（*.bmp,*.jpg,*gif,*tiff）|*.bmp;*.jpg;*gif;*.tiff| BMP(*.bmp)|*.bmp|JPG(*.jpg)|*.jpg|GIF(*gif)|*.gif|TIFF(*tiff)|*.tiff||";
	CFileDialog dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY, strFilter, NULL);
	if (dlg.DoModal() == IDOK)
	{
		CSaveHelper obj;
		obj.Save(this, dlg.GetPathName());

	}
}


void CChildView::OnAppAbout()
{
	// TODO: 在此添加命令处理程序代码
	MessageBox(TEXT("一、本程序用于进行简单的画图操作。\n二、您可以通过打开和另存为功能实现对JPG和BMP格式文件进行打开操作以及保存当前界面为JPG和BMP。通过选择各种图形可以进行简单图形的创建与编辑，同时通过选择橡皮工具进行擦除。本程序提供绘制颜色的快捷方式以及画笔RGB及粗度设置。\n三、本程序为贲清炜同学OOP大作业，感谢大家的使用~"), TEXT("程序说明"), MB_OK);
}
