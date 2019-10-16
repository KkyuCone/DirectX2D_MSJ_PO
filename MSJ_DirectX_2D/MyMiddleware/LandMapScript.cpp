#include "LandMapScript.h"
#include "LandRenderer.h"
#include "ToolCamera.h"
#include <Texture.h>
#include <Sprite.h>
#include <Input.h>
#include <GameDebug.h>
#include <wincodec.h>


bool LandMapScript::m_bPickStart = false;

LandMapScript::LandMapScript() 
	: m_bBigTile(true), m_bSlope(false), m_bSlimTile(false), m_bDownSlope(false), m_bSmallTile(false),
	m_CurFolderName(L""), m_CurTileImageName(L""),
	m_CameraObject(nullptr), m_CameraPTR(nullptr), m_CameraScript(nullptr), m_LandMapObject(nullptr), m_CurLandMapRender(nullptr),
	SaveSceneX(1024), SaveSCeneY(768)
{
}


LandMapScript::~LandMapScript()
{
}

// �������̵�

bool LandMapScript::Init()
{
	// �̹��� �ε�
	ImageLoading();

	// ī�޶������Ʈ
	m_CameraObject = GetScene()->GameObject_Create(L"Camera");
	m_CameraPTR = m_CameraObject->Component_Add<Camera>();
	m_CameraPTR->SettingViewGroup(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
	m_CameraPTR->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -100.0f));
	m_CameraScript = m_CameraObject->Component_Add<ToolCamera>();
	m_CameraScript->SetCamera(m_CameraPTR);

	m_LandMapObject = GetScene()->GameObject_Create(L"LandMapRenderObject");

	if (false == Input::IsKey(L"MapTilePick"))
	{
		Input::Create_Key(L"MapTilePick", VK_LBUTTON);
	}

	return true;
}

bool LandMapScript::Loading()
{
	return true;
}

void LandMapScript::Start()
{

}

void LandMapScript::Update()
{
	// Up�̸� ������ ��Ÿ���� �������. ���ο� ��Ÿ���� �������ϴϱ� ���� �������� nullptr�� �ٲ��ش�.
	if (true == Input::Up(L"MapTilePick"))
	{
		// Ÿ���� ����ٸ�
		if (m_CurLandMapRender != nullptr)
		{
			m_CurLandMapRender->SetAutoSmall(m_bSmallTile);
			m_CurLandMapRender->SetAutoSlim(m_bSlimTile);
			m_CurLandMapRender->SetAutoSlop(m_bSlope);
			m_CurLandMapRender->SetAutoDownSlop(m_bDownSlope);
			m_CurLandMapRender->SetAutoTileAllMap(m_AllAutoTileMap);	// �ڵ�Ÿ�ϵ� �־��ֱ�
			m_CurLandMapRender->SetAutoTileDrawCheck(true);		// ������ �ڵ� Ÿ�� �׸���

			m_LandMapList.push_back(m_CurLandMapRender);
		}
		m_CurLandMapRender = nullptr;
		return;
	}

	// Ÿ���� ���õ��� �ʾҴٸ�
	if (L"" == m_CurTileImageName)
	{
		return;
	}

	// ȭ�� ���� Ŭ���Ѱ��� üũ�ؾ��Ѵ�. true�� ȭ��� false�� ȭ�� ��
	bool ClickSceneCheck = ClickSceneRangeExcess();


	// ���ο� Ÿ�� �������� �������ش�.
	if (true == Input::DownStay(L"MapTilePick") && nullptr == m_CurLandMapRender)
	{
		// ȭ��� üũ
		if (true == ClickSceneCheck)
			return;

		// Ÿ�� �׸���
		FirstMapRender();
	}

	// ���� Ÿ�� �������� �׷��ش�. ( ���� ������ ��� �巡���ϰ� �־����.. )
	if (true == Input::DownStay(L"MapTilePick") && nullptr != m_CurLandMapRender)
	{
		// ȭ��� üũ
		if (true == ClickSceneCheck)
			return;

		// Ÿ�� �׸���
		RemainderMapRender();
	}
}

void LandMapScript::LateUpdate()
{
}

void LandMapScript::End()
{

}

