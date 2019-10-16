#include "stdafx.h"
#include "MySpWnd.h"



MySpWnd::MySpWnd() : m_bLock(false)
{
}


MySpWnd::~MySpWnd()
{
}

// CSplitterWnd Ŭ����
// ���� ���� â�� ���Ե� â�� ������ â ����� ������
// BEGIN_MESSAGE_MAP( A, B)   A�� �θ�Ŭ������, B�� �Ļ� Ŭ���� ��
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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	// �����Բ�..
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
