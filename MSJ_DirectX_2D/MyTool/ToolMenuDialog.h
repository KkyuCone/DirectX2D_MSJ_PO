#pragma once
#include "stdafx.h"
#include <vector>
#include "TabDialog.h"

class DialogInfo
{
public:
	bool m_bScene;			// ���� �ִ��� ����
	CString Name;
	TabDialog* pDlg;		// �� ���̾�α� 

public:
	DialogInfo() : m_bScene(true), pDlg(nullptr)
	{

	}

	~DialogInfo() 
	{
	}
};


// ToolMenuDialog ��ȭ �����Դϴ�.

class ToolMenuDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ToolMenuDialog)

public:
	ToolMenuDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~ToolMenuDialog();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOOLMENUDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();			// Ȯ�ι�ư Ŭ����
	afx_msg void OnBnClickedCancel();		// ��ҹ�ư Ŭ����

private:
	CTabCtrl m_Tab;
	std::vector<DialogInfo> m_TabInfo;

	virtual BOOL OnInitDialog();

public:
	// �� �̸��̶� �� �̸��� ���� �Ұ��� ������
	//void CreateTab(const wchar_t* _Name);


	// �� ü����
	void ChangeTab(const wchar_t* _Name);
	void ChangeTab(int _Index);

public:
	void AllDialogHide();

public:
	// �� �����ϱ� ( �����°�)
	void CreateTab(const wchar_t* _Name)
	{
		m_Tab.InsertItem((int)m_TabInfo.size(), _Name);

		DialogInfo NewInfo;
		NewInfo.m_bScene = false;
		NewInfo.Name = _Name;
		m_TabInfo.push_back(NewInfo);
	}

	// ������ ���� ����� ���̾�α� ����
	template<typename T>
	void CreateTab(const wchar_t* _Name, int _DialogID)
	{
		CreateTab<T>(_Name, true, _DialogID);
	}

	// ���� ���� ���� �ְ� �ȸ������ �ִ� ���̾�α� �����ϱ�
	template<typename T>
	void CreateTab(const wchar_t* _Name, bool _bScene, int _DialogID)
	{
		m_Tab.InsertItem((int)m_TabInfo.size(), _Name);

		My_PTR<Scene> NewScene = nullptr;		// ��

		// �� �����ϱ�
		if (true == _bScene)
		{
			NewScene = Scene::Scene_Create(_Name);
		}

		T* NewDialog = nullptr;

		if (-1 != _DialogID)
		{
			// ���̾�α� �����
			NewDialog = new T();
			NewDialog->SetScene(NewScene);			// ���־��ֱ�
			NewDialog->Create(_DialogID, &m_Tab);
			NewDialog->SetBackgroundColor(RGB(255, 255, 255), TRUE);
			NewDialog->ShowWindow(SW_HIDE);
		}

		// ���̾�α� ���� �߰� 
		DialogInfo NewInfo;
		NewInfo.m_bScene = _bScene;
		NewInfo.Name = _Name;
		NewInfo.pDlg = (TabDialog*)NewDialog;
		m_TabInfo.push_back(NewInfo);
	}

public:
	afx_msg void OnTcnSelchangeMenutab(NMHDR *pNMHDR, LRESULT *pResult);
};