void LandMapScript::DebugRender()
{
	// ���콺 ����
	wchar_t Arr[256];
	swprintf_s(Arr, L"WorldMousePos = %f, %f", GetScene()->GetCamera()->WMousePosition().x, GetScene()->GetCamera()->WMousePosition().y);
	GameDebug::DrawStr(Arr, 20, { 10.0f, 30.0f }, { 1.0f, 1.0f, 1.0f }, FW1_TEXT_FLAG::FW1_TOP);

	// FPS
	wchar_t CurFPS[256];
	swprintf_s(CurFPS, L"FPS = %d", Time::FPS());
	GameDebug::DrawStr(CurFPS, 20, { 10.0f, 60.0f }, { 1.0f, 1.0f, 1.0f }, FW1_TEXT_FLAG::FW1_TOP);
}

/////////////////////////////////////////////////////////////////////////////////

// �̹��� �ε�
void LandMapScript::ImageLoading()
{
	PathFileLoad(L"arcana1.img");
	PathFileLoad(L"arcana2.img");
	PathFileLoad(L"blueToyCastle.img");
	PathFileLoad(L"BM2-1.img");
	PathFileLoad(L"BM2-2.img");
	PathFileLoad(L"critias.img");
	PathFileLoad(L"darkGrassySoil.img");
	PathFileLoad(L"destructionField.img");
	PathFileLoad(L"flagDay.img");
	PathFileLoad(L"grassySoil.img");			// GrassySoil (������� �⺻�����Ѱ�)
	PathFileLoad(L"greenToyCastle.img");
	PathFileLoad(L"logMarble.img");
	PathFileLoad(L"zerotempleFiled.img");
}

// �� �����

// ó��
void LandMapScript::FirstMapRender()
{
	m_CurLandMapRender = m_LandMapObject->Component_Add<LandRenderer>(1);
	Vector4 ClickPosition = Vector4(m_CameraPTR->GetTransform()->Local_Position().x + GetScene()->GetCamera()->WMousePosition().x, m_CameraPTR->GetTransform()->Local_Position().y + GetScene()->GetCamera()->WMousePosition().y, 0.0f, 0.0f);

	m_CurLandMapRender->SetTileImage(m_CurTileImageName.c_str());		// Ÿ�� �̹��� ����
	size_t Width = m_CurLandMapRender->GetSprite()->GetTexture()->Width();
	size_t Height = m_CurLandMapRender->GetSprite()->GetTexture()->Height();
	m_CurLandMapRender->SetTileSize(Vector2((float)Width, (float)Height));				// ������ �޾ƿ���
	m_CurLandMapRender->CreateTile(Vector2(ClickPosition.x, ClickPosition.y));		// Ÿ�� ���� (��ġ)
}

// ������
void LandMapScript::RemainderMapRender()
{
	Vector4 ClickPosition = Vector4(m_CameraPTR->GetTransform()->Local_Position().x + GetScene()->GetCamera()->WMousePosition().x, m_CameraPTR->GetTransform()->Local_Position().y + GetScene()->GetCamera()->WMousePosition().y, 0.0f, 0.0f);

	// ���� Ÿ��
	std::wstring FileName = RandomTile();

	// ����ó��
	if (L"" == FileName)
	{
		EAssert(true);
		return;
	}

	m_CurLandMapRender->SetTileImage(FileName.c_str());
	m_CurLandMapRender->CreateTile(Vector2(ClickPosition.x, ClickPosition.y));
}

//

void LandMapScript::PathFileLoad(std::wstring _RootPathFolder)
{
	std::vector<std::wstring> AssortmentTileImage;

	std::vector<std::wstring> CharacterImages = Path::PathToFullFileList(Path::FindPath(_RootPathFolder.c_str()), L"*.png");
	for (size_t i = 0; i < CharacterImages.size(); ++i)
	{
		AssortmentTileImage.push_back(CharacterImages[i]);				// Ÿ���̹��� �־��ֱ�

		ResourceManager<Texture>::Load(Path::FindPath(_RootPathFolder.c_str()) + CharacterImages[i]);
		ResourceManager<Sprite>::Create(CharacterImages[i], CharacterImages[i].c_str(), 1, 1);
	}

	if (0 < AssortmentTileImage.size())
	{
		// ���������� ���� �̸� ������ �������
		m_ALlTileMap.insert(std::unordered_map<std::wstring, std::vector<std::wstring>>::value_type(_RootPathFolder, AssortmentTileImage));
	}
}


