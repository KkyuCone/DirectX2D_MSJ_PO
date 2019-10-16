#include "MouseCursorScript.h"
#include <GameObject.h>
#include <Camera.h>


MouseCursorScript::MouseCursorScript() : m_Camera(nullptr)
{
}


MouseCursorScript::~MouseCursorScript()
{
}

//

bool MouseCursorScript::Init()
{
	m_Renderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_Renderer)
	{
		EAssert(true);
		return false;
	}

	m_Animation = GetComponent<Animation2D>();

	if (nullptr == m_Animation)
	{
		EAssert(true);
		return false;
	}

	m_Animation->Animation_Create(L"Base", 0, 0, true, 0.1f);
	m_Animation->Animation_Create(L"Click", 1, 2, true, 0.3f);
	m_Animation->Animation_Create(L"Catch", 3, 6, true, 0.2f);
	m_Animation->Animation_Change(L"Base");

	return true;
}

void MouseCursorScript::Update()
{
	if (nullptr == m_Camera)
		return;

	Move();
}

void MouseCursorScript::FinalUpdate()
{

}

void MouseCursorScript::SetCamera(Camera* _Camera)
{
	m_Camera = _Camera;
}

void MouseCursorScript::Move()
{
	Vector2 CurMousePos = m_Camera->WMousePosition();
	Vector4 ChangeMousePos = GetTransform()->Local_Position();
	ChangeMousePos.x = CurMousePos.x + 8.0f;
	ChangeMousePos.y = CurMousePos.y - 10.0f;

	GetTransform()->Local_Position(ChangeMousePos);
	//GetTransform()->World_Position(ChangeMousePos);
}

//
void MouseCursorScript::MouseStateChange(std::wstring _State)
{
	if (L"Base" == _State || L"Click" == _State || L"Catch" == _State)
	{
		m_Animation->Animation_Change(_State.c_str());
	}
}

