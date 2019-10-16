#include "TitleScene.h"
#include <GameObject.h>
#include <DXHeader.h>
#include <Camera.h>
#include <Animation2D.h>
#include <SpriteRenderer2D.h>
#include <SoundPlayer.h>
#include <Input.h>
#include <Scene.h>
#include <BGFade.h>

TitleScene::TitleScene() : m_bFade(false), m_bSceneChange(false), m_CurTime(0.0f), m_ChangeTime(3.0f)
{
}


TitleScene::~TitleScene()
{
}
//

bool TitleScene::Loading()
{
	BGMSoundLoading();
	EffectSoundLoading();
	BGLoading();
	CreateCamera();
	return true;
}

void TitleScene::Start()
{
	SoundPlay_BGM(L"Title.mp3");
}

void TitleScene::Update()
{
	if (true == Input::Down(L"Key_Space"))
	{
		SoundPlay_Effect(L"NxLogo.mp3");
		m_bSceneChange = true;
		m_BGFadeScript->m_bFadeDown = true;
	}

	if (true == m_bSceneChange)
	{
		if (m_CurTime >= m_ChangeTime)
		{
			m_BGMPlayer->Stop_BGM();
			Scene::Scene_Change(L"ArcanaTownScene");
		}
		m_CurTime += Time::DeltaTime();
	}
}

void TitleScene::LateUpdate()
{

}

void TitleScene::End()
{

}

void TitleScene::DebugRender()
{

}

//

void TitleScene::CreateCamera()
{
	// 메인 카메라
	m_MainCamera = GetScene()->GameObject_Create(L"MainCamera");

	m_CameraCom = m_MainCamera->Component_Add<Camera>(0);
	m_CameraCom->SettingViewGroup(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133);
	m_CameraCom->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -20.0f));

}

void TitleScene::BGLoading()
{
	m_BG = GetScene()->GameObject_Create(L"TitleBG");
	m_BG->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, 0.0f, 0.0f));
	m_BG->GetTransform()->Local_Scale(Vector4(1024.0f, 768.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> BGRenderer = m_BG->Component_Add<SpriteRenderer2D>(RENDER_ORDER::BG_Layer02);
	BGRenderer->SetSprite(L"TitleBG.png", 0);

	m_BGFade = GetScene()->GameObject_Create(L"BGFade");
	m_BGFade->GetTransform()->Local_Position(Vector4(0.0f, 0.0f, -19.0f, 1.0f));
	m_BGFade->GetTransform()->Local_Scale(Vector4(1024.0f, 768.0f, 1.0f, 1.0f));
	My_PTR<SpriteRenderer2D> BGFadeRenderer = m_BGFade->Component_Add<SpriteRenderer2D>(RENDER_ORDER::BG_Layer02);
	BGFadeRenderer->SetSprite(L"FadeInOutBG.png", 0);
	m_BGFadeScript = m_BGFade->Component_Add<BGFade>();
}


//
void TitleScene::BGMSoundLoading()
{
	// 사운드플레이
	m_BGMEmptyObject = GetScene()->GameObject_Create(L"BGMPlayer");
	m_BGMEmptyObject->GetTransform()->Local_Position(Vector4::Zero);
	m_BGMEmptyObject->GetTransform()->Local_Scale(Vector4::Zero);
	m_BGMPlayer = m_BGMEmptyObject->Component_Add<SoundPlayer>();
}

void TitleScene::SoundPlay_BGM(std::wstring _SoundFileName)
{
	m_BGMPlayer->Play_BGM(_SoundFileName.c_str());
}


void TitleScene::EffectSoundLoading()
{
	m_EffectSoundObject = GetScene()->GameObject_Create(L"EffectSoundPlayer");
	m_EffectSoundObject->GetTransform()->Local_Position(Vector4::Zero);
	m_EffectSoundObject->GetTransform()->Local_Scale(Vector4::Zero);
	m_EffectSoundPlayer = m_EffectSoundObject->Component_Add<SoundPlayer>();
}

void TitleScene::SoundPlay_Effect(std::wstring _SoundFileName)
{
	m_EffectSoundPlayer->Play(_SoundFileName.c_str());
}