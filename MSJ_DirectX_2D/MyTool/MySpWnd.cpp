#include "stdafx.h"
#include "MySpWnd.h"



MySpWnd::MySpWnd() : m_bLock(false)
{
}


MySpWnd::~MySpWnd()
{
}

// CSplitterWnd 클래스
// 여러 개의 창이 포함된 창인 분할자 창 기능을 제공함
// BEGIN_MESSAGE_MAP( A, B)   A는 부모클래스명, B는 파생 클래스 명
BEGIN_MESSAGE_MAP(MySpWnd, CSplitterWnd)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



void MySpWnd::OnLButtonDown(UINT _nFlages, CPoint _Point)
{
	if (false == m_bLock)
	{
		CSplitterWnd::OnLButtonDown(_nFlages, _Point);
	}
}

BOOL MySpWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// 꽉차게끔..
	m_cxBorder = 1;
	m_cyBorder = 1;
	/*
	m_cxSplitter = 5;
	m_cySplitter = 5;

	m_cxBorder = 5;
	m_cyBorder = 5;

	m_cxSplitterGap = 1;
	m_cySplitterGap = 1;*/

	return CSplitterWnd::PreCreateWindow(cs);
}
