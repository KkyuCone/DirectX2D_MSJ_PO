#include "ToolCamera.h"
#include <Camera.h>



ToolCamera::ToolCamera()
	: m_LimitX(1024), m_LimitY(768), m_MoveSpeed(400.0f), m_LX(512.0f), m_LY(384.0f)
	, m_CameraPTR(nullptr)
{
}


ToolCamera::~ToolCamera()
{
}

//

bool ToolCamera::Init()
{
	InitKey();
	return true;
}

void ToolCamera::Update()
{
	ZoomInOut();
	Move();
}

//
void ToolCamera::SetLimitPos(int _SceneX, int _Scene_Y)
{
	if (500 > _SceneX || 500 > _Scene_Y)
	{
		return;
	}

	m_LimitX = _SceneX;
	m_LimitY = _Scene_Y;
}

void ToolCamera::Move()
{
	if (true == Input::DownStay(L"TMoveLeft"))
	{
		if (GetTransform()->Local_Position().x - (float)m_LX <= -(m_LimitX/2))
			return;

		GetTransform()->Local_Move(Vector4::Left * m_MoveSpeed * Time::DeltaTime());
	}
	else if (true == Input::DownStay(L"TMoveRight"))
	{
		if (GetTransform()->Local_Position().x + (float)m_LX >= (m_LimitX/2))
			return;

		GetTransform()->Local_Move(Vector4::Right * m_MoveSpeed * Time::DeltaTime());
	}
	else if (true == Input::DownStay(L"TMoveUp"))
	{
		if (GetTransform()->Local_Position().y + (float)m_LY >= (m_LimitY/2))
			return;

		GetTransform()->Local_Move(Vector4::Up * m_MoveSpeed * Time::DeltaTime());
	}
	else if (true == Input::DownStay(L"TMoveDown"))
	{
		if (GetTransform()->Local_Position().y - (float)m_LY <= -(m_LimitY/2))
			return;

		GetTransform()->Local_Move(Vector4::Down * m_MoveSpeed * Time::DeltaTime());
	}
}

void ToolCamera::ZoomInOut()
{
	if (nullptr == m_CameraPTR)
		return;

	if (true == Input::DownStay(L"ZoomIn"))
	{
		m_CameraPTR->ZoomIn();
	}
	else if (true == Input::DownStay(L"ZoomOut"))
	{
		m_CameraPTR->ZoomOut();
	}
}

void ToolCamera::SetCamera(My_PTR<Camera> _Camera)
{
	m_CameraPTR = _Camera;
}

void ToolCamera::InitKey()
{
	Input::Create_Key(L"TMoveLeft", 'A');
	Input::Create_Key(L"TMoveRight", 'D');
	Input::Create_Key(L"TMoveUp", 'W');
	Input::Create_Key(L"TMoveDown", 'S');

	Input::Create_Key(L"ZoomIn", 'C');
	Input::Create_Key(L"ZoomOut", 'V');
}
