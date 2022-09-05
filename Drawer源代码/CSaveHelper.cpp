#include "pch.h"
#include "CSaveHelper.h"
using namespace std;
int CSaveHelper::Save(CWnd* pWnd, CString filePath)
{
	CClientDC dc(pWnd);
	CRect rect;
	GetClientRect(pWnd->GetSafeHwnd(), &rect);//��ȡ������С
	HBITMAP hbitmap = CreateCompatibleBitmap(dc, rect.right - rect.left, rect.bottom - rect.top);//��������λͼ
	HDC hdc = CreateCompatibleDC(dc);
	HBITMAP hOldMap = (HBITMAP)SelectObject(hdc, hbitmap);
	BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, dc, 0, 0, SRCCOPY);
	CImage image;
	image.Attach(hbitmap);

	HRESULT hResult = image.Save(filePath);
	if (FAILED(hResult))
	{
		return -1;
	}
	else return 1;
	image.Detach();
	SelectObject(hdc, hOldMap);
	return 0;
}