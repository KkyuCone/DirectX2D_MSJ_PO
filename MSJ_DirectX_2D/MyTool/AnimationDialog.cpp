// AnimationDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyTool.h"
#include "AnimationDialog.h"
#include "afxdialogex.h"


// AnimationDialog ��ȭ �����Դϴ�.

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


// AnimationDialog �޽��� ó�����Դϴ�.


void AnimationDialog::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//CDialogEx::OnOK();
}


void AnimationDialog::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//CDialogEx::OnCancel();
}


BOOL AnimationDialog::OnInitDialog()
{
	TabDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	My_PTR<GameObject> NewGmaeObject = m_TabScene->GameObject_Create(L"Camera");
	My_PTR<Camera> CamPTR = NewGmaeObject->Component_Add<Camera>();
	CamPTR->SettingViewGroup(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
	CamPTR->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -100.0f));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
