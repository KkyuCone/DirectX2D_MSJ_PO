#pragma once
#include "TabDialog.h"
#include "stdafx.h"
#include <list>
#include <Path.h>
#include <Texture.h>
#include <Sprite.h>
#include <SpriteRenderer2D.h>


class SpriteInfo
{
public:
	bool m_bFile;		// 파일인지 여부 (폴더면 false)
	CString m_OnlyPath;
	CString m_Path;
	CString m_FileName;
};


// SpriteDialog 대화 상자입니다.

class SpriteDialog : public TabDialog
{
	DECLARE_DYNAMIC(SpriteDialog)

public:
	SpriteDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~SpriteDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SPRITEDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	CTreeCtrl m_SpriteTree;
	std::list<SpriteInfo*> m_TileSpriteInfoList;
	My_PTR<SpriteRenderer2D> m_BackRender;

	// 예비용
	My_PTR<SpriteRenderer2D> m_BackRender1;
	My_PTR<SpriteRenderer2D> m_BackRender2;
	My_PTR<SpriteRenderer2D> m_BackRender3;
	My_PTR<SpriteRenderer2D> m_BackRender4;

public:
	void SpriteFinder(const CString& _Folder, HTREEITEM _Parent);

public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnTvnSelchangedSpritetree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
};
