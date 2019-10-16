#pragma once


class MyClientView : public CView
{
	// DECLARE_DYNCREATE(클래스이름) : 동적으로 만드는것을 할수있게 해줌. 동적으로 만들고 싶을때 설정해줘야한다.
	DECLARE_DYNCREATE(MyClientView);

protected:
	MyClientView();				// 이제부터 이 생성장는 동적만들기로 사용하게됨
	virtual ~MyClientView();


public:
	virtual void OnDraw(CDC* _pDC);				// 이 뷰를 그리기 위해 재정의 된다.

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef  _WIN32_WCE
	virtual void Dump(CDumpContext& _DC) const;
#endif // ! _WIN32_WCE

#endif // _DEBUG

	
	// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
};

