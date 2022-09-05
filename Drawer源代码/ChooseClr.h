#pragma once


// ChooseClr 对话框

class ChooseClr : public CDialogEx
{
	DECLARE_DYNAMIC(ChooseClr)

public:
	ChooseClr(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ChooseClr();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIYCLOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString blueVal;
	CString greenVal;
	CString redVal;
};
