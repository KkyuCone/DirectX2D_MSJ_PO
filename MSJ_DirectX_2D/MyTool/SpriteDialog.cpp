// SpriteDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyTool.h"
#include "SpriteDialog.h"
#include "afxdialogex.h"
#include <Path.h>
#include <RectCol2D.h>


// SpriteDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(SpriteDialog, CDialogEx)

SpriteDialog::SpriteDialog(CWnd* pParent /*=NULL*/)
	: TabDialog(IDD_SPRITEDLG, pParent)
{

}

SpriteDialog::~SpriteDialog()
{
}

void SpriteDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPRITETREE, m_SpriteTree);		// ��ũ���ֱ� ������
}


BEGIN_MESSAGE_MAP(SpriteDialog, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &SpriteDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &SpriteDialog::OnBnClickedOk)
	ON_NOTIFY(TVN_SELCHANGED, IDC_SPRITETREE, &SpriteDialog::OnTvnSelchangedSpritetree)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// SpriteDialog �޽��� ó�����Դϴ�.

void SpriteDialog::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//CDialogEx::OnCancel();
}


void SpriteDialog::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//CDialogEx::OnOK();
}


BOOL SpriteDialog::OnInitDialog()
{
	TabDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_SpriteTree.DeleteAllItems();		// ������ ���δ� �����ϱ�

	std::wstring Name = Path::FindPath(L"Texture");			// ��� ����
	CString NewPath = Name.c_str();

	HTREEITEM m_Item = m_SpriteTree.InsertItem(L"Texture");
	SpriteFinder(NewPath, m_Item);		// ã�� 

	// Ȯ���ϱ� ( �ٷ� �ε����ִ°� ������ )
	m_SpriteTree.Expand(m_Item, TVE_EXPAND);

	My_PTR<GameObject> NewGameObject = m_TabScene->GameObject_Create(L"Camera");
	My_PTR<Camera> NewCamera = NewGameObject->Component_Add<Camera>();		// ī�޶��߰�
	NewCamera->SettingViewGroup(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
	NewCamera->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -100.0f));

	My_PTR<GameObject> NewBack = m_TabScene->GameObject_Create(L"SpriteViewer");
	//NewBack->Over(L"Ani", L"Map");		// ���ӿ�����Ʈ�� �ѳ���¾��� �߰�

	NewBack->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 20.0f));
	NewBack->GetTransform()->Local_Scale(Vector4(500.0f, 500.0f, 1.0f));		//ũ�� ���Ƿ�..
	NewBack->Component_Add<RectCol2D>();
	
	m_BackRender = NewBack->Component_Add<SpriteRenderer2D>(0);

	//NewBack->Over(L"Map");


	// �߰�

	My_PTR<GameObject> NewBack1 = m_TabScene->GameObject_Create(L"SpriteViewer1");
	My_PTR<GameObject> NewBack2 = m_TabScene->GameObject_Create(L"SpriteViewer2");
	My_PTR<GameObject> NewBack3 = m_TabScene->GameObject_Create(L"SpriteViewer3");
	My_PTR<GameObject> NewBack4 = m_TabScene->GameObject_Create(L"SpriteViewer4");

	NewBack1->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 20.0f));
	NewBack1->GetTransform()->Local_Scale(Vector4(500.0f, 500.0f, 1.0f));		//ũ�� ���Ƿ�..

	NewBack2->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 20.0f));
	NewBack2->GetTransform()->Local_Scale(Vector4(500.0f, 500.0f, 1.0f));		//ũ�� ���Ƿ�..

	NewBack3->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 20.0f));
	NewBack3->GetTransform()->Local_Scale(Vector4(500.0f, 500.0f, 1.0f));		//ũ�� ���Ƿ�..

	NewBack4->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 20.0f));
	NewBack4->GetTransform()->Local_Scale(Vector4(500.0f, 500.0f, 1.0f));		//ũ�� ���Ƿ�..

	m_BackRender1 = NewBack1->Component_Add<SpriteRenderer2D>(0);;
	m_BackRender2 = NewBack2->Component_Add<SpriteRenderer2D>(0);;
	m_BackRender3 = NewBack3->Component_Add<SpriteRenderer2D>(0);;
	m_BackRender4 = NewBack4->Component_Add<SpriteRenderer2D>(0);;


	//
	//NewBack1->Over(L"Map");
	//NewBack2->Over(L"Map");
	//NewBack3->Over(L"Map");
	//NewBack4->Over(L"Map");

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void SpriteDialog::SpriteFinder(const CString & _Folder, HTREEITEM _Parent)
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
			HTREEITEM FolderItem = m_SpriteTree.InsertItem(FolderName.c_str(), _Parent);
			SpriteFinder(_Folder + FileFinder.GetFileName() + L"\\", FolderItem);

			//
			SpriteInfo* NewSpriteInfo = new SpriteInfo();
			NewSpriteInfo->m_bFile = false;		// �� �����̾ƴϰ� �����ϱ�..
			DWORD_PTR PTR = (DWORD_PTR)NewSpriteInfo;
			m_SpriteTree.SetItemData(FolderItem, PTR);

			m_TileSpriteInfoList.push_back(NewSpriteInfo);
		}
		else
		{
			std::wstring FolderName = FileFinder.GetFileName();
			HTREEITEM FileItem = m_SpriteTree.InsertItem(FolderName.c_str(), _Parent);

			//
			SpriteInfo* NewSpriteInfo = new SpriteInfo();
			NewSpriteInfo->m_bFile = true;		// �� ����
			NewSpriteInfo->m_Path = _Folder + FolderName.c_str();
			NewSpriteInfo->m_FileName = FolderName.c_str();
			NewSpriteInfo->m_OnlyPath = _Folder;

			DWORD_PTR PTR = (DWORD_PTR)NewSpriteInfo;
			m_SpriteTree.SetItemData(FileItem, PTR);

			m_TileSpriteInfoList.push_back(NewSpriteInfo);
		}
	}
}


