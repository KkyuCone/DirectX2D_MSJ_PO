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

// 오버라이딩

bool LandMapScript::Init()
{
	// 이미지 로딩
	ImageLoading();

	// 카메라오브젝트
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
	// Up이면 기존의 맵타일을 다찍었다. 새로운 맵타일을 만들어야하니까 현재 렌더러를 nullptr로 바꿔준다.
	if (true == Input::Up(L"MapTilePick"))
	{
		// 타일을 찍었다면
		if (m_CurLandMapRender != nullptr)
		{
			m_CurLandMapRender->SetAutoSmall(m_bSmallTile);
			m_CurLandMapRender->SetAutoSlim(m_bSlimTile);
			m_CurLandMapRender->SetAutoSlop(m_bSlope);
			m_CurLandMapRender->SetAutoDownSlop(m_bDownSlope);
			m_CurLandMapRender->SetAutoTileAllMap(m_AllAutoTileMap);	// 자동타일들 넣어주기
			m_CurLandMapRender->SetAutoTileDrawCheck(true);		// 나머지 자동 타일 그리기

			m_LandMapList.push_back(m_CurLandMapRender);
		}
		m_CurLandMapRender = nullptr;
		return;
	}

	// 타일이 선택되지 않았다면
	if (L"" == m_CurTileImageName)
	{
		return;
	}

	// 화면 밖을 클릭한건지 체크해야한다. true는 화면밖 false는 화면 안
	bool ClickSceneCheck = ClickSceneRangeExcess();


	// 새로운 타일 렌더러를 생성해준다.
	if (true == Input::DownStay(L"MapTilePick") && nullptr == m_CurLandMapRender)
	{
		// 화면밖 체크
		if (true == ClickSceneCheck)
			return;

		// 타일 그리기
		FirstMapRender();
	}

	// 기존 타일 렌더러에 그려준다. ( 얘의 시점은 계속 드래그하고 있어야함.. )
	if (true == Input::DownStay(L"MapTilePick") && nullptr != m_CurLandMapRender)
	{
		// 화면밖 체크
		if (true == ClickSceneCheck)
			return;

		// 타일 그리기
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
	// 마우스 포스
	wchar_t Arr[256];
	swprintf_s(Arr, L"WorldMousePos = %f, %f", GetScene()->GetCamera()->WMousePosition().x, GetScene()->GetCamera()->WMousePosition().y);
	GameDebug::DrawStr(Arr, 20, { 10.0f, 30.0f }, { 1.0f, 1.0f, 1.0f }, FW1_TEXT_FLAG::FW1_TOP);

	// FPS
	wchar_t CurFPS[256];
	swprintf_s(CurFPS, L"FPS = %d", Time::FPS());
	GameDebug::DrawStr(CurFPS, 20, { 10.0f, 60.0f }, { 1.0f, 1.0f, 1.0f }, FW1_TEXT_FLAG::FW1_TOP);
}

/////////////////////////////////////////////////////////////////////////////////

// 이미지 로딩
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
	PathFileLoad(L"grassySoil.img");			// GrassySoil (현재까지 기본으로한거)
	PathFileLoad(L"greenToyCastle.img");
	PathFileLoad(L"logMarble.img");
	PathFileLoad(L"zerotempleFiled.img");
}

// 맵 만들기

// 처음
void LandMapScript::FirstMapRender()
{
	m_CurLandMapRender = m_LandMapObject->Component_Add<LandRenderer>(1);
	Vector4 ClickPosition = Vector4(m_CameraPTR->GetTransform()->Local_Position().x + GetScene()->GetCamera()->WMousePosition().x, m_CameraPTR->GetTransform()->Local_Position().y + GetScene()->GetCamera()->WMousePosition().y, 0.0f, 0.0f);

	m_CurLandMapRender->SetTileImage(m_CurTileImageName.c_str());		// 타일 이미지 설정
	size_t Width = m_CurLandMapRender->GetSprite()->GetTexture()->Width();
	size_t Height = m_CurLandMapRender->GetSprite()->GetTexture()->Height();
	m_CurLandMapRender->SetTileSize(Vector2((float)Width, (float)Height));				// 사이즈 받아오기
	m_CurLandMapRender->CreateTile(Vector2(ClickPosition.x, ClickPosition.y));		// 타일 생성 (위치)
}

