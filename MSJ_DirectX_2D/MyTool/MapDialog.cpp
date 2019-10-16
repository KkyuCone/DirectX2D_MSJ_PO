// MapDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyTool.h"
#include "MapDialog.h"
#include "afxdialogex.h"
#include <Input.h>


// MapDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(MapDialog, TabDialog)

MapDialog::MapDialog(CWnd* pParent /*=NULL*/)
	: TabDialog(IDD_MAPDLG, pParent), m_TilePreview(nullptr)
{

}

MapDialog::~MapDialog()
{
	if (nullptr != m_TilePreview)
	{
		delete m_TilePreview;
	}
}

void MapDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	/*DDX_Control(pDX, IDC_TILELISTBOX, m_TileLIst);*/
	DDX_Control(pDX, IDC_TILETREELIST, m_TileTree);
	DDX_Control(pDX, IDC_SLOPE, m_SlopeCehck);
	DDX_Control(pDX, IDC_SLIM, m_SlimCheck);
	DDX_Control(pDX, IDC_SLOPDOWN, m_DownSlopCheck);
	DDX_Control(pDX, IDC_INPUTSAVENAME, m_InputSaveName);
	DDX_Control(pDX, IDC_SCENEX, m_SceneX);
	DDX_Control(pDX, IDC_SCENEY, m_SceneY);
}


BEGIN_MESSAGE_MAP(MapDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &MapDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &MapDialog::OnBnClickedCancel)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TILETREELIST, &MapDialog::OnTvnSelchangedTiletreelist)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_SLOPE, &MapDialog::OnBnClickedSlope)
	ON_BN_CLICKED(IDC_SLOPDOWN, &MapDialog::OnBnClickedSlopdown)
	ON_BN_CLICKED(IDC_SLIM, &MapDialog::OnBnClickedSlim)
	ON_BN_CLICKED(IDC_SAVE, &MapDialog::OnBnClickedSave)
	ON_STN_CLICKED(IDC_SAVENAME, &MapDialog::OnStnClickedSavename)
	ON_BN_CLICKED(IDC_SCENESIZE, &MapDialog::OnBnClickedScenesize)
	ON_WM_MOUSEHWHEEL()
END_MESSAGE_MAP()


// MapDialog �޽��� ó�����Դϴ�.


void MapDialog::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//CDialogEx::OnOK();
}


void MapDialog::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//CDialogEx::OnCancel();
}


BOOL MapDialog::OnInitDialog()
{
	TabDialog::OnInitDialog();
	ReSpriteFinder();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void MapDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	TabDialog::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	m_LandMapOwner = m_TabScene->Script_Create<LandMapScript>();
}


// Ʈ������
void MapDialog::OnTvnSelchangedTiletreelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	//
	DWORD_PTR ItemPTR = m_TileTree.GetItemData(pNMTreeView->itemNew.hItem);
	ImageInfo* SpriteInfo_PTR = (ImageInfo*)ItemPTR;


	if (nullptr == SpriteInfo_PTR)
	{
		return;
	}

	// ������ �ƴ϶�� (����)
	if (false == SpriteInfo_PTR->m_bFile)
	{
		return;
	}

	My_PTR<Sprite> LoadSprite = ResourceManager<Sprite>::Find(SpriteInfo_PTR->m_FileName.GetString());
	My_PTR<Texture> LoadTexture = ResourceManager<Texture>::Find(SpriteInfo_PTR->m_FileName.GetString());

	if (nullptr == LoadSprite)
	{
		// ���ϸ���� �����ϱ�..(�ȳ־)
		LoadTexture = ResourceManager<Texture>::Load(SpriteInfo_PTR->m_Path.GetString());
		LoadSprite = ResourceManager<Sprite>::Create(SpriteInfo_PTR->m_FileName.GetString(), SpriteInfo_PTR->m_FileName.GetString());
	}


	// ���⼭ ���� ���� �����ؾ���.. �����߸��Ǹ� ���� -��-;;
	m_LandMapOwner->SetFileName(SpriteInfo_PTR->m_FileName.GetString());
	m_LandMapOwner->SetBigTile(SpriteInfo_PTR->m_bBigTile);							// ūŸ��/����Ÿ�� ���� ����
	m_LandMapOwner->SetSlimCheck(m_SlimCheck.GetCheck());							// ���� Ÿ�� üũ
	m_LandMapOwner->SetSlopCheck(m_SlopeCehck.GetCheck());							// ��� üũ
	m_LandMapOwner->SetDownSlopCheck(m_DownSlopCheck.GetCheck());					// �ϴ� ��� Ÿ�� üũ
	m_LandMapOwner->SetFolderName(SpriteInfo_PTR->m_FolderName.GetString());		// ���� ����

	// �̸�����
	std::wstring FolderPath = Path::FindPath(L"MapTile");
	std::wstring PreForlderName = SpriteInfo_PTR->m_FolderName.GetString();
	std::wstring PreFileName = SpriteInfo_PTR->m_FileName.GetString();

	std::wstring ALLPathName = FolderPath + PreForlderName + L"\\" + PreFileName;

	SetilePreview(ALLPathName, 90, 40);

	UpdateData(FALSE);

}

