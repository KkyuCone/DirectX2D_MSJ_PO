#pragma once
#include "TabDialog.h"
#include "afxwin.h"
#include <atlimage.h>
#include <list>
#include <Path.h>
#include <Texture.h>
#include <Sprite.h>
#include <SpriteRenderer2D.h>

#include <LandMapScript.h>


// MapDialog ��ȭ �����Դϴ�.


class ImageInfo
{
public:
	bool m_bFile;		// �������� ���� (������ false)
	bool m_bBigTile;
	CString m_OnlyPath;
	CString m_Path;
	CString m_FolderName;
	CString m_FileName;
};

class MapDialog : public TabDialog
{
	DECLARE_DYNAMIC(MapDialog)

public:
	MapDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~MapDialog();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

private:
	CTreeCtrl m_TileTree;
	CButton m_SlopeCehck;
	CButton m_SlimCheck;
	CButton m_DownSlopCheck;
	CImage* m_TilePreview;

	CEdit m_InputSaveName;

	CEdit m_SceneX;
	CEdit m_SceneY;

	CString m_SaveFileName;

	std::list<ImageInfo*> m_TileSpriteInfoList;
	//My_PTR<SpriteRenderer2D> m_TileRender;


	// ���� �߰��Ѱ�
	My_PTR<LandMapScript> m_LandMapOwner;

private:
	void ReSpriteFinder();

	void SetilePreview(std::wstring _ImagePath, size_t _Width, size_t _Height);


public:
	void SpriteFinder(const CString& _Folder, HTREEITEM _Parent);

	

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTvnSelchangedTiletreelist(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnDestroy();
	afx_msg void OnBnClickedSlope();
	afx_msg void OnBnClickedSlopdown();
	afx_msg void OnBnClickedSlim();
	afx_msg void OnBnClickedSave();
	afx_msg void OnStnClickedSavename();
	afx_msg void OnBnClickedScenesize();
};
