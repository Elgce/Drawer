
// ChildView.h: CChildView 类的接口
//


#pragma once


// CChildView 窗口

class CChildView : public CWnd
{
// 构造
public:
	CChildView();

// 特性
public:
	CString BmpName;//打开的bmp/jpg文件的文件名
	CString EntName;//打开的bmp/jpg文件的拓展名
	CBitmap m_bitmap;//用于将打开的图像文件绘制到程序中
	CPoint p;//用于储存鼠标按下时所在的坐标点位置
	CPoint term;//用于储存鼠标上次移动时所在的标点位置
	CPoint mid;//用于记录任意三角形第二个顶点的位置
	
	CPtrArray ptr;
	CPen cho_pen[100];//记录每一个使用过的笔，方便重绘时调用
	COLORREF pen_color;
	CPen w_cho_pen; //用于橡皮绘制的笔

	CDC bufferdc;
	CBitmap bufferbmp;//用于绘制打开的图像文件

	int flag;//用于记录当前的绘图模式
	int do_flag;//用于判断当前
	int trnum;//记录当前任意三角形已经确定顶点的个数
	int a; 
	int pen_size; //当前画笔的宽度
	int number;//记录已绘制的图像个数
// 操作
public:
	void showBitmap(CDC* pDC, CString BmpName);//展示bmp类型的文件
	BOOL showJpgmap(CDC* pDC, CString strPath);//展示jpg类型的文件
	BOOL CreatePen(int pensize, COLORREF pencolor);//创造新的画笔
// 重写
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);//MFC绘制窗口函数
public:
	virtual void OnDraw(CDC* pDC);
	virtual BOOL CChildView::OnEraseBkgnd(CDC* pDC);//重构OnEraseBkgnd函数
// 实现
public:
	virtual ~CChildView();

	// 生成的消息映射函数
protected:
	afx_msg void OnPaint();//重构OnPaint函数
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLine();
	afx_msg void OnRectangle();
	afx_msg void OnOval();
	afx_msg void OnRed();
	afx_msg void OnBlack();
	afx_msg void OnBlue();
	afx_msg void OnGreen();
	afx_msg void OnChooseRgb();
	afx_msg void OnPencil();
	afx_msg void OnEraser();
	afx_msg void OnCancel();
	afx_msg void OnTtriangle();
	afx_msg void OnDtriangle();
	afx_msg void OnChwidth();
	afx_msg void OnFileOpen();
	afx_msg void OnSave();
	afx_msg void OnAppAbout();//此处分别为点击各按钮的命令响应函数
};
class CChildDoc;