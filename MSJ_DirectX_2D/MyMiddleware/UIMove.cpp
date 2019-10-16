#include "UIMove.h"
#include <GameObject.h>



UIMove::UIMove() 
	: m_BasePosX(0.0f), m_BasePosY(0.0f)
	, m_pCamera(nullptr)
{
}


UIMove::~UIMove()
{
}

// overrid 함수
bool UIMove::Init()
{
	return true;
}

void UIMove::Update()
{
	//Move();
}

void UIMove::FinalUpdate()
{

}

// private 함수
void UIMove::Move()
{
	if (nullptr == m_pCamera)
	{
		return;
	}

	//GetTransform()->Local_Position(Vector4(m_pCamera->GetTransform()->World_Position().x + m_BasePosX, m_pCamera->GetTransform()->World_Position().y + m_BasePosY, m_BasePosZ, 1.0f));
	float a = GetTransform()->Local_Position().z;
	GetTransform()->World_Position(Vector4(m_pCamera->GetTransform()->World_Position().x + m_BasePosX, m_pCamera->GetTransform()->World_Position().y + m_BasePosY, GetTransform()->Local_Position().z, 1.0f));
}


// public 함수
// Get, Set
void UIMove::SetCamera(GameObject* _Obj)
{
	m_pCamera = _Obj;
}

void UIMove::SetBasePos(Vector4 _Pos)
{
	m_BasePosX = _Pos.x;
	m_BasePosY = _Pos.y;
	m_BasePosZ = _Pos.z;
}

// 일반 함수