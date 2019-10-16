#pragma once
#include <Scene.h>
#include <RenderOrder.h>
#include <EnumOrder.h>

class SoundPlayer;
class BGFade;
class TitleScene : public Scene::SceneScript, public RenderOrder, public EnumOrder
{
private:
	bool m_bFade;
	bool m_bSceneChange;

	float m_CurTime;
	float m_ChangeTime;
private:
	My_PTR<GameObject> m_BGMEmptyObject;
	My_PTR<SoundPlayer> m_BGMPlayer;

	My_PTR<GameObject> m_EffectSoundObject;
	My_PTR<SoundPlayer> m_EffectSoundPlayer;

private:
	My_PTR<GameObject> m_BG;
	My_PTR<GameObject> m_BGFade;
	My_PTR<BGFade> m_BGFadeScript;
private:
	My_PTR<GameObject> m_MainCamera;
	My_PTR<Camera> m_CameraCom;

private:
	// »ç¿îµå
	void BGMSoundLoading();
	void SoundPlay_BGM(std::wstring _SoundFileName);

	void EffectSoundLoading();
	void SoundPlay_Effect(std::wstring _SoundFileName);

private:
	void CreateCamera();
	void BGLoading();

public:
	bool Loading() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void End() override;
	void DebugRender() override;

public:
	TitleScene();
	~TitleScene();
};

