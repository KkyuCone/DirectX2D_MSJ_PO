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
		m_bLock = true;			// �����ֱ�
	}

	void UnLock()
	{
		m_bLock = false;		// �����ֱ� ����
	}

// �޼��� �Լ� �缳��
public:
	afx_msg void OnLButtonDown(UINT _nFlages, CPoint _Point);			// ON_WM_LBUTTONDOWN ������ ����ؾ���.. ���̺� �°Խ�ߵ�


	// �޼��� �� ��ũ��(MFC) - 	DECLARE_MESSAGE_MAP()
	// �޼��� �� (Ŭ���� ���� ��� �ؾ���) �Լ��� �޼����� �����ϱ� ���ؼ� Ŭ�������� �������� �����Ѵ�.
	DECLARE_MESSAGE_MAP()		

public:
	MySpWnd();
	~MySpWnd();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};



// �޽��� �� ���� ��ũ�� -> http://elearning.kocw.net/document/MFC%20Message.pdf (����)
// - �޽��� �� - �޽����� �޽��� �ڵ鷯�� �����ϴ� ���̺�
// - MFC���� �����ϴ� ���� ��ũ��(3���� ����)


