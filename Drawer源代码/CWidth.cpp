// CWidth.cpp: 实现文件
//

#include "pch.h"
#include "Drawer.h"
#include "CWidth.h"
#include "afxdialogex.h"


// CWidth 对话框

IMPLEMENT_DYNAMIC(CWidth, CDialogEx)

CWidth::CWidth(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CWidth::~CWidth()
{
}

void CWidth::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CWIDTH, width);
}


BEGIN_MESSAGE_MAP(CWidth, CDialogEx)
END_MESSAGE_MAP()


// CWidth 消息处理程序
