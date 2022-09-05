// ChooseClr.cpp: 实现文件
//

#include "pch.h"
#include "Drawer.h"
#include "ChooseClr.h"
#include "afxdialogex.h"


// ChooseClr 对话框

IMPLEMENT_DYNAMIC(ChooseClr, CDialogEx)

ChooseClr::ChooseClr(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIYCLOR, pParent)
	, blueVal(_T(""))
	, greenVal(_T(""))
	, redVal(_T(""))
{

}

ChooseClr::~ChooseClr()
{
}

void ChooseClr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_BLUval, blueVal);
	DDX_Text(pDX, IDC_GRNval, greenVal);
	DDX_Text(pDX, IDC_REDval, redVal);
}


BEGIN_MESSAGE_MAP(ChooseClr, CDialogEx)
END_MESSAGE_MAP()


// ChooseClr 消息处理程序