//
void MapDialog::ReSpriteFinder()
{
	//TabDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	//������ �̹�����..
	m_TileTree.DeleteAllItems();

	std::wstring TileName = Path::FindPath(L"MapTile");
	CString TilePath = TileName.c_str();

	HTREEITEM m_Item = m_TileTree.InsertItem(L"MapTile");
	SpriteFinder(TilePath, m_Item);		// ã�� 

	m_TileTree.Expand(m_Item, TVE_EXPAND);  // Ȯ���ϱ� ( �ٷ� �ε����ִ°� ������ )
}

// �߰�
void MapDialog::SpriteFinder(const CString & _Folder, HTREEITEM _Parent)
{
	// ����ã��~
	CFileFind FileFinder;

	// *.* = *(�����̸�).*(Ȯ����) ��� �����̸��� ��� Ȯ���� ã��
	// . (�ڱ��ڽ�), ..(�θ�)
	BOOL IsFile = FileFinder.FindFile(_Folder + L"*.*");

	while (TRUE == IsFile)
	{
		IsFile = FileFinder.FindNextFileW();

		if (FileFinder.IsDots())
		{
			// �ڱ� �ڽ�
			continue;
		}
		else if (TRUE == FileFinder.IsDirectory())
		{
			// ���丮�� ���ִٸ�..(������ �� �ִٸ�..)
			std::wstring FolderName = FileFinder.GetFileName();

			// ���� �׸����ִ� Ÿ�ϸ� �����ϱ� (�˻�)

			HTREEITEM FolderItem = m_TileTree.InsertItem(FolderName.c_str(), _Parent);
			SpriteFinder(_Folder + FileFinder.GetFileName() + L"\\", FolderItem);

			//
			ImageInfo* NewSpriteInfo = new ImageInfo();
			NewSpriteInfo->m_bFile = false;		// �� �����̾ƴϰ� �����ϱ�..
			DWORD_PTR PTR = (DWORD_PTR)NewSpriteInfo;
			m_TileTree.SetItemData(FolderItem, PTR);

			m_TileSpriteInfoList.push_back(NewSpriteInfo);
		}
		else
		{
			// Ÿ�� ����
			std::wstring FolderName = FileFinder.GetFileName();

			// ���� �׸����ִ� Ÿ�ϸ� �����ϱ� (�˻�)
			if (1 == m_SlimCheck.GetCheck())
			{
				// ���� Ÿ���̶�� ����Ÿ�ϸ� ��Ͽ� �ְ� �Ѵ�.
				if (std::wstring::npos == FolderName.find(L"_enH0."))
				{
					continue;
				}
			}
			else
			{
				if (std::wstring::npos == FolderName.find(L"_bsc.") && std::wstring::npos == FolderName.find(L"_edU."))
				{
					continue;
				}
			}

			HTREEITEM FileItem = m_TileTree.InsertItem(FolderName.c_str(), _Parent);

			// Ÿ�� �����ֱ� ����
			ImageInfo* NewSpriteInfo = new ImageInfo();
			NewSpriteInfo->m_bFile = true;		// �� ����
			NewSpriteInfo->m_Path = _Folder + FolderName.c_str();
			NewSpriteInfo->m_FileName = FolderName.c_str();
			NewSpriteInfo->m_OnlyPath = _Folder;

			std::wstring OnlyFolderName = FolderName.substr(0, FolderName.find(L"_"));
			NewSpriteInfo->m_FolderName = OnlyFolderName.c_str();

			if (std::wstring::npos != FolderName.find(L".img_edU."))
			{
				// ������ �ִ� ����Ÿ��
				NewSpriteInfo->m_bBigTile = false;
			}
			else
			{
				NewSpriteInfo->m_bBigTile = true;
			}

			DWORD_PTR PTR = (DWORD_PTR)NewSpriteInfo;
			m_TileTree.SetItemData(FileItem, PTR);

			m_TileSpriteInfoList.push_back(NewSpriteInfo);
		}
	}
}

