#include "RectCol2D.h"
#include "GameMath.h"
#include "Transform.h"
#include "SubTransform.h"

#include"Scene.h"
#include"Camera.h"
#include "DXHeader.h"

#include "VertexShader.h"
#include "PixelShader.h"
#include "Mesh.h"

RectCol2D::RectCol2D() : Collision(CT_RECT_2D), m_UICamera(false)
{

}


RectCol2D::~RectCol2D()
{
}


bool RectCol2D::Collision_Check(Collision* _OtherCol)
{

	switch (_OtherCol->GetColType())
	{
	case CT_RECT_2D:
		return GameMath::FRectToRect(Data, ((RectCol2D*)_OtherCol)->Data);
	default:
		break;
	}

	return false;
}

void RectCol2D::DebugRender()
{
	if (false == IsUpdate())
	{
		return;
	}

	// 여기서 사각형 띄우기 (렌더러)
	// ex ) GameDebug::DrawRect()

	VCOLOR Color = VCOLOR::NONECOLOR;
	SubTransformUpdate();

	m_ColVertexShader->UpdateConstBuffer<Matrix>(L"ColDebugWVP", (SubConstRefWorldMatrix() * VP).Get_Transpos());
	m_ColPixelShader->UpdateConstBuffer<VCOLOR>(L"COLCOLOR", Color);


	m_ColMesh->Update();
	m_ColVertexShader->Update();
	m_ColPixelShader->Update();
	m_ColMesh->Render();
}

void RectCol2D::CollisionDataUpdate()
{

	if (true == m_bUI)
	{
		VP = m_UICamera->GetViewProj();
	}
	else
	{
		VP = GetGameObject()->GetScene()->GetCamera()->GetViewProj();
	}

	if (nullptr != m_UICamera)
	{
		VP = m_UICamera->GetViewProj();

		if (nullptr == GetSubTransform())
		{
			Data.Pos = GetTransform()->World_Positon2D();
			Data.Size = GetTransform()->Local_Scale2D();
		}
		else
		{
			float a = GetSubTransform()->m_vScale.x;
			float b = GetSubTransform()->m_vScale.y;
			Data.Pos.x = GetTransform()->World_Positon2D().x + GetSubTransform()->m_vPivot.x;
			Data.Pos.y = GetTransform()->World_Positon2D().y + GetSubTransform()->m_vPivot.y;
			//Data.Pos = Vector2{ GetSubTransform()->m_vPivot.x, GetSubTransform()->m_vPivot.y };
			Data.Size = Vector2{ GetSubTransform()->m_vScale.x, GetSubTransform()->m_vScale.y };
		}
		Data.Size.FABS();
	}
	else
	{
		if (nullptr == GetSubTransform())
		{
			Data.Pos = GetTransform()->Local_Position2D();
			Data.Size = GetTransform()->Local_Scale2D();
		}
		else
		{
			float a = GetSubTransform()->m_vScale.x;
			float b = GetSubTransform()->m_vScale.y;
			Data.Pos.x = GetTransform()->Local_Position2D().x + GetSubTransform()->m_vPivot.x;
			Data.Pos.y = GetTransform()->Local_Position2D().y + GetSubTransform()->m_vPivot.y;
			//Data.Pos = Vector2{ GetSubTransform()->m_vPivot.x, GetSubTransform()->m_vPivot.y };
			Data.Size = Vector2{ GetSubTransform()->m_vScale.x, GetSubTransform()->m_vScale.y };
		}
		Data.Size.FABS();
	}

}
void RectCol2D::SetColSize(Vector2 _Size)
{
	Data.Size = _Size;
}

Vector2 RectCol2D::GetColSize()
{
	return Data.Size;
}

void RectCol2D::SetUICamera(Camera* _UICamera)
{
	m_UICamera = _UICamera;
	m_bUI = true;
}