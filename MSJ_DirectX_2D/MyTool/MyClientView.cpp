#include "stdafx.h"
#include "MyTool.h"
#include "MyClientView.h"

// DECLARE_DYNCREATE ��ũ�ο� �Բ� ����ϸ� ��Ÿ�ӿ� CObject���� �Ļ��� Ŭ������ ��ü�� �������� ���� �� �ִ�.
// CView : ����� ���� �� Ŭ������ ���� �⺻ ��� ����
IMPLEMENT_DYNCREATE(MyClientView, CView)

MyClientView::MyClientView()
{
}


MyClientView::~MyClientView()
{
}

BEGIN_MESSAGE_MAP(MyClientView, CView)
END_MESSAGE_MAP()



// MyClientView�׸���
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