// 나머지
void LandMapScript::RemainderMapRender()
{
	Vector4 ClickPosition = Vector4(m_CameraPTR->GetTransform()->Local_Position().x + GetScene()->GetCamera()->WMousePosition().x, m_CameraPTR->GetTransform()->Local_Position().y + GetScene()->GetCamera()->WMousePosition().y, 0.0f, 0.0f);

	// 랜덤 타일
	std::wstring FileName = RandomTile();

	// 예외처리
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
		AssortmentTileImage.push_back(CharacterImages[i]);				// 타일이미지 넣어주기

		ResourceManager<Texture>::Load(Path::FindPath(_RootPathFolder.c_str()) + CharacterImages[i]);
		ResourceManager<Sprite>::Create(CharacterImages[i], CharacterImages[i].c_str(), 1, 1);
	}

	if (0 < AssortmentTileImage.size())
	{
		// 폴더정보와 파일 이름 정보가 들어있음
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
	// 이름들 갖고있는 벡터들 지우기 ( 안에 요소가 있다면..)
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

	// 예외처리
	if (EndIter == FindIter)
	{
		return;
	}

	m_CurFolderName = _Folder;

	ALLAutoTileVectorClear();			// 폴더가 바뀜 -> 바뀐 폴더의 찍을 타일 클릭 -> 기존의 모아둔 타일들 제거

	// 그릴수 있는 이름들 넣어주기
	for (; StartIter!= EndIter; ++StartIter)
	{
		if (_Folder == StartIter->first)
		{
			for (size_t i = 0; i < StartIter->second.size(); i++)
			{
				// 타일 검사후 넣어주기(타일이름별로..)
				DrawTileNamesInit(StartIter->second[i]);
			}
		}

	}

	// 다했으면 개별 자동타일들 벡터를 자동타일 전체묶음에 넣어준다.
	AutoTileMapAdd(L"Auto_RSlopTile", m_RSlopAutoTileVec);
	AutoTileMapAdd(L"Auto_LSlopTile", m_LSlopAutoTileVec);
	AutoTileMapAdd(L"Auto_UpTile", m_UpAutoTileVec);
	AutoTileMapAdd(L"Auto_LeftTile", m_LeftAutoTileVec);
	AutoTileMapAdd(L"Auto_RightTile", m_RightAutoTileVec);
	AutoTileMapAdd(L"Auto_DownTile", m_DownAutoTileVec);
	AutoTileMapAdd(L"Auto_RDownTile", m_RDSlopAutoTileVec);				// 우하단경사타일
	AutoTileMapAdd(L"Auto_LDownTile", m_LDSlopAutoTileVec);				// 좌하단경사타일

	AutoTileMapAdd(L"Auto_SDownTile", m_SDownAutoTileVec);

	AutoTileMapAdd(L"Auto_SDownLHalfTile", m_SDownAutoLHalfTileVec);		// 좌하단
	AutoTileMapAdd(L"Auto_SDownRHalfTile", m_SDownAutoRHalfTileVec);		// 우하단
	AutoTileMapAdd(L"Auto_SUpLHalfTile", m_SUpAutoLHalfTileVec);			// 좌상단
	AutoTileMapAdd(L"Auto_SUpRHalfTile", m_SUpAutoRHalfTileVec);			// 우상단
}

// 자동타일 벡터 묶기
void LandMapScript::AutoTileMapAdd(std::wstring _TileType, std::vector<std::wstring> _TileVector)
{
	std::unordered_map<std::wstring, std::vector<std::wstring>>::iterator FindIter = m_AllAutoTileMap.find(_TileType);

	// 이전의 타일이 존재한다면 (이미 있다면) 현재 선택의 이전의 타일이니 삭제해준다..
	if (m_AllAutoTileMap.end() != FindIter)
	{
		m_AllAutoTileMap.erase(FindIter);
	}

	m_AllAutoTileMap.insert(std::unordered_map<std::wstring, std::vector<std::wstring>>::value_type(_TileType, _TileVector));
}

