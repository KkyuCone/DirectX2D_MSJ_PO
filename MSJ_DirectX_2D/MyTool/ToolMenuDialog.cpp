// ToolMenuDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyTool.h"
#include "ToolMenuDialog.h"
#include "afxdialogex.h"
#include <Scene.h>
#include "MapDialog.h"
#include "AnimationDialog.h"
#include "SpriteDialog.h"


// ToolMenuDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(ToolMenuDialog, CDialogEx)

ToolMenuDialog::ToolMenuDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TOOLMENUDIALOG, pParent)
{

}

ToolMenuDialog::~ToolMenuDialog()
{
	for (size_t i = 0; i < m_TabInfo.size(); ++i)
	{
		if (nullptr != m_TabInfo[i].pDlg)
		{
			delete m_TabInfo[i].pDlg;		// 다이얼로그 정보다 있다면 삭제
		}
	}
}

void ToolMenuDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MENUTAB, m_Tab);

#pragma region DDX_Control 설명
	//리소스 에디터로 여러 종류의 대화 상자를 만들어놓고 그것을 사용하려면 그 자원들을 변수와 연결을 해줘야한다. ㅇㅅㅇ
	//이걸 지정해주는 항목이 DoDataExchange()인데 이 함수 안에 DDX_계열의 함수를 사용해서 링크시켜줘야한다.
#pragma endregion

}


BEGIN_MESSAGE_MAP(ToolMenuDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &ToolMenuDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ToolMenuDialog::OnBnClickedCancel)
	ON_NOTIFY(TCN_SELCHANGE, IDC_MENUTAB, &ToolMenuDialog::OnTcnSelchangeMenutab)
END_MESSAGE_MAP()


// ToolMenuDialog 메시지 처리기입니다.


void ToolMenuDialog::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
}


void ToolMenuDialog::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnCancel();
}


void ToolMenuDialog::ChangeTab(const wchar_t* _Name)
{
	// 이름에따른 탭 바꿔주기
	for (int i = 0; i < (int)(m_TabInfo.size()); ++i)
	{
		if (m_TabInfo[i].Name == _Name)
		{
			m_Tab.SetCurSel(i);
			if (true == m_TabInfo[i].m_bScene)
			{
				// 씬도.. 맞게 바꿔주기 
				Scene::Scene_Change(m_TabInfo[i].Name.GetString());

				// 다이얼로그가 있다면
				if (nullptr != m_TabInfo[i].pDlg)
				{
					RECT RC;

					m_Tab.GetWindowRect(&RC);		// 사각형가져오기(크기)
					m_Tab.ScreenToClient(&RC);			// 클라이언트 화면크기..(?)가져오기

					AllDialogHide();

					m_TabInfo[i].pDlg->SetWindowPos(nullptr, 2, 22, RC.right - (2 * 2), RC.bottom - (26), SWP_NOZORDER);
					//m_TabInfo[_Index].pDlg->SetWindowPos(nullptr, 100, 100, RC.right - (10 * 10), RC.bottom - (26), SWP_NOZORDER);
					m_TabInfo[i].pDlg->ShowWindow(SW_SHOWDEFAULT);
				}
				else
				{
					AllDialogHide();
				}
			}
		}
	}
}

void ToolMenuDialog::ChangeTab(int _Index)
{
	// 인덱스에 따른 탭 바꿔주기
	m_Tab.SetCurSel(_Index);

	if (true == m_TabInfo[_Index].m_bScene)
	{
		// 씬이 있다면~ 씬체인지~
		Scene::Scene_Change(m_TabInfo[_Index].Name.GetString());
	}

	// 다이얼로그가 있다면
	if (nullptr != m_TabInfo[_Index].pDlg)
	{
		RECT RC;

		m_Tab.GetWindowRect(&RC);		// 사각형가져오기(크기)
		m_Tab.ScreenToClient(&RC);			// 클라이언트 화면크기..(?)가져오기

		AllDialogHide();

		m_TabInfo[_Index].pDlg->SetWindowPos(nullptr, 2, 22, RC.right - (2 * 2), RC.bottom - (26), SWP_NOZORDER);
		//m_TabInfo[_Index].pDlg->SetWindowPos(nullptr, 100, 100, RC.right - (10 * 10), RC.bottom - (26), SWP_NOZORDER);
		m_TabInfo[_Index].pDlg->ShowWindow(SW_SHOWDEFAULT);
	}
	else
	{
		AllDialogHide();
	}
}

void ToolMenuDialog::AllDialogHide()
{
	// 다이얼로그 전부 가리기(안보이게하기)
	for (int i = 0; i < (int)(m_TabInfo.size()); ++i)
	{
		if (nullptr != m_TabInfo[i].pDlg)
		{
			m_TabInfo[i].pDlg->ShowWindow(SW_HIDE);
		}
	}
}


//void ToolMenuDialog::CreateTab(const wchar_t* _Name)
//{
//	// 탭 만들기
//	m_Tab.InsertItem(0, _Name);
//	Scene::Scene_Create(_Name);
//
//	DialogInfo NewInfo;
//	NewInfo.Name = _Name;
//
//	m_TabInfo.push_back(NewInfo);
//}


BOOL ToolMenuDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_TabInfo.reserve(3);			// 씬 3개 해줄거니까.. 미리 할당


									// 각 탭마다 담당 씬을 설정해줄 것임.
									// 담당씬이 없어도 되게 할거 ㅇㅅㅇ..
	CreateTab<MapDialog>(L"Map", IDD_MAPDLG);
	CreateTab<AnimationDialog>(L"Animation", IDD_ANIDLG);
	CreateTab<SpriteDialog>(L"Sprite", IDD_SPRITEDLG);

	ChangeTab(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

// 탭 눌렀을때 ㅇㅅㅇ
void ToolMenuDialog::OnTcnSelchangeMenutab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	// 선택된게 없으면 -1을 리턴,.. (얘는 클릭시 해당 탭번호를 리턴해줌)
	int SelectIndex = m_Tab.GetCurSel();		

	if (SelectIndex == -1)
	{
		return;
	}

	ChangeTab(SelectIndex);
}