void MapDialog::OnDestroy()
{
	// ���� (delete)
	std::list<ImageInfo*>::iterator Start = m_TileSpriteInfoList.begin();
	std::list<ImageInfo*>::iterator End = m_TileSpriteInfoList.end();

	for (; Start != End; ++Start)
	{
		if (nullptr != (*Start))
		{
			delete (*Start);
		}
	}

	TabDialog::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


// ���üũ
void MapDialog::OnBnClickedSlope()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (1 == m_SlimCheck.GetCheck())
	{
		// ���Ÿ���� ������ ��� ���� Ÿ���� ����� �� ����.
		m_SlimCheck.SetCheck(0);
	}

	ReSpriteFinder();		// üũ�� �Ŀ� �ٽ� ����� ��������Ѵ�. (�����Ҽ��ִ� �̹�����)
}


// �Ʒ��� ��Ÿ�� ��� üũ
void MapDialog::OnBnClickedSlopdown()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (1 == m_SlimCheck.GetCheck())
	{
		// ���Ÿ���� ������ ��� ���� Ÿ���� ����� �� ����.
		m_SlimCheck.SetCheck(0);
	}

	ReSpriteFinder();		// üũ�� �Ŀ� �ٽ� ����� ��������Ѵ�. (�����Ҽ��ִ� �̹�����)
}

// ����Ÿ�� üũ
void MapDialog::OnBnClickedSlim()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if ( 1 == m_SlopeCehck.GetCheck() || 1 == m_DownSlopCheck.GetCheck())
	{
		// ���� Ÿ���� ������ ���
		// ���Ÿ���� ����� ������.
		m_SlopeCehck.SetCheck(0);
		m_DownSlopCheck.SetCheck(0);
	}

	ReSpriteFinder();		// üũ�� �Ŀ� �ٽ� ����� ��������Ѵ�. (�����Ҽ��ִ� �̹�����)
}


void MapDialog::OnBnClickedSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_InputSaveName.GetWindowTextW(m_SaveFileName);
	m_LandMapOwner->ImageSave(m_SaveFileName.GetString());
}




// ������ Ÿ�� �̸�����
void MapDialog::SetilePreview(std::wstring _ImagePath, size_t _Width, size_t _Height)
{
	UpdateData(TRUE);

	if (nullptr != m_TilePreview)
	{
		delete m_TilePreview;
	}

	m_TilePreview = new CImage();

	m_TilePreview->Load(_ImagePath.c_str());

	CClientDC CurDC(this);
	Rectangle(CurDC, 269, 309, 431, 411);
	m_TilePreview->Draw(CurDC, RECT{ 270, 310 ,430, 410 });


	UpdateData(FALSE);
}


void MapDialog::OnStnClickedSavename()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}



void MapDialog::OnBnClickedScenesize()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString SizeX;
	CString SizeY;
	m_SceneX.GetWindowTextW(SizeX);
	m_SceneY.GetWindowTextW(SizeY);

	int ChangeX = _ttoi(SizeX);
	int ChangeY = _ttoi(SizeY);

	m_LandMapOwner->SetSceneSize(ChangeX, ChangeY);
}

