// MyToolView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyTool.h"
#include "MyToolView.h"


// MyToolView

IMPLEMENT_DYNCREATE(MyToolView, CView)

MyToolView::MyToolView()
{

}

MyToolView::~MyToolView()
{
}

BEGIN_MESSAGE_MAP(MyToolView, CView)
END_MESSAGE_MAP()


// MyToolView �׸����Դϴ�.

void MyToolView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


// MyToolView �����Դϴ�.

#ifdef _DEBUG
void MyToolView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void MyToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// MyToolView �޽��� ó�����Դϴ�.
BOOL MyToolView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.


	BOOL Result = CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

	MenuDlg.Create(IDD_TOOLMENUDIALOG, this);		// ��ȭ���ڸ����~ ���~
	MenuDlg.ShowWindow(SW_SHOW);					// ������â���� ����~

	return Result;
}