void SpriteDialog::OnTvnSelchangedSpritetree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	DWORD_PTR ItemPTR = m_SpriteTree.GetItemData(pNMTreeView->itemNew.hItem);
	SpriteInfo* SpriteInfo_PTR = (SpriteInfo*)ItemPTR;


	if (nullptr == m_BackRender)
	{
		return;
	}

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
		LoadTexture = ResourceManager<Texture>::Load(SpriteInfo_PTR->m_Path.GetString());
		LoadSprite = ResourceManager<Sprite>::Create(SpriteInfo_PTR->m_FileName.GetString(), SpriteInfo_PTR->m_FileName.GetString());
	}

	// ũ�� ����..?"��";;��ư �°�����
	m_BackRender->GetGameObject()->GetTransform()->Local_Scale(Vector4((float)LoadTexture->Width(), (float)LoadTexture->Height(), 1.0f));
	m_BackRender->SetSprite(SpriteInfo_PTR->m_FileName.GetString(), 0);

	
	// �Ǽ��Ǽ�
	SpriteInfo* SpriteInfo_PTR1 = (SpriteInfo*)ItemPTR;
	SpriteInfo* SpriteInfo_PTR2 = (SpriteInfo*)ItemPTR;
	SpriteInfo* SpriteInfo_PTR3 = (SpriteInfo*)ItemPTR;
	SpriteInfo* SpriteInfo_PTR4 = (SpriteInfo*)ItemPTR;

	CString SpriteInfo_PTR1_FileName = L"";
	CString SpriteInfo_PTR2_FileName = L"";
	CString SpriteInfo_PTR3_FileName = L"";
	CString SpriteInfo_PTR4_FileName = L"";

	CString SpriteInfo_PTR1_Path = SpriteInfo_PTR1->m_OnlyPath;
	CString SpriteInfo_PTR2_Path = SpriteInfo_PTR1->m_OnlyPath;
	CString SpriteInfo_PTR3_Path = SpriteInfo_PTR1->m_OnlyPath;
	CString SpriteInfo_PTR4_Path = SpriteInfo_PTR1->m_OnlyPath;


	const wchar_t* SpriteName = SpriteInfo_PTR->m_FileName.GetString();
	const wchar_t* SpritePath = SpriteInfo_PTR->m_OnlyPath.GetString();


	std::wstring Sprite_enH0 = L"";		// ��
	std::wstring Sprite_enH1 = L"";		// �Ʒ�
	std::wstring Sprite_enV0 = L"";		// ����
	std::wstring Sprite_enV1 = L"";		// ������ �Ѥ�

	int ab = rand() % 4;
	int cd = rand() % 3;

	switch (ab)
	{
	case 0:
		Sprite_enH0 = L"enH0.0.png";
		Sprite_enH1 += L"enH1.0.png";
		break;
	case 1:
		Sprite_enH0 += L"enH0.1.png";
		Sprite_enH1 += L"enH1.1.png";
		break;
	case 2:
		Sprite_enH0 += L"enH0.2.png";
		Sprite_enH1 += L"enH1.2.png";
		break;
	case 3:
		Sprite_enH0 += L"enH0.3.png";
		Sprite_enH1 += L"enH1.3.png";
		break;
	}

	switch (cd)
	{
	case 0:
		Sprite_enV0 += L"enV0.0.png";
		Sprite_enV1 += L"enV1.0.png";
		break;
	case 1:
		Sprite_enV0 += L"enV0.1.png";
		Sprite_enV1 += L"enV1.1.png";
		break;
	case 2:
		Sprite_enV0 += L"enV0.2.png";
		Sprite_enV1 += L"enV1.2.png";
		break;
	}

	
	if (nullptr != wcsstr(SpriteName, L"bsc"))
	{
		// �´� �༮�̸�~ (���ڿ��˻�)
		int a = 0;



		SpriteInfo_PTR1_FileName = Sprite_enH0.c_str();
		SpriteInfo_PTR2_FileName = Sprite_enH1.c_str();
		SpriteInfo_PTR3_FileName = Sprite_enV0.c_str();
		SpriteInfo_PTR4_FileName = Sprite_enV1.c_str();

		SpriteInfo_PTR1_Path += SpriteInfo_PTR1_FileName;
		SpriteInfo_PTR2_Path += SpriteInfo_PTR2_FileName;
		SpriteInfo_PTR3_Path += SpriteInfo_PTR3_FileName;
		SpriteInfo_PTR4_Path += SpriteInfo_PTR4_FileName;


		My_PTR<Sprite> LoadSprite1 = ResourceManager<Sprite>::Find((const wchar_t*)SpriteInfo_PTR1_FileName);
		My_PTR<Texture> LoadTexture1 = ResourceManager<Texture>::Find((const wchar_t*)SpriteInfo_PTR1_FileName);

		My_PTR<Sprite> LoadSprite2 = ResourceManager<Sprite>::Find((const wchar_t*)SpriteInfo_PTR2_FileName);
		My_PTR<Texture> LoadTexture2 = ResourceManager<Texture>::Find((const wchar_t*)SpriteInfo_PTR2_FileName);

		My_PTR<Sprite> LoadSprite3 = ResourceManager<Sprite>::Find((const wchar_t*)SpriteInfo_PTR3_FileName);
		My_PTR<Texture> LoadTexture3 = ResourceManager<Texture>::Find((const wchar_t*)SpriteInfo_PTR3_FileName);

		My_PTR<Sprite> LoadSprite4 = ResourceManager<Sprite>::Find((const wchar_t*)SpriteInfo_PTR4_FileName);
		My_PTR<Texture> LoadTexture4 = ResourceManager<Texture>::Find((const wchar_t*)SpriteInfo_PTR4_FileName);

		if (nullptr == LoadSprite1)
		{
			LoadTexture1 = ResourceManager<Texture>::Load((const wchar_t*)SpriteInfo_PTR1_Path);
			LoadSprite1 = ResourceManager<Sprite>::Create((const wchar_t*)SpriteInfo_PTR1_FileName, (const wchar_t*)SpriteInfo_PTR1_FileName);
		}

		if (nullptr == LoadSprite2)
		{
			LoadTexture2 = ResourceManager<Texture>::Load((const wchar_t*)SpriteInfo_PTR2_Path);
			LoadSprite2 = ResourceManager<Sprite>::Create((const wchar_t*)SpriteInfo_PTR2_FileName, (const wchar_t*)SpriteInfo_PTR2_FileName);
		}

		if (nullptr == LoadSprite3)
		{
			LoadTexture3 = ResourceManager<Texture>::Load((const wchar_t*)SpriteInfo_PTR3_Path);
			LoadSprite3 = ResourceManager<Sprite>::Create((const wchar_t*)SpriteInfo_PTR3_FileName, (const wchar_t*)SpriteInfo_PTR3_FileName);
		}

		if (nullptr == LoadSprite4)
		{
			LoadTexture4 = ResourceManager<Texture>::Load((const wchar_t*)SpriteInfo_PTR4_Path);
			LoadSprite4 = ResourceManager<Sprite>::Create((const wchar_t*)SpriteInfo_PTR4_FileName, (const wchar_t*)SpriteInfo_PTR4_FileName);
		}

		//

		m_BackRender1->GetGameObject()->GetTransform()->Local_Scale(Vector4((float)LoadTexture1->Width(), (float)LoadTexture1->Height(), 1.0f));
		m_BackRender1->SetSprite((const wchar_t*)SpriteInfo_PTR1_FileName, 0);

		m_BackRender2->GetGameObject()->GetTransform()->Local_Scale(Vector4((float)LoadTexture2->Width(), (float)LoadTexture2->Height(), 1.0f));
		m_BackRender2->SetSprite((const wchar_t*)SpriteInfo_PTR2_FileName, 0);

		m_BackRender3->GetGameObject()->GetTransform()->Local_Scale(Vector4((float)LoadTexture3->Width(), (float)LoadTexture3->Height(), 1.0f));
		m_BackRender3->SetSprite((const wchar_t*)SpriteInfo_PTR3_FileName, 0);

		m_BackRender4->GetGameObject()->GetTransform()->Local_Scale(Vector4((float)LoadTexture4->Width(), (float)LoadTexture4->Height(), 1.0f));
		m_BackRender4->SetSprite((const wchar_t*)SpriteInfo_PTR4_FileName, 0);


		Vector4 BasePosition = m_BackRender->GetTransform()->Local_Position();
		Vector4 BaseScale = m_BackRender->GetTransform()->Local_Scale();

		m_BackRender1->GetGameObject()->GetTransform()->Local_Position(Vector4(BasePosition.x, BasePosition.y + ((float)LoadTexture->Height()/2) + ((float)LoadTexture1->Height()/2), m_BackRender1->GetGameObject()->GetTransform()->Local_Position().z));
		m_BackRender2->GetGameObject()->GetTransform()->Local_Position(Vector4(BasePosition.x, BasePosition.y - ((float)LoadTexture->Height() / 2) - ((float)LoadTexture2->Height() / 2), m_BackRender1->GetGameObject()->GetTransform()->Local_Position().z));
		m_BackRender3->GetGameObject()->GetTransform()->Local_Position(Vector4(BasePosition.x - ((float)LoadTexture->Width() / 2) - ((float)LoadTexture3->Width() / 2), BasePosition.y, m_BackRender1->GetGameObject()->GetTransform()->Local_Position().z));
		m_BackRender4->GetGameObject()->GetTransform()->Local_Position(Vector4(BasePosition.x + ((float)LoadTexture->Width() / 2) + ((float)LoadTexture4->Width() / 2), BasePosition.y, m_BackRender1->GetGameObject()->GetTransform()->Local_Position().z));

	}
}



void SpriteDialog::OnDestroy()
{
	// ���� (delete)
	std::list<SpriteInfo*>::iterator Start = m_TileSpriteInfoList.begin();
	std::list<SpriteInfo*>::iterator End = m_TileSpriteInfoList.end();

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
