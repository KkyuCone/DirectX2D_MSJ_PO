#include "MapScript.h"
#include <MapRenderer.h>
#include <Texture.h>
#include <Sprite.h>
#include <Input.h>
#include <GameDebug.h>
#include "ToolCamera.h"

#include <stdlib.h>


std::wstring MapScript::SelectTile = L"";
bool MapScript::m_bPickStart = false;
// My_PTR<MapRenderer> MapScript::MapRender = nullptr;


MapScript::MapScript()
{
}


MapScript::~MapScript()
{
}


//

bool MapScript::Init()
{
	// 이미지
	ImageLoading();

	//// 기본형
	std::wstring PathName = L"grassySoil";
	SelectTile = PathName + L"bsc.2.png";
	//if (nullptr == ResourceManager<Texture>::Find(L"bsc.2.png"))
	//{
	//	//std::vector<std::wstring> ALLPathName = Path::FindPath(L"grassySoil");
	//	std::wstring ALLPathName = Path::FindPath(L"grassySoil");

	//	ResourceManager<Texture>::Load(PathName + L"bsc.2.png");
	//	ResourceManager<Sprite>::Create(L"bsc.2.png", L"bsc.2.png", 1, 1);

	//	SelectTile = PathName + L"bsc.2.png";
	//}


	m_CamerObject = GetScene()->GameObject_Create(L"Camera");

	m_CaemraPTR = m_CamerObject->Component_Add<Camera>();
	m_CaemraPTR->SettingViewGroup(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
	m_CaemraPTR->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -100.f));

	m_CameraScript = m_CamerObject->Component_Add<ToolCamera>();

	m_MapObject = GetScene()->GameObject_Create(L"TileMap");
	//MapRender = m_MapObject->Component_Add<MapRenderer>(0);
	//MapRender->SetMapTileSprite(L"bsc.2.png");
	//MapRender->SetMapTileSprite(SelectTile.c_str());
	
	SetMapRenderFolderPath();


	// 맵찍게..만들기ㅇㅅㅇ
	if (false == Input::IsKey(L"MapPick"))
	{
		Input::Create_Key(L"MapPick", VK_LBUTTON);
	}


	return true;
}

bool MapScript::Loading()
{
	return true;
}

void MapScript::Start()

{

}

void MapScript::Update()
{
	// 마우스 좌클릭시

	if (true == Input::Up(L"MapPick"))
	{
		m_CurRender = nullptr;
		return;
	}

	if (true == Input::DownStay(L"MapPick") && nullptr == m_CurRender)
	{
		m_CurRender = m_MapObject->Component_Add<MapRenderer>(1);
		Vector4 Pos = Vector4(GetScene()->GetCamera()->WMousePosition().x, GetScene()->GetCamera()->WMousePosition().y, 0.0f, 0.0f);
		// m_MapObject->GetTransform()->Local_Position(Pos);
		m_CurRender->SetTileSize(Vector2(90.0f, 60.0f));
		m_CurRender->SetMapTileSprite(L"bsc.2.png");
		m_CurRender->CreateTile(Vector2(Pos.x, Pos.y));
	}

	if (true == Input::DownStay(L"MapPick") && nullptr != m_CurRender) 
	{
		Vector4 Pos = Vector4(GetScene()->GetCamera()->WMousePosition().x, GetScene()->GetCamera()->WMousePosition().y, 0.0f, 0.0f);
		m_CurRender->CreateTile(Vector2(Pos.x, Pos.y));
	}

}

void MapScript::LateUpdate()
{

}

void MapScript::End()
{

}

void MapScript::DebugRender()
{

	// 마우스 포스
	wchar_t Arr[256];
	swprintf_s(Arr, L"WorldMousePos = %f, %f", GetScene()->GetCamera()->WMousePosition().x, GetScene()->GetCamera()->WMousePosition().y);
	GameDebug::DrawStr(Arr, 20, { 10.0f, 30.0f }, { 1.0f, 1.0f, 1.0f }, FW1_TEXT_FLAG::FW1_TOP);

	// FPS
	wchar_t CurFPS[256];
	swprintf_s(CurFPS, L"FPS = %d", Time::FPS());
	GameDebug::DrawStr(CurFPS, 20, { 10.0f, 60.0f }, { 1.0f, 1.0f, 1.0f }, FW1_TEXT_FLAG::FW1_TOP);




	//GameDebug::DrawStr(MPosX.c_str(), 20, { 20.0f, 4.0f, }, { 1.0f, 1.0f, 1.0f }, FW1_TEXT_FLAG::FW1_TOP);
	//GameDebug::DrawStr(MPosY.c_str(), 20, { 20.0f, 24.0f, }, { 1.0f, 1.0f, 1.0f }, FW1_TEXT_FLAG::FW1_TOP);
}


//
void MapScript::ImageLoading()
{
	// 이미지 파일(PNG)들 가져오기
	//PathFileLoad(L"arcana1");				// Arcana1
	//PathFileLoad(L"arcana2");				// Arcana2
	//PathFileLoad(L"blueToyCastle");			// BlueToyCastle
	//PathFileLoad(L"BM2_1");					// BM2_1
	//PathFileLoad(L"BM2_2");					// BM2_2
	//PathFileLoad(L"critias");				// Critias
	//PathFileLoad(L"darkGrassySoil");		// DarkGrassySoil
	//PathFileLoad(L"destructionField");		// DestructionField
	//PathFileLoad(L"flagDay");				// FlagDay
	PathFileLoad(L"grassySoil");			// GrassySoil
	PathFileLoad(L"greenToyCastle");		// GreenToyCastle
	PathFileLoad(L"logMarble");				// LogMarble
	PathFileLoad(L"zero_templeFiled");		// Zero_templeFiled
}


void MapScript::PathFileLoad(std::wstring _RootPathFolder)
{
	std::vector<std::wstring> CharacterImages = Path::PathToFullFileList(Path::FindPath(_RootPathFolder.c_str()), L"*.png");
	for (size_t i = 0; i < CharacterImages.size(); ++i)
	{
		std::wstring NewName = _RootPathFolder + CharacterImages[i];

		ResourceManager<Texture>::Load(Path::FindPath(_RootPathFolder.c_str()) + CharacterImages[i]);
		ResourceManager<Sprite>::Create(CharacterImages[i], CharacterImages[i].c_str(), 1, 1);

		//ResourceManager<Sprite>::Create(CharacterImages[i], NewName.c_str(), 1, 1);
	}

	if (0 != CharacterImages.size())
	{
		ALLFolderPath.push_back(Path::FindPath(_RootPathFolder.c_str()));
	}
}

void MapScript::SetMapRenderFolderPath()
{
	//for (size_t i = 0; i < ALLFolderPath.size(); i++)
	//{
	//	MapRender->ALLFolderPath.push_back(ALLFolderPath[i]);
	//}
}