// 어떤 타일인지 검사하고 각 타일벡터에 넣어주기
void LandMapScript::DrawTileNamesInit(std::wstring _TileImageName)
{
	std::wstring IamgeNameCheck = _TileImageName;

	// 얇은 타일인지, 경사타일인지, 하단 경사타일인지 체크하고 넣어줘야함.

	if (std::wstring::npos != IamgeNameCheck.find(L"_bsc."))
	{
		// 찍을 수 있는 큰타일
		// 얇은 타일인지 체크
		m_BigTileNames.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_edU."))
	{
		// 찍을 수 있는 작은타일
		m_SmallTileNames.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_slRU."))
	{
		// 오른쪽 경사타일
		m_RSlopAutoTileVec.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_slLU."))
	{
		// 왼쪽 경사타일
		m_LSlopAutoTileVec.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_enH0."))
	{
		// 위쪽 타일
		m_UpAutoTileVec.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_enV0."))
	{
		// 왼쪽 타일
		m_LeftAutoTileVec.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_enV1."))
	{
		// 오른쪽 타일
		m_RightAutoTileVec.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_enH1."))
	{
		// 아래 타일(기본형)
		m_DownAutoTileVec.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_slRD."))
	{
		// 오른쪽 아래 타일(방향존재)
		m_RDSlopAutoTileVec.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_slLD."))
	{
		// 왼쪽 아래 타일(방향존재)
		m_LDSlopAutoTileVec.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_edD."))
	{
		// 작은 아래타일 (이거 하나뿐임)
		m_SDownAutoTileVec.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_edDHL."))
	{
		// 작은 아래 왼쪽 반쪽타일
		m_SDownAutoLHalfTileVec.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_edDHR."))
	{
		// 작은 아래 오른쪽 반쪽타일 
		m_SDownAutoRHalfTileVec.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_edUHL."))
	{
		// 작은 위 왼쪽 반쪽타일
		m_SUpAutoLHalfTileVec.push_back(IamgeNameCheck);
	}
	else if (std::wstring::npos != IamgeNameCheck.find(L"_edUHR."))
	{
		// 작은 위 오른쪽 반쪽타일
		m_SUpAutoRHalfTileVec.push_back(IamgeNameCheck);
	}
	else
	{
		int a = 0;
		// 여긴 절대로 들어오면안된다.
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
		m_bSlope = false;		// 경사체크 X
	}
	else
	{
		m_bSlope = true;		// 경사체크 O
	}
}

void LandMapScript::SetDownSlopCheck(int _Value)
{
	if (0 == _Value)
	{
		m_bDownSlope = false;		// 경사체크 X
	}
	else
	{
		m_bDownSlope = true;		// 경사체크 O
	}
}

void LandMapScript::SetSlimCheck(int _Value)
{
	if (0 == _Value)
	{
		m_bSlimTile = false;		// 얇기체크 X
	}
	else
	{
		m_bSlimTile = true;		// 얇기체크 O
	}
}


std::wstring LandMapScript::RandomTile()
{
	// 랜덤 번호 부여
	int RandomValue = -1;
	std::wstring ImageName = L"";

	if (true == m_bSlimTile)
	{
		// 얇은 타일의 경우
		m_bSmallTile = false;
		RandomValue = rand() % m_UpAutoTileVec.size();
		ImageName = m_UpAutoTileVec[RandomValue];
	}
	else
	{
		// 얇은타일이 아닌경우
		if (true == m_bBigTile)
		{
			// 큰타일
			m_bSmallTile = false;
			RandomValue = rand() % m_BigTileNames.size();
			ImageName = m_BigTileNames[RandomValue];
		}
		else
		{
			// 작은타일 - 쪼꼬미
			m_bSmallTile = true;
			RandomValue = rand() % m_SmallTileNames.size();
			ImageName = m_SmallTileNames[RandomValue];
		}
	}


	if (-1 == RandomValue)
	{
		return L"";
	}

	//std::wstring Number = std::to_wstring(RandomValue);		// 번호
	//std::wstring FileName = L"grassySoil.img_bsc." + Number + L".png";

	return ImageName;
}

// 화면 밖 체크
bool LandMapScript::ClickSceneRangeExcess()
{
	// 화면밖은 true, 화면 안이라면 false
	// 카메라 + 현재 보여주는 씬을 기준으로 판단한다. (가로 1024, 세로 768)
	Vector2 ClickPosition = Vector2(m_CameraPTR->GetTransform()->Local_Position().x + GetScene()->GetCamera()->WMousePosition().x, m_CameraPTR->GetTransform()->Local_Position().y + GetScene()->GetCamera()->WMousePosition().y);

	// 화면 에따른 ..최대치랑 최저치
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
		// 순서
		pixelsSize[i] = 0x00ff0000;  // ARGB  -> 단순 초기화
	}


	std::list<My_PTR<LandRenderer>>::iterator StartLand = m_LandMapList.begin();
	std::list<My_PTR<LandRenderer>>::iterator EndLand = m_LandMapList.end();

	for (; StartLand != EndLand; ++StartLand)
	{
		// 안에서 그려주기
		(*StartLand)->DrawLandMap(pixelsSize, pRealImage->width, pRealImage->height);
	}

	pRealImage->rowPitch = SaveSceneX * sizeof(int); // SaveImage->width * SaveImage->format 의 바이트 사이즈 (모두 1바이트니까 ARGB -> 4바이트)
	pRealImage->slicePitch = pRealImage->rowPitch * pRealImage->height;	// rowPitch * 높이

	std::wstring FName = _FileName + L".png";

	std::wstring FileName = Path::FindPath(L"SaveImage");
	FileName += FName;


	HRESULT hr = DirectX::SaveToWICFile(*pRealImage, DirectX::WIC_FLAGS_NONE, GUID_ContainerFormatPng, FileName.c_str());



	////hr = D3DX11SaveTextureToFile(Device::Get_Context(), (ID3D11Resource*)(m_Sprite->GetTexture()->GetTexture2D()), D3DX11_IFF_PNG, FileName.c_str());

	delete pixelsSize;
	delete pRealImage;
}


// 기타 관련
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