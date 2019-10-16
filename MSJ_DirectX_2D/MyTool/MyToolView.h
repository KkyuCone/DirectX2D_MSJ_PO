#pragma once
#include "ToolMenuDialog.h"


// ��������Ʈ �� -> �ؽ�ó ���� �ȿ� �ִ� ��� �ؽ�ó�� �ε��ϴ� ���.
// ���� -> ������Ʈ��ġ�� ��Ÿ�� ��ġ
// �ִϸ��̼��� �����ϴ� ��� -> �����.

// MyToolView ���Դϴ�.

class MyToolView : public CView
{
	DECLARE_DYNCREATE(MyToolView)

private:
	ToolMenuDialog MenuDlg;

protected:
	MyToolView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~MyToolView();

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
};


