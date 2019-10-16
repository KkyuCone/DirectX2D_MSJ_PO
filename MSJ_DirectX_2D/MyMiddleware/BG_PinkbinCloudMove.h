#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>

class m_Paleyr;
class BG_PinkbinCloudMove : public Script
{
private:
	GameObject* m_Paleyr;

public:
	void SetPlayer(GameObject* _Obj);

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	BG_PinkbinCloudMove();
	~BG_PinkbinCloudMove();
};

