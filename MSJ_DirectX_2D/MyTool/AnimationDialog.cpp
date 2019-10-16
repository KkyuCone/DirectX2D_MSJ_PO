// AnimationDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyTool.h"
#include "AnimationDialog.h"
#include "afxdialogex.h"


// AnimationDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(AnimationDialog, TabDialog)

AnimationDialog::AnimationDialog(CWnd* pParent /*=NULL*/)
	: TabDialog(IDD_ANIDLG, pParent)
{

}

AnimationDialog::~AnimationDialog()
{
}

void AnimationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AnimationDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &AnimationDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &AnimationDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// AnimationDialog 메시지 처리기입니다.


void AnimationDialog::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
}


void AnimationDialog::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnCancel();
}


BOOL AnimationDialog::OnInitDialog()
{
	TabDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	My_PTR<GameObject> NewGmaeObject = m_TabScene->GameObject_Create(L"Camera");
	My_PTR<Camera> CamPTR = NewGmaeObject->Component_Add<Camera>();
	CamPTR->SettingViewGroup(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
	CamPTR->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -100.0f));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