//

void LandMapScript::SetFileName(std::wstring _FileName)
{
	m_CurTileImageName = _FileName;
}

void LandMapScript::ALLAutoTileVectorClear()
{
	// �̸��� �����ִ� ���͵� ����� ( �ȿ� ��Ұ� �ִٸ�..)
	if (0 < m_BigTileNames.size())
	{
		m_BigTileNames.erase(m_BigTileNames.begin(), m_BigTileNames.end());
	}

	if (0 < m_SmallTileNames.size())
	{
		m_SmallTileNames.erase(m_SmallTileNames.begin(), m_SmallTileNames.end());
	}

	m_RSlopAutoTileVec.clear();
	m_LSlopAutoTileVec.clear();
	m_UpAutoTileVec.clear();
	m_LeftAutoTileVec.clear();
	m_RightAutoTileVec.clear();
	m_DownAutoTileVec.clear();
	m_RDSlopAutoTileVec.clear();
	m_LDSlopAutoTileVec.clear();

	m_SDownAutoTileVec.clear();
	m_SDownAutoLHalfTileVec.clear();
	m_SDownAutoRHalfTileVec.clear();
	m_SUpAutoLHalfTileVec.clear();
	m_SUpAutoRHalfTileVec.clear();
}

void LandMapScript::SetFolderName(std::wstring _Folder)
{

	std::unordered_map<std::wstring, std::vector<std::wstring>>::iterator StartIter = m_ALlTileMap.begin();
	std::unordered_map<std::wstring, std::vector<std::wstring>>::iterator EndIter = m_ALlTileMap.end();


	std::unordered_map<std::wstring, std::vector<std::wstring>>::iterator FindIter = m_ALlTileMap.find(_Folder);

	// ����ó��
	if (EndIter == FindIter)
	{
		return;
	}

	m_CurFolderName = _Folder;

	ALLAutoTileVectorClear();			// ������ �ٲ� -> �ٲ� ������ ���� Ÿ�� Ŭ�� -> ������ ��Ƶ� Ÿ�ϵ� ����

	// �׸��� �ִ� �̸��� �־��ֱ�
	for (; StartIter!= EndIter; ++StartIter)
	{
		if (_Folder == StartIter->first)
		{
			for (size_t i = 0; i < StartIter->second.size(); i++)
			{
				// Ÿ�� �˻��� �־��ֱ�(Ÿ���̸�����..)
				DrawTileNamesInit(StartIter->second[i]);
			}
		}

	}

	// �������� ���� �ڵ�Ÿ�ϵ� ���͸� �ڵ�Ÿ�� ��ü������ �־��ش�.
	AutoTileMapAdd(L"Auto_RSlopTile", m_RSlopAutoTileVec);
	AutoTileMapAdd(L"Auto_LSlopTile", m_LSlopAutoTileVec);
	AutoTileMapAdd(L"Auto_UpTile", m_UpAutoTileVec);
	AutoTileMapAdd(L"Auto_LeftTile", m_LeftAutoTileVec);
	AutoTileMapAdd(L"Auto_RightTile", m_RightAutoTileVec);
	AutoTileMapAdd(L"Auto_DownTile", m_DownAutoTileVec);
	AutoTileMapAdd(L"Auto_RDownTile", m_RDSlopAutoTileVec);				// ���ϴܰ��Ÿ��
	AutoTileMapAdd(L"Auto_LDownTile", m_LDSlopAutoTileVec);				// ���ϴܰ��Ÿ��

	AutoTileMapAdd(L"Auto_SDownTile", m_SDownAutoTileVec);

	AutoTileMapAdd(L"Auto_SDownLHalfTile", m_SDownAutoLHalfTileVec);		// ���ϴ�
	AutoTileMapAdd(L"Auto_SDownRHalfTile", m_SDownAutoRHalfTileVec);		// ���ϴ�
	AutoTileMapAdd(L"Auto_SUpLHalfTile", m_SUpAutoLHalfTileVec);			// �»��
	AutoTileMapAdd(L"Auto_SUpRHalfTile", m_SUpAutoRHalfTileVec);			// ����
}

