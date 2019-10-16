#include "Inven_MoveScript.h"
#include <GameObject.h>
#include <Camera.h>


Inven_MoveScript::Inven_MoveScript() : m_bMove(false), m_BasePos(Vector4::One)
{
}


Inven_MoveScript::~Inven_MoveScript()
{
}


//

bool Inven_MoveScript::Init()
{
	return true;
}

void Inven_MoveScript::Update()
{
	if (true == m_bMove)
	{
		Move();
	}
}

void Inven_MoveScript::FinalUpdate()
{

}

//
void Inven_MoveScript::SetMoveObject(GameObject* _MoveOBject)
{
	m_InventoryMoveObject = _MoveOBject;		// 움직일 부모 오브젝트
}

GameObject* Inven_MoveScript::GetMoveObejct()
{
	return m_InventoryMoveObject;
}

void Inven_MoveScript::SetCamera(Camera* _Camera)
{
	m_UICamera = _Camera;
}

void Inven_MoveScript::SetbMove(bool _Value)
{
	m_bMove = _Value;

	if (true == m_bMove)
	{
		m_BasePos = m_InventoryMoveObject->GetTransform()->Local_Position();
		m_MouseBasePos = m_UICamera->WMousePosition();
	}
}

bool Inven_MoveScript::GetbMove()
{
	return m_bMove;
}

//
void Inven_MoveScript::Move()
{
	if (true == Input::Up(L"MouseLeftButton")) 
	{
		SetbMove(false);
	}


	Vector2 CurMousePos = m_UICamera->WMousePosition();

	Vector2 ChangeMPos = CurMousePos;
	ChangeMPos.x -= m_MouseBasePos.x;
	ChangeMPos.y -= m_MouseBasePos.y;


	Vector4 ChangeNewVector = m_BasePos;
	ChangeNewVector.x += ChangeMPos.x;
	ChangeNewVector.y += ChangeMPos.y;

	m_InventoryMoveObject->GetTransform()->Local_Position(ChangeNewVector);
}