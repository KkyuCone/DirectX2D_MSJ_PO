#include "InGameCamera.h"
#include "PlayerMove.h"
#include <Input.h>
#include "PlayerMove.h"
#include "Weather_Snow.h"
#include <Scene.h>


InGameCamera::InGameCamera() 
	: m_MapWidth(0.0f), m_MapHeight(0.0f)
	, m_SceneHX(SCENEW/2), m_SceneHY(SCENEH/2)
	, m_MaxSpeed(0.0f), m_MinSpeed(40.0f), m_AccelSpeed(0.0f), m_MaxJumpForce(600.0f), m_AccelSpeedY(0.0f), m_MinJumpForce(80.0f)
	, m_bMove(false), m_bRMove(false), m_bLMove(false)
	, m_bNewSetBGSize(false), m_BGMinX(0.0f), m_BGMaxX(0.0f)
{
}


InGameCamera::~InGameCamera()
{
}


//
bool InGameCamera::Init()
{
	m_Snow = GetScene()->GameObject_Create(L"Snow");
	m_Snow->GetTransform()->Local_Position();

	return true;
}

void InGameCamera::Update()
{
	Move();
}

void InGameCamera::FinalUpdate()
{

}

//
void InGameCamera::Move()
{
	// x축 방향 이동
	if (true == Input::DownStay(L"Left"))
	{
		m_bMove = true;
		LMove();

	}
	else if (true == Input::DownStay(L"Right"))
	{
		m_bMove = true;
		RMove();
	}

	if (true == Input::UpStay(L"Left"))
	{
		if (true == m_bMove)
		{
			if (true == m_bLMove)
			{
				LUMove();
			}
		}
		else
		{
			m_AccelSpeed = 0.0f;
		}
	}

	if (true == Input::UpStay(L"Right"))
	{
		if (true == m_bMove)
		{
			if (true == m_bRMove)
			{
				RUMove();
			}
		}
		else
		{
			m_AccelSpeed = 0.0f;
		}
	}


	// y축 방향 이동
	m_YResult = m_pPlayer->GetTransform()->World_Position().y - GetTransform()->World_Position().y;

	if (0 <= m_YResult)
	{
		if (FSMState::IDLE == m_pPlayer->GetComponent<PlayerMove>()->GetState())
		{
			UUMove();
		}
		else
		{
			UMove();
		}
	}
	else
	{
		if (FSMState::IDLE == m_pPlayer->GetComponent<PlayerMove>()->GetState())
		{
			DUMove();
		}
		else
		{
			DMove();
		}
	}
}


void InGameCamera::LMove()
{
	m_bLMove = true;
	m_bRMove = false;

	if (GetTransform()->Local_Position().x >= m_pPlayer->GetTransform()->Local_Position().x)
	{
		if (true == m_bNewSetBGSize)
		{
			if (GetTransform()->Local_Position().x >= (float)(m_BGMinX) + (float)(SCENEW / 2))
			{
				Vector4 MovePos = Vector4::Left * m_AccelSpeed * Time::DeltaTime();
				GetTransform()->Local_Move(MovePos);

				m_AccelSpeed += 50.0f * Time::DeltaTime();

				if (m_AccelSpeed >= m_MaxSpeed)
				{
					m_AccelSpeed = m_MaxSpeed;
				}
			}
		}
		else
		{
			if (GetTransform()->Local_Position().x >= (float)((-m_MapWidth) / 2) + (float)(SCENEW / 2))
			{
				Vector4 MovePos = Vector4::Left * m_AccelSpeed * Time::DeltaTime();
				GetTransform()->Local_Move(MovePos);

				m_AccelSpeed += 50.0f * Time::DeltaTime();

				if (m_AccelSpeed >= m_MaxSpeed)
				{
					m_AccelSpeed = m_MaxSpeed;
				}
			}
		}
	}
}

void InGameCamera::RMove()
{
	m_bLMove = false;
	m_bRMove = true;

	if (GetTransform()->Local_Position().x <= m_pPlayer->GetTransform()->Local_Position().x)
	{
		if (true == m_bNewSetBGSize)
		{
			if (GetTransform()->Local_Position().x <= (float)(m_BGMaxX) - (float)(SCENEW / 2))
			{
				Vector4 MovePos = Vector4::Right * m_AccelSpeed * Time::DeltaTime();
				GetTransform()->Local_Move(MovePos);

				m_AccelSpeed += 50.0f * Time::DeltaTime();

				if (m_AccelSpeed >= m_MaxSpeed)
				{
					m_AccelSpeed = m_MaxSpeed;
				}
			}
		}
		else
		{
			if (GetTransform()->Local_Position().x <= (float)(m_MapWidth / 2) - (float)(SCENEW / 2))
			{
				Vector4 MovePos = Vector4::Right * m_AccelSpeed * Time::DeltaTime();
				GetTransform()->Local_Move(MovePos);

				m_AccelSpeed += 50.0f * Time::DeltaTime();

				if (m_AccelSpeed >= m_MaxSpeed)
				{
					m_AccelSpeed = m_MaxSpeed;
				}
			}
		}
	}
}


void InGameCamera::LUMove()
{
	if (GetTransform()->Local_Position().x < m_pPlayer->GetTransform()->Local_Position().x)
	{
		m_AccelSpeed = 0.0f;
		return;
	}

	if (true == m_bNewSetBGSize)
	{
		if (GetTransform()->Local_Position().x <= (float)(m_BGMinX) + (float)(SCENEW / 2))
		{
			m_AccelSpeed = 0.0f;
			return;
		}
	}
	else
	{
		if (GetTransform()->Local_Position().x <= (float)((-m_MapWidth) / 2) + (float)(SCENEW / 2))
		{
			m_AccelSpeed = 0.0f;
			return;
		}
	}


	Vector4 MovePos = Vector4::Left * m_AccelSpeed * Time::DeltaTime();
	GetTransform()->Local_Move(MovePos);

	m_AccelSpeed -= 50.0f * Time::DeltaTime();

	if (m_AccelSpeed <= m_MinSpeed)
	{
		m_AccelSpeed = m_MinSpeed;
	}
}

