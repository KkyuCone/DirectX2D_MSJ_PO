#include "HPBarMoveScript.h"



HPBarMoveScript::HPBarMoveScript() : m_Monster(nullptr), m_YPivot(0.0f)
{
}


HPBarMoveScript::~HPBarMoveScript()
{
}


//

bool HPBarMoveScript::Init()
{
	return true;
}

void HPBarMoveScript::Update()
{
	if (nullptr == m_Monster)
		return;
	
	Move();
}

void HPBarMoveScript::FinalUpdate()
{

}

//

void HPBarMoveScript::Move()
{
	Vector4 ChangePos = m_Monster->GetTransform()->Local_Position();
	ChangePos.y += m_YPivot;

	GetTransform()->Local_Position(ChangePos);
}

//

void HPBarMoveScript::SetMosnter(GameObject* _Monster)
{
	m_Monster = _Monster;
}

void HPBarMoveScript::SetPivot(float _Pos)
{
	m_YPivot = _Pos;
}