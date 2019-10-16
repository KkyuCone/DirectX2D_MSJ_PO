#pragma once
#include "ToolMenuDialog.h"


// 스프라이트 툴 -> 텍스처 폴더 안에 있는 모든 텍스처를 로드하는 기능.
// 맵툴 -> 오브젝트배치와 맵타일 배치
// 애니메이션을 편집하는 기능 -> 만든다.

// MyToolView 뷰입니다.

class MyToolView : public CView
{
	DECLARE_DYNCREATE(MyToolView)

private:
	ToolMenuDialog MenuDlg;

protected:
	MyToolView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~MyToolView();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
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