void InGameCamera::RUMove()
{
	if (GetTransform()->Local_Position().x > m_pPlayer->GetTransform()->Local_Position().x)
	{
		m_AccelSpeed = 0.0f;
		return;
	}

	if (true == m_bNewSetBGSize)
	{
		if (GetTransform()->Local_Position().x >= (float)(m_BGMaxX) - (float)(SCENEW / 2))
		{
			m_AccelSpeed = 0.0f;
			return;
		}
	}
	else
	{
		if (GetTransform()->Local_Position().x >= (float)(m_MapWidth / 2) - (float)(SCENEW / 2))
		{
			m_AccelSpeed = 0.0f;
			return;
		}
	}

	Vector4 MovePos = Vector4::Right * m_AccelSpeed * Time::DeltaTime();
	GetTransform()->Local_Move(MovePos);

	m_AccelSpeed -= 50.0f * Time::DeltaTime();

	if (m_AccelSpeed <= m_MinSpeed)
	{
		m_AccelSpeed = m_MinSpeed;
	}
}


void InGameCamera::UMove()
{
	if (GetTransform()->Local_Position().y < m_pPlayer->GetTransform()->Local_Position().y)
	{
		if (GetTransform()->Local_Position().y + (float)(SCENEH / 2) < (float)(m_MapHeight / 2))
		{
			Vector4 MovePos = Vector4::Up * m_AccelSpeedY * Time::DeltaTime();
			GetTransform()->Local_Move(MovePos);

			m_AccelSpeedY += 100.0f * Time::DeltaTime();

			if (m_AccelSpeedY >= m_MaxJumpForce)
			{
				m_AccelSpeedY = m_MaxJumpForce;
			}
		}
	}
}

void InGameCamera::DMove()
{
	if (GetTransform()->Local_Position().y - 210.0f > m_pPlayer->GetTransform()->Local_Position().y)
	{
		if (GetTransform()->Local_Position().y - (float)(SCENEH / 2) > (float)((-m_MapHeight) / 2))
		{
			Vector4 MovePos = Vector4::Down * m_AccelSpeedY * Time::DeltaTime();
			GetTransform()->Local_Move(MovePos);

			m_AccelSpeedY += 100.0f * Time::DeltaTime();

			if (m_AccelSpeedY >= m_MaxJumpForce)
			{
				m_AccelSpeedY = m_MaxJumpForce;
			}
		}
	}
}

void InGameCamera::UUMove()
{
	if (GetTransform()->Local_Position().y > m_pPlayer->GetTransform()->Local_Position().y)
	{
		m_AccelSpeedY = 0.0f;
		return;
	}

	if (GetTransform()->Local_Position().y + (float)(SCENEH / 2) > (float)(m_MapHeight / 2))
	{
		m_AccelSpeedY = 0.0f;
		return;
	}


	Vector4 MovePos = Vector4::Up * m_AccelSpeedY * Time::DeltaTime();
	GetTransform()->Local_Move(MovePos);

	m_AccelSpeedY -= 100.0f * Time::DeltaTime();

	if (m_AccelSpeedY <= m_MinJumpForce)
	{
		m_AccelSpeedY = m_MinJumpForce;
	}
}

void InGameCamera::DUMove()
{
	if (GetTransform()->Local_Position().y - 210.0f < m_pPlayer->GetTransform()->Local_Position().y)
	{
		m_AccelSpeedY = 0.0f;
		return;
	}

	if (GetTransform()->Local_Position().y - (float)(SCENEH / 2) < (float)((-m_MapHeight) / 2))
	{
		m_AccelSpeedY = 0.0f;
		return;
	}


	Vector4 MovePos = Vector4::Down * m_AccelSpeedY * Time::DeltaTime();
	GetTransform()->Local_Move(MovePos);

	m_AccelSpeedY -= 100.0f * Time::DeltaTime();

	if (m_AccelSpeedY <= m_MinJumpForce)
	{
		m_AccelSpeedY = m_MinJumpForce;
	}
}


//
void InGameCamera::SetCurMap(GameObject* _CurMap)
{
	m_pCurMap = _CurMap;
	m_MapWidth = m_pCurMap->GetTransform()->Local_Scale().x;
	m_MapHeight = m_pCurMap->GetTransform()->Local_Scale().y;
}

void InGameCamera::SetPlayer(GameObject* _Player)
{
	m_pPlayer = _Player;
	m_MaxSpeed = m_pPlayer->GetComponent<PlayerMove>()->GetMoveSpeed();
}

void InGameCamera::SetMovePos(Vector4 _Pos)
{
	GetTransform()->Local_Move(_Pos);
}

void InGameCamera::SetMoveCheck(bool _Value)
{
	m_bMove = _Value;
}

void InGameCamera::SetSceneXSize(float _MinX, float _MaxX)
{
	m_bNewSetBGSize = true;
	
	float NewMinValue = -(m_MapWidth / 2) + _MinX;
	m_BGMinX = NewMinValue;

	float HalfValue = m_MapWidth / 2;
	
	float NewValue = m_MapWidth - _MaxX;
	NewValue = (m_MapWidth - NewValue);
	NewValue = (NewValue - HalfValue);
	m_BGMaxX = NewValue;
}