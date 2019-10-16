#include "ItemMoveScript.h"
#include <GameObject.h>
#include <Texture.h>
#include <Sprite.h>
#include <math.h>


ItemMoveScript::ItemMoveScript() : m_CurTime(0.0f), m_JumpForce(ITEMFORCE), m_bDownCheck(true), m_ItemState(1), m_bDrop(false)
{
}


ItemMoveScript::~ItemMoveScript()
{
}

//
bool ItemMoveScript::Init()
{
	m_Renderer = GetComponent<SpriteRenderer2D>();

	if (nullptr == m_Renderer)
	{
		EAssert(true);
		return false;
	}

	int randValue = rand() % 4 + 1;

	std::wstring ItemSpriteName = L"Item_Money0";
	ItemSpriteName = ItemSpriteName + std::to_wstring(randValue)+ L".png";
	m_Renderer->SetSprite(ItemSpriteName.c_str(), 0);

	if (3 == randValue)
	{
		GetTransform()->Local_Scale(Vector4(33.0f,33.0f, 1.0f, 1.0f));
	}
	else if (4 == randValue)
	{
		GetTransform()->Local_Scale(Vector4(32.0f, 31.0f, 1.0f, 1.0f));
	}

	m_Animation = GetComponent<Animation2D>();

	if (nullptr == m_Animation)
	{
		EAssert(true);
		return false;
	}

	m_Animation->Animation_Create(L"Ani", 0, 3, true, 0.1f);
	m_Animation->Animation_Change(L"Ani");

	return true;
}

void ItemMoveScript::Update()
{
	//GDown();

	if (1 == m_ItemState)
	{
		ItemJumpUp();
	}
	else if (2 == m_ItemState)
	{
		ItemJumpDown();

		if (true == m_bDrop)
		{
			float CurMoneyPosX = GetTransform()->Local_Position().x;
			float CurPlayerPosX = m_Player->GetTransform()->Local_Position().x;

			float XDecValue = CurMoneyPosX - CurPlayerPosX;

			if (XDecValue >= 0.0f)
			{
				// 오른편
				GetTransform()->Local_Move(Vector4::Left * 100.0f * Time::DeltaTime());
			}
			else
			{
				GetTransform()->Local_Move(Vector4::Right * 100.0f * Time::DeltaTime());
			}

			float CurMoneyPos = GetTransform()->Local_Position().y;
			float CurPlayerPos = m_Player->GetTransform()->Local_Position().y;

			if (CurMoneyPos <= CurPlayerPos - 20.0f)
			{
				GetGameObject()->Death();
				//m_Renderer->Update_Off();
			}
		}
	}
}

void ItemMoveScript::FinalUpdate()
{

}

//

void ItemMoveScript::ItemJumpUp()
{
	int a = 0;
	// 점프중인 상태일때
	// 점프의 최대치에 도달했을때
	if (0.0f >= m_JumpForce)
	{
		m_JumpForce = 0.0f;
		m_ItemState = 2;
		return;
	}


	Vector4 JumpPos = Vector4::Up * m_JumpForce * Time::DeltaTime();

	Vector4 MovePos = JumpPos /*+ DirPos*/;
	GetTransform()->Local_Move(MovePos);

	m_JumpForce -= 1700.0f * Time::DeltaTime();
}

void ItemMoveScript::ItemJumpDown()
{
	// 아래로 떨어지는 상태

	Vector4 JumpPos = Vector4::Up * m_JumpForce * Time::DeltaTime();

	Vector4 MovePos = JumpPos /*+ DirPos*/;
	GetTransform()->Local_Move(MovePos);

	m_JumpForce -= 1700.0f * Time::DeltaTime();

	if (m_JumpForce <= -((ITEMFORCE) * 2))
	{
		m_JumpForce = -(ITEMFORCE) * 2;
	}

	 //바닥에 부딪혔을때 IDLE (멈추기)

	// 픽셀충돌하기
	Vector4 TestVec = Vector4{ -1, -1, -1, -1 };


	TestVec = m_Floors->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->ImageGetPixel((unsigned int)(GetTransform()->World_Position().x), (unsigned int)(GetTransform()->World_Position().y - (GetTransform()->Local_Scale().y / 2) - 15));

	if (TestVec.a > 0)
	{
		m_JumpForce = ITEMFORCE;
		m_ItemState = 0;
		GetTransform()->Local_Move(Vector4::Up);
		return;
	}
}

//

void ItemMoveScript::SetItemState(int _Value)
{
	m_ItemState = _Value;
}

void ItemMoveScript::SetPlayer(GameObject* _Obj)
{
	m_Player = _Obj;
}

void ItemMoveScript::SetFloor(GameObject* _Floor)
{
	m_Floors = _Floor;
}

//

void ItemMoveScript::GDown()
{
	Vector4 TestVec = Vector4{ -1, -1, -1, -1 };
	Vector4 TestVec2 = Vector4{ -1, -1, -1, -1 };
	Vector4 TestVec3 = Vector4{ -1, -1, -1, -1 };
	Vector4 TestVec4 = Vector4{ -1, -1, -1, -1 };
	Vector4 Pos = GetTransform()->World_Position();

	TestVec = m_Floors->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->ImageGetPixel((unsigned int)(Pos.x), (unsigned int)(Pos.y - (GetTransform()->Local_Scale().y / 2) - 15));
	TestVec2 = m_Floors->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->ImageGetPixel((unsigned int)(Pos.x), (unsigned int)(Pos.y - (GetTransform()->Local_Scale().y / 2) - (15 - 1)));
	TestVec3 = m_Floors->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->ImageGetPixel((unsigned int)(Pos.x), (unsigned int)(Pos.y - (GetTransform()->Local_Scale().y / 2) - (15 - 2)));
	TestVec4 = m_Floors->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->ImageGetPixel((unsigned int)(Pos.x), (unsigned int)(Pos.y - (GetTransform()->Local_Scale().y / 2) - (15 - 1)));

	if (TestVec3.a == 0 && (m_ItemState == 1 /*|| m_PlayerState == RUN*/))
	{
		m_bDownCheck = true;
	}

	if (TestVec4.a == 0 && (m_ItemState == 2))
	{
		//m_bDownCheck = true;
		Vector4 yMove = Vector4{ 0.0f, -1.0f, 0.0f };
		GetTransform()->Local_Move(yMove);
	}

	if (TestVec2.a == 0)
	{
		if (m_ItemState != 2 && m_bDownCheck == true)
		{
			m_JumpForce = 0.0f;
			//m_ItemState = 1;
		}
	}

	if (TestVec.a != 0)
	{
		m_bDownCheck = false;

		Vector4 Pos = GetTransform()->World_Position();
		TestVec = m_Floors->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->ImageGetPixel((unsigned int)(Pos.x), (unsigned int)(Pos.y - (GetTransform()->Local_Scale().y / 2) - 15));
		while (TestVec.a != 0)
		{
			Pos.y += 1.0f;
			TestVec = m_Floors->GetComponent<SpriteRenderer2D>()->GetSprite()->GetTexture()->ImageGetPixel((unsigned int)(Pos.x), (unsigned int)(Pos.y - (GetTransform()->Local_Scale().y / 2) - 15));
		}
		GetTransform()->World_Position(Pos);
	}
}

void ItemMoveScript::SetDrop(bool _Value)
{
	m_bDrop = _Value;

	if (true == m_bDrop)
	{
		m_ItemState = 1;
	}
}
