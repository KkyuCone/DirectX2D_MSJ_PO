#include "stdafx.h"
#include "MyTool.h"
#include "MyClientView.h"

// DECLARE_DYNCREATE 매크로와 함께 사용하면 런타임에 CObject에서 파생된 클래스의 개체를 동적으로 만들 수 있다.
// CView : 사용자 정의 뷰 클래스에 대한 기본 기능 제공
IMPLEMENT_DYNCREATE(MyClientView, CView)

MyClientView::MyClientView()
{
}


MyClientView::~MyClientView()
{
}

BEGIN_MESSAGE_MAP(MyClientView, CView)
END_MESSAGE_MAP()



// MyClientView그리기
void MyClientView::OnDraw(CDC* _pDC)
{
	CDocument* pDC = GetDocument();
}

#ifdef _DEBUG
void MyClientView::AssertValid() const
{
	CView::AssertValid();
}
#ifndef WIN32_WCE
void MyClientView::Dump(CDumpContext& _DC) const
{
	CView::Dump(_DC);
}

#endif // !WIN32_WCE

#endif // _DEBUG
