#include "WorldMouseCursorScript.h"
#include <Camera.h>


WorldMouseCursorScript::WorldMouseCursorScript()
{
}


WorldMouseCursorScript::~WorldMouseCursorScript()
{
}

//

bool WorldMouseCursorScript::Init()
{
	return true;
}

void WorldMouseCursorScript::Update()
{
	WorldMousePos();
}

void WorldMouseCursorScript::FinalUpdate()
{

}

// Get, Set
void WorldMouseCursorScript::SetCamera(Camera* _Camera)
{
	m_Camera = _Camera;
}

//
void WorldMouseCursorScript::WorldMousePos()
{
	Vector2 CurMousePos = m_Camera->WMosuePositionCamera();
	Vector4 ChangeMousePos = GetTransform()->Local_Position();
	ChangeMousePos.x = CurMousePos.x + 8.0f;
	ChangeMousePos.y = CurMousePos.y - 10.0f;

	GetTransform()->Local_Position(ChangeMousePos);
}
