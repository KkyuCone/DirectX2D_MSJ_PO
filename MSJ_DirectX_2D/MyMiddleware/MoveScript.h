#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class MoveScript : public Script
{
private:
	//float m_fAccTime;


private:
	float MoveSpeed;
	My_PTR<SpriteRenderer2D> m_SpriteRender;
	My_PTR<Animation2D> m_Animation;
	size_t m_Index;



	//Transform CentralTransform;


public:
	bool Init() override;
	void Update() override;

public:
	MoveScript();
	~MoveScript();
};