// �ڵ�Ÿ�� ���� ����
void LandMapScript::AutoTileMapAdd(std::wstring _TileType, std::vector<std::wstring> _TileVector)
{
	std::unordered_map<std::wstring, std::vector<std::wstring>>::iterator FindIter = m_AllAutoTileMap.find(_TileType);

	// ������ Ÿ���� �����Ѵٸ� (�̹� �ִٸ�) ���� ������ ������ Ÿ���̴� �������ش�..
	if (m_AllAutoTileMap.end() != FindIter)
	{
		m_AllAutoTileMap.erase(FindIter);
	}

	m_AllAutoTileMap.insert(std::unordered_map<std::wstring, std::vector<std::wstring>>::value_type(_TileType, _TileVector));
}

// � Ÿ������ �˻��ϰ� �� Ÿ�Ϻ��Ϳ� �־��ֱ�
void LandMapScript::DrawTileNamesInit(std::wstring _TileImageName)
{
	std::wstring IamgeNameCheck = _TileImageName;

	// ���� Ÿ������, ���Ÿ������, �ϴ� ���Ÿ������ üũ�ϰ� �־������.

	if (std::wstring::npos != IamgeNameCheck.find(L"_bsc."))
	{
		// ���� �� �ִ� ūŸ��
		// ���� Ÿ������ üũ
		m_BigTileNames.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_edU."))
	{
		// ���� �� �ִ� ����Ÿ��
		m_SmallTileNames.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_slRU."))
	{
		// ������ ���Ÿ��
		m_RSlopAutoTileVec.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_slLU."))
	{
		// ���� ���Ÿ��
		m_LSlopAutoTileVec.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_enH0."))
	{
		// ���� Ÿ��
		m_UpAutoTileVec.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_enV0."))
	{
		// ���� Ÿ��
		m_LeftAutoTileVec.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_enV1."))
	{
		// ������ Ÿ��
		m_RightAutoTileVec.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_enH1."))
	{
		// �Ʒ� Ÿ��(�⺻��)
		m_DownAutoTileVec.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_slRD."))
	{
		// ������ �Ʒ� Ÿ��(��������)
		m_RDSlopAutoTileVec.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_slLD."))
	{
		// ���� �Ʒ� Ÿ��(��������)
		m_LDSlopAutoTileVec.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_edD."))
	{
		// ���� �Ʒ�Ÿ�� (�̰� �ϳ�����)
		m_SDownAutoTileVec.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_edDHL."))
	{
		// ���� �Ʒ� ���� ����Ÿ��
		m_SDownAutoLHalfTileVec.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_edDHR."))
	{
		// ���� �Ʒ� ������ ����Ÿ�� 
		m_SDownAutoRHalfTileVec.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_edUHL."))
	{
		// ���� �� ���� ����Ÿ��
		m_SUpAutoLHalfTileVec.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_edUHR."))
	{
		// ���� �� ������ ����Ÿ��
		m_SUpAutoRHalfTileVec.push_back(IamgeNameCheck);
	}
	else
	{
		int a = 0;
		// ���� ����� ������ȵȴ�.
		EAssert(true);
		return;
	}
}

void LandMapScript::SetPathName(std::wstring _PathName)
{

}

void LandMapScript::SetBigTile(bool _Value)
{
	m_bBigTile = _Value;
}

void LandMapScript::SetSlopCheck(int _Value)
{
	if (0 == _Value)
	{
		m_bSlope = false;		// ���üũ X
	}
	else
	{
		m_bSlope = true;		// ���üũ O
	}
}

void LandMapScript::SetDownSlopCheck(int _Value)
{
	if (0 == _Value)
	{
		m_bDownSlope = false;		// ���üũ X
	}
	else
	{
		m_bDownSlope = true;		// ���üũ O
	}
}

void LandMapScript::SetSlimCheck(int _Value)
{
	if (0 == _Value)
	{
		m_bSlimTile = false;		// ���üũ X
	}
	else
	{
		m_bSlimTile = true;		// ���üũ O
	}
}


std::wstring LandMapScript::RandomTile()
{
	// ���� ��ȣ �ο�
	int RandomValue = -1;
	std::wstring ImageName = L"";

	if (true == m_bSlimTile)
	{
		// ���� Ÿ���� ���
		m_bSmallTile = false;
		RandomValue = rand() % m_UpAutoTileVec.size();
		ImageName = m_UpAutoTileVec[RandomValue];
	}
	else
	{
		// ����Ÿ���� �ƴѰ��
		if (true == m_bBigTile)
		{
			// ūŸ��
			m_bSmallTile = false;
			RandomValue = rand() % m_BigTileNames.size();
			ImageName = m_BigTileNames[RandomValue];
		}
		else
		{
			// ����Ÿ�� - �ɲ���
			m_bSmallTile = true;
			RandomValue = rand() % m_SmallTileNames.size();
			ImageName = m_SmallTileNames[RandomValue];
		}
	}


	if (-1 == RandomValue)
	{
		return L"";
	}

	//std::wstring Number = std::to_wstring(RandomValue);		// ��ȣ
	//std::wstring FileName = L"grassySoil.img_bsc." + Number + L".png";

	return ImageName;
}

// ȭ�� �� üũ
bool LandMapScript::ClickSceneRangeExcess()
{
	// ȭ����� true, ȭ�� ���̶�� false
	// ī�޶� + ���� �����ִ� ���� �������� �Ǵ��Ѵ�. (���� 1024, ���� 768)
	Vector2 ClickPosition = Vector2(m_CameraPTR->GetTransform()->Local_Position().x + GetScene()->GetCamera()->WMousePosition().x, m_CameraPTR->GetTransform()->Local_Position().y + GetScene()->GetCamera()->WMousePosition().y);

	// ȭ�� ������ ..�ִ�ġ�� ����ġ
	Vector2 MaxRange = Vector2((float)(SaveSceneX / 2), (float)(SaveSCeneY / 2));
	Vector2 MinRange = Vector2(-(float)(SaveSceneX / 2), -(float)(SaveSCeneY / 2));


	if (ClickPosition.x < MinRange.x ||
		ClickPosition.x > MaxRange.x)
	{
		return true;
	}

	if (ClickPosition.y < MinRange.y ||
		ClickPosition.y > MaxRange.y)
	{
		return true;
	}
	
	return false;
}


void LandMapScript::ImageSave(std::wstring _FileName)
{

	DirectX::Image* pRealImage = new DirectX::Image();

	pRealImage->width = SaveSceneX;
	pRealImage->height = SaveSCeneY;
	pRealImage->format = DXGI_FORMAT_B8G8R8A8_UNORM;
	int* pixelsSize = new int[pRealImage->width *  pRealImage->height];

	pRealImage->pixels = (uint8_t*)pixelsSize;

	for (size_t i = 0; i < pRealImage->width * pRealImage->height; ++i)
	{
		// ����
		pixelsSize[i] = 0x00ff0000;  // ARGB  -> �ܼ� �ʱ�ȭ
	}


	std::list<My_PTR<LandRenderer>>::iterator StartLand = m_LandMapList.begin();
	std::list<My_PTR<LandRenderer>>::iterator EndLand = m_LandMapList.end();

	for (; StartLand != EndLand; ++StartLand)
	{
		// �ȿ��� �׷��ֱ�
		(*StartLand)->DrawLandMap(pixelsSize, pRealImage->width, pRealImage->height);
	}

	pRealImage->rowPitch = SaveSceneX * sizeof(int); // SaveImage->width * SaveImage->format �� ����Ʈ ������ (��� 1����Ʈ�ϱ� ARGB -> 4����Ʈ)
	pRealImage->slicePitch = pRealImage->rowPitch * pRealImage->height;	// rowPitch * ����

	std::wstring FName = _FileName + L".png";

	std::wstring FileName = Path::FindPath(L"SaveImage");
	FileName += FName;


	HRESULT hr = DirectX::SaveToWICFile(*pRealImage, DirectX::WIC_FLAGS_NONE, GUID_ContainerFormatPng, FileName.c_str());



	////hr = D3DX11SaveTextureToFile(Device::Get_Context(), (ID3D11Resource*)(m_Sprite->GetTexture()->GetTexture2D()), D3DX11_IFF_PNG, FileName.c_str());

	delete pixelsSize;
	delete pRealImage;
}


// ��Ÿ ����
void LandMapScript::SetSceneSize(int _X, int _Y)
{
	m_CameraScript->SetLimitPos(_X, _Y);

	if (500 > _X || 500 > _Y)
		return;

	SaveSceneX = _X;
	SaveSCeneY = _Y;
}


//void LandMapScript::MouseWheelEvent()
//{
//	DIMOUSESTATE button;
//	Device::Get_Device()->Sta
//}