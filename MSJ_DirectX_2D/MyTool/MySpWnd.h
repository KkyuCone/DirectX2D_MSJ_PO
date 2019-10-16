#pragma once
class MySpWnd : public CSplitterWnd
{
private:
	bool m_bLock;

public:
	int BoderSizeY() { return m_cxBorder; }

public:
	void Lock()
	{
		m_bLock = true;			// 보여주기
	}

	void UnLock()
	{
		m_bLock = false;		// 보여주기 끄기
	}

// 메세지 함수 재설정
public:
	afx_msg void OnLButtonDown(UINT _nFlages, CPoint _Point);			// ON_WM_LBUTTONDOWN 때문에 사용해야함.. 테이블에 맞게써야됨


	// 메세지 맵 매크로(MFC) - 	DECLARE_MESSAGE_MAP()
	// 메세지 맵 (클래스 선언에 사용 해야함) 함수에 메세지를 매핑하기 위해서 클래스에서 사용될지를 선언한다.
	DECLARE_MESSAGE_MAP()		

public:
	MySpWnd();
	~MySpWnd();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};



// 메시지 맵 관련 매크로 -> http://elearning.kocw.net/document/MFC%20Message.pdf (설명)
// - 메시지 맵 - 메시지와 메시지 핸들러를 매핑하는 테이블
// - MFC에서 제공하는 매핑 매크로(3가지 유형)


