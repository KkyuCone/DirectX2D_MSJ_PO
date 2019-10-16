// MapDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyTool.h"
#include "MapDialog.h"
#include "afxdialogex.h"
#include <Input.h>


// MapDialog 대화 상자입니다.

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


// MapDialog 메시지 처리기입니다.


void MapDialog::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
}


void MapDialog::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnCancel();
}


BOOL MapDialog::OnInitDialog()
{
	TabDialog::OnInitDialog();
	ReSpriteFinder();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void MapDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	TabDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_LandMapOwner = m_TabScene->Script_Create<LandMapScript>();
}


// 트리관련
void MapDialog::OnTvnSelchangedTiletreelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	//
	DWORD_PTR ItemPTR = m_TileTree.GetItemData(pNMTreeView->itemNew.hItem);
	ImageInfo* SpriteInfo_PTR = (ImageInfo*)ItemPTR;


	if (nullptr == SpriteInfo_PTR)
	{
		return;
	}

	// 파일이 아니라면 (폴더)
	if (false == SpriteInfo_PTR->m_bFile)
	{
		return;
	}

	My_PTR<Sprite> LoadSprite = ResourceManager<Sprite>::Find(SpriteInfo_PTR->m_FileName.GetString());
	My_PTR<Texture> LoadTexture = ResourceManager<Texture>::Find(SpriteInfo_PTR->m_FileName.GetString());

	if (nullptr == LoadSprite)
	{
		// 파일만들기 없으니까..(안넣어서)
		LoadTexture = ResourceManager<Texture>::Load(SpriteInfo_PTR->m_Path.GetString());
		LoadSprite = ResourceManager<Sprite>::Create(SpriteInfo_PTR->m_FileName.GetString(), SpriteInfo_PTR->m_FileName.GetString());
	}


	// 여기서 세팅 순서 유의해야함.. 순서잘못되면 터짐 -ㅅ-;;
	m_LandMapOwner->SetFileName(SpriteInfo_PTR->m_FileName.GetString());
	m_LandMapOwner->SetBigTile(SpriteInfo_PTR->m_bBigTile);							// 큰타일/작은타일 여부 세팅
	m_LandMapOwner->SetSlimCheck(m_SlimCheck.GetCheck());							// 얇은 타일 체크
	m_LandMapOwner->SetSlopCheck(m_SlopeCehck.GetCheck());							// 경사 체크
	m_LandMapOwner->SetDownSlopCheck(m_DownSlopCheck.GetCheck());					// 하단 경사 타일 체크
	m_LandMapOwner->SetFolderName(SpriteInfo_PTR->m_FolderName.GetString());		// 폴더 변경

	// 미리보기
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

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	//폴더내 이미지들..
	m_TileTree.DeleteAllItems();

	std::wstring TileName = Path::FindPath(L"MapTile");
	CString TilePath = TileName.c_str();

	HTREEITEM m_Item = m_TileTree.InsertItem(L"MapTile");
	SpriteFinder(TilePath, m_Item);		// 찾기 

	m_TileTree.Expand(m_Item, TVE_EXPAND);  // 확장하기 ( 바로 로드해주는거 ㅇㅅㅇ )
}

// 추가
void MapDialog::SpriteFinder(const CString & _Folder, HTREEITEM _Parent)
{
	// 파일찾기~
	CFileFind FileFinder;

	// *.* = *(파일이름).*(확장자) 모든 파일이름의 모든 확장자 찾기
	// . (자기자신), ..(부모)
	BOOL IsFile = FileFinder.FindFile(_Folder + L"*.*");

	while (TRUE == IsFile)
	{
		IsFile = FileFinder.FindNextFileW();

		if (FileFinder.IsDots())
		{
			// 자기 자신
			continue;
		}
		else if (TRUE == FileFinder.IsDirectory())
		{
			// 디렉토리가 더있다면..(폴더가 더 있다면..)
			std::wstring FolderName = FileFinder.GetFileName();

			// 내가 그릴수있는 타일만 띄우게하기 (검사)

			HTREEITEM FolderItem = m_TileTree.InsertItem(FolderName.c_str(), _Parent);
			SpriteFinder(_Folder + FileFinder.GetFileName() + L"\\", FolderItem);

			//
			ImageInfo* NewSpriteInfo = new ImageInfo();
			NewSpriteInfo->m_bFile = false;		// 얜 파일이아니고 폴더니까..
			DWORD_PTR PTR = (DWORD_PTR)NewSpriteInfo;
			m_TileTree.SetItemData(FolderItem, PTR);

			m_TileSpriteInfoList.push_back(NewSpriteInfo);
		}
		else
		{
			// 타일 띄우기
			std::wstring FolderName = FileFinder.GetFileName();

			// 내가 그릴수있는 타일만 띄우게하기 (검사)
			if (1 == m_SlimCheck.GetCheck())
			{
				// 얇은 타일이라면 얇은타일만 목록에 넣게 한다.
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

			// 타일 보여주기 유무
			ImageInfo* NewSpriteInfo = new ImageInfo();
			NewSpriteInfo->m_bFile = true;		// 얜 파일
			NewSpriteInfo->m_Path = _Folder + FolderName.c_str();
			NewSpriteInfo->m_FileName = FolderName.c_str();
			NewSpriteInfo->m_OnlyPath = _Folder;

			std::wstring OnlyFolderName = FolderName.substr(0, FolderName.find(L"_"));
			NewSpriteInfo->m_FolderName = OnlyFolderName.c_str();

			if (std::wstring::npos != FolderName.find(L".img_edU."))
			{
				// 찍을수 있는 작은타일
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
	// 삭제 (delete)
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

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


// 경사체크
void MapDialog::OnBnClickedSlope()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (1 == m_SlimCheck.GetCheck())
	{
		// 경사타일을 설정한 경우 얇은 타일을 사용할 수 없다.
		m_SlimCheck.SetCheck(0);
	}

	ReSpriteFinder();		// 체크한 후에 다시 목록을 보여줘야한다. (선택할수있는 이미지로)
}


// 아래쪽 맵타일 경사 체크
void MapDialog::OnBnClickedSlopdown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (1 == m_SlimCheck.GetCheck())
	{
		// 경사타일을 설정한 경우 얇은 타일을 사용할 수 없다.
		m_SlimCheck.SetCheck(0);
	}

	ReSpriteFinder();		// 체크한 후에 다시 목록을 보여줘야한다. (선택할수있는 이미지로)
}

// 얇은타일 체크
void MapDialog::OnBnClickedSlim()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if ( 1 == m_SlopeCehck.GetCheck() || 1 == m_DownSlopCheck.GetCheck())
	{
		// 얇은 타일을 설정한 경우
		// 경사타일을 사용할 수없다.
		m_SlopeCehck.SetCheck(0);
		m_DownSlopCheck.SetCheck(0);
	}

	ReSpriteFinder();		// 체크한 후에 다시 목록을 보여줘야한다. (선택할수있는 이미지로)
}


void MapDialog::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_InputSaveName.GetWindowTextW(m_SaveFileName);
	m_LandMapOwner->ImageSave(m_SaveFileName.GetString());
}




// 선택한 타일 미리보기
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}



void MapDialog::OnBnClickedScenesize()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString SizeX;
	CString SizeY;
	m_SceneX.GetWindowTextW(SizeX);
	m_SceneY.GetWindowTextW(SizeY);

	int ChangeX = _ttoi(SizeX);
	int ChangeY = _ttoi(SizeY);

	m_LandMapOwner->SetSceneSize(ChangeX, ChangeY);
}

