// MyToolView.cpp : 구현 파일입니다.
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


// MyToolView 그리기입니다.

void MyToolView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// MyToolView 진단입니다.

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


// MyToolView 메시지 처리기입니다.
BOOL MyToolView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.


	BOOL Result = CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

	MenuDlg.Create(IDD_TOOLMENUDIALOG, this);		// 대화상자만들기~ 룰루~
	MenuDlg.ShowWindow(SW_SHOW);					// 윈도우창에다 띄우기~

	return Result;
}