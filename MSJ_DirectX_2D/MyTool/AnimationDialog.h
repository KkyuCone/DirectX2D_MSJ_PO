#pragma once
#include "TabDialog.h"


// AnimationDialog ��ȭ �����Դϴ�.

class AnimationDialog : public TabDialog
{
	DECLARE_DYNAMIC(AnimationDialog)

public:
	AnimationDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~AnimationDialog();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
