#include "MonsterCreator.h"
#include <GameObject.h>
#include <DXHeader.h>
#include <Camera.h>
#include <Animation2D.h>
#include <SpriteRenderer2D.h>
#include <MoveScript.h>
#include <Texture.h>
#include <Sprite.h>



MonsterCreator::MonsterCreator() : NewActor(nullptr), TestActor(nullptr)
{
}


MonsterCreator::~MonsterCreator()
{
}

// 게임오브젝트 생성
My_PTR<GameObject> MonsterCreator::GameObject2D_Create(const wchar_t* _Name)
{
	My_PTR<GameObject> NewGameObject = GetScene()->GameObject_Create(_Name);
	My_PTR<SpriteRenderer2D> NewRender = NewGameObject->Component_Add<SpriteRenderer2D>(5);

	if (nullptr != NewRender)
	{
		NewRender->SetSprite(L"unit_bazzi.png", 5);
	}

	NewGameObject->GetTransform()->Local_Scale(Vector4(200.0f, 200.0f, 200.0f));
	NewGameObject->Component_Add<Animation2D>();
	NewGameObject->DebugMode_On();

	return NewGameObject;
}

// Scene이 로딩 되지 않았을 때 한번만 실행함.
bool MonsterCreator::Loading()
{
	// Textrue
	std::vector<std::wstring> TextureImages = Path::PathToFullFileList(Path::FindPath(L"Texture"), L"*.png");

	for (size_t i = 0; i < TextureImages.size(); i++)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"Texture") + TextureImages[i]);
	}

	// Character
	int a = 0;
	std::vector<std::wstring> CharacterImages = Path::PathToFullFileList(Path::FindPath(L"Character"), L"*.png");

	for (size_t i = 0; i < CharacterImages.size(); i++)
	{
		ResourceManager<Texture>::Load(Path::FindPath(L"Character") + CharacterImages[i]);
	}

	//ResourceManager<Texture>::Load(Path::FindPath(L"Texture") + L"ladder.0.backHair.png");

	//ResourceManager<Sprite>::Create(L"unit_bazzi.png", L"unit_bazzi.png", 10, 7);

	ResourceManager<Sprite>::Create(L"ladder.0.backHair.png", L"ladder.0.backHair.png", 1, 1);


	// 카메라 1 
	NewActor = GetScene()->GameObject_Create(L"Camera");

	My_PTR<Camera> Ptr = NewActor->Component_Add<Camera>(0);
	Ptr->SettingViewGroup(0, 1, 2, 3, 4, 5);
	Ptr->GetTransform()->Local_Position(Vector4(-1.0f, 0.0f, -10.0f));

	//NewActor->Component_Add<MoveScript>();



	// 카메라 2 - UI용
	//My_PTR<GameObject> UICamera = GetScene()->GameObject_Create(L"UICamera");

	//My_PTR<Camera> UIPtr = UICamera->Component_Add<Camera>(0);
	//UIPtr->SettingViewGroup(6);
	//UIPtr->GetTransform()->Local_Position(Vector4(-1.0f, 0.0f, -10.0f));

	// 오브젝트들 

	// 1024
	// 768
	// 10.24
	// 7.68

	// 애니메이션
	//TestActor = GameObject2D_Create(L"Player");
	//TestActor->Component_Add<MoveScript>();
	//TestActor->DebugMode_On();



	TestActor = GetScene()->GameObject_Create(L"TestMesh");
	//TestActor->Component_Add<SpriteRenderer2D>(5);
	TestActor->Component_Add<SpriteRenderer2D>(5)->SetSprite(L"ladder.0.backHair.png");
	TestActor->GetTransform()->Local_Scale(Vector4(512.0f, 512.0f, 512.0f));
	TestActor->Component_Add<MoveScript>();

	//TestActor->DebugMode_On();

	// 자식오브젝트
	//My_PTR<GameObject> ChildActor = GetScene()->GameObject_Create(L"TestChild");
	//ChildActor->Component_Add<SpriteRenderer2D>(5);
	//ChildActor->GetTransform()->Local_Position(Vector4(2.0f, 2.0f, 0.0f));
	//ChildActor->GetTransform()->Local_Scale(Vector4(1.0f, 1.0f, 1.0f));
	//ChildActor->SetParent(TestActor);


	// 다른거 (플레이어)

	//My_PTR<GameObject> UnTestActor = GetScene()->GameObject_Create(L"UnneMesh");
	//UnTestActor->Component_Add<SpriteRenderer2D>(5);
	//UnTestActor->GetTransform()->Local_Position(Vector4(-100.0f, 0.0f, -1.0f));
	//UnTestActor->GetTransform()->Local_Scale(Vector4(512.0f, 512.0f, 100.0f));
	//UnTestActor->Component_Add<MoveScript>();




	//// UI
	//My_PTR<GameObject> UITestActor = GetScene()->GameObject_Create(L"UITestMesh");
	//UITestActor->Component_Add<SpriteRenderer2D>(6);
	//UITestActor->GetTransform()->Local_Position(Vector4(-400.0f, 300.0f, 0.0f));
	//UITestActor->GetTransform()->Local_Scale(Vector4(200.0f, 30.0f, 100.0f));


	//float A = 1.0f;
	//HMAT M;
	//for (size_t x = 0; x < 4; ++x)
	//{
	//	for (size_t y = x + 1; y < 4; ++y)
	//	{
	//		M.m[x][y] = A;
	//		++A;
	//	}
	//}


	return true;
}

// Scene이 변경되었을때 한번 실행
void MonsterCreator::Start()
{
	int a = 0;
}

// Scene의 게임오브젝트들의 업데이트가 실행되기 전에 한번 실행
Vector4 Speed = Vector4(1.0f, 1.0f, 1.0f);
void MonsterCreator::Update()
{

	if (Input::DownStay(L"Q"))
	{
		NewActor = nullptr;
		TestActor = nullptr;
	}
}

// 게임오브젝트들의 모든 업데이트가 끝난후에 한번 실행 
void MonsterCreator::LateUpdate()
{
	int a = 0;
}

// 현재 실행중인 Scene에서 다른 Scene으로 변경되었을 때 한번 실행 ( Scene이 끝날때..! )
void MonsterCreator::End()
{
	int a = 0;
}
