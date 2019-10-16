// ToolMenuDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyTool.h"
#include "ToolMenuDialog.h"
#include "afxdialogex.h"
#include <Scene.h>
#include "MapDialog.h"
#include "AnimationDialog.h"
#include "SpriteDialog.h"


// ToolMenuDialog ��ȭ �����Դϴ�.

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
			delete m_TabInfo[i].pDlg;		// ���̾�α� ������ �ִٸ� ����
		}
	}
}

void ToolMenuDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MENUTAB, m_Tab);

#pragma region DDX_Control ����
	//���ҽ� �����ͷ� ���� ������ ��ȭ ���ڸ� �������� �װ��� ����Ϸ��� �� �ڿ����� ������ ������ ������Ѵ�. ������
	//�̰� �������ִ� �׸��� DoDataExchange()�ε� �� �Լ� �ȿ� DDX_�迭�� �Լ��� ����ؼ� ��ũ��������Ѵ�.
#pragma endregion

}


BEGIN_MESSAGE_MAP(ToolMenuDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &ToolMenuDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ToolMenuDialog::OnBnClickedCancel)
	ON_NOTIFY(TCN_SELCHANGE, IDC_MENUTAB, &ToolMenuDialog::OnTcnSelchangeMenutab)
END_MESSAGE_MAP()


// ToolMenuDialog �޽��� ó�����Դϴ�.


void ToolMenuDialog::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//CDialogEx::OnOK();
}


void ToolMenuDialog::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//CDialogEx::OnCancel();
}


void ToolMenuDialog::ChangeTab(const wchar_t* _Name)
{
	// �̸������� �� �ٲ��ֱ�
	for (int i = 0; i < (int)(m_TabInfo.size()); ++i)
	{
		if (m_TabInfo[i].Name == _Name)
		{
			m_Tab.SetCurSel(i);
			if (true == m_TabInfo[i].m_bScene)
			{
				// ����.. �°� �ٲ��ֱ� 
				Scene::Scene_Change(m_TabInfo[i].Name.GetString());

				// ���̾�αװ� �ִٸ�
				if (nullptr != m_TabInfo[i].pDlg)
				{
					RECT RC;

					m_Tab.GetWindowRect(&RC);		// �簢����������(ũ��)
					m_Tab.ScreenToClient(&RC);			// Ŭ���̾�Ʈ ȭ��ũ��..(?)��������

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
	// �ε����� ���� �� �ٲ��ֱ�
	m_Tab.SetCurSel(_Index);

	if (true == m_TabInfo[_Index].m_bScene)
	{
		// ���� �ִٸ�~ ��ü����~
		Scene::Scene_Change(m_TabInfo[_Index].Name.GetString());
	}

	// ���̾�αװ� �ִٸ�
	if (nullptr != m_TabInfo[_Index].pDlg)
	{
		RECT RC;

		m_Tab.GetWindowRect(&RC);		// �簢����������(ũ��)
		m_Tab.ScreenToClient(&RC);			// Ŭ���̾�Ʈ ȭ��ũ��..(?)��������

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
	// ���̾�α� ���� ������(�Ⱥ��̰��ϱ�)
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
//	// �� �����
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

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_TabInfo.reserve(3);			// �� 3�� ���ٰŴϱ�.. �̸� �Ҵ�


									// �� �Ǹ��� ��� ���� �������� ����.
									// ������ ��� �ǰ� �Ұ� ������..
	CreateTab<MapDialog>(L"Map", IDD_MAPDLG);
	CreateTab<AnimationDialog>(L"Animation", IDD_ANIDLG);
	CreateTab<SpriteDialog>(L"Sprite", IDD_SPRITEDLG);

	ChangeTab(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

// �� �������� ������
void ToolMenuDialog::OnTcnSelchangeMenutab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	// ���õȰ� ������ -1�� ����,.. (��� Ŭ���� �ش� �ǹ�ȣ�� ��������)
	int SelectIndex = m_Tab.GetCurSel();		

	if (SelectIndex == -1)
	{
		return;
	}

	ChangeTab(SelectIndex);
}
