#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

#define COOLTIMETEXCOUNT 17

class CoolTimeScript : public Script
{
private:
	float m_CoolTime;
	float m_CurTime;
private:
	My_PTR<SpriteRenderer2D> m_Renderer;
	My_PTR<Animation2D> m_Animaiton;

private:
	bool m_CoolEnd;

private:
	void CoolTimeEndCheck();

public:
	void ChangeCoolTimeAni();

public:
	bool GetCoolEnd();
	void SetCoolTimeChange(float _Time);
	void SetCoolTimeAnimationFrame(int _Frame);
	void SetCoolTimeAnimationCurTime(float _Time);

	std::wstring GetCurAnimationName();
	float GetCurCoolTime();
	int GetCurCoolTimeFrame();

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	CoolTimeScript();
	~CoolTimeScript();
};

