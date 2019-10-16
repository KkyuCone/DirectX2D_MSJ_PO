#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class QuestAlarm : public Script
{
private:
	My_PTR<SpriteRenderer2D> m_Renderer;
	My_PTR<Animation2D> m_Animation;

public:
	void ChangeAnimation(std::wstring _AniName);

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	QuestAlarm();
	~QuestAlarm();
};

