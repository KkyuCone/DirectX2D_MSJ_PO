#pragma once
#include "TabDialog.h"


// AnimationDialog 대화 상자입니다.

class AnimationDialog : public TabDialog
{
	DECLARE_DYNAMIC(AnimationDialog)

public:
	AnimationDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~AnimationDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
