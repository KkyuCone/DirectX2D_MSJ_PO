#pragma once
#include "stdafx.h"
#include <vector>
#include "TabDialog.h"

class DialogInfo
{
public:
	bool m_bScene;			// 씬이 있는지 여부
	CString Name;
	TabDialog* pDlg;		// 탭 다이얼로그 

public:
	DialogInfo() : m_bScene(true), pDlg(nullptr)
	{

	}

	~DialogInfo() 
	{
	}
};


// ToolMenuDialog 대화 상자입니다.

class ToolMenuDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ToolMenuDialog)

public:
	ToolMenuDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~ToolMenuDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOOLMENUDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();			// 확인버튼 클릭시
	afx_msg void OnBnClickedCancel();		// 취소버튼 클릭시

private:
	CTabCtrl m_Tab;
	std::vector<DialogInfo> m_TabInfo;

	virtual BOOL OnInitDialog();

public:
	// 탭 이름이랑 씬 이름을 같게 할것임 ㅇㅅㅇ
	//void CreateTab(const wchar_t* _Name);


	// 탭 체인지
	void ChangeTab(const wchar_t* _Name);
	void ChangeTab(int _Index);

public:
	void AllDialogHide();

public:
	// 탭 생성하기 ( 씬없는거)
	void CreateTab(const wchar_t* _Name)
	{
		m_Tab.InsertItem((int)m_TabInfo.size(), _Name);

		DialogInfo NewInfo;
		NewInfo.m_bScene = false;
		NewInfo.Name = _Name;
		m_TabInfo.push_back(NewInfo);
	}

	// 무조건 씬을 만드는 다이얼로그 생성
	template<typename T>
	void CreateTab(const wchar_t* _Name, int _DialogID)
	{
		CreateTab<T>(_Name, true, _DialogID);
	}

	// 씬을 만들 수도 있고 안만들수도 있는 다이얼로그 생성하기
	template<typename T>
	void CreateTab(const wchar_t* _Name, bool _bScene, int _DialogID)
	{
		m_Tab.InsertItem((int)m_TabInfo.size(), _Name);

		My_PTR<Scene> NewScene = nullptr;		// 씬

		// 씬 생성하기
		if (true == _bScene)
		{
			NewScene = Scene::Scene_Create(_Name);
		}

		T* NewDialog = nullptr;

		if (-1 != _DialogID)
		{
			// 다이얼로그 만들기
			NewDialog = new T();
			NewDialog->SetScene(NewScene);			// 씬넣어주기
			NewDialog->Create(_DialogID, &m_Tab);
			NewDialog->SetBackgroundColor(RGB(255, 255, 255), TRUE);
			NewDialog->ShowWindow(SW_HIDE);
		}

		// 다이얼로그 정보 추가 
		DialogInfo NewInfo;
		NewInfo.m_bScene = _bScene;
		NewInfo.Name = _Name;
		NewInfo.pDlg = (TabDialog*)NewDialog;
		m_TabInfo.push_back(NewInfo);
	}

public:
	afx_msg void OnTcnSelchangeMenutab(NMHDR *pNMHDR, LRESULT *pResult);
};
