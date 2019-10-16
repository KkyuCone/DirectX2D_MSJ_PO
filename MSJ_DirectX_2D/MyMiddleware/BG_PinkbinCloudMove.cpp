#include "BG_PinkbinCloudMove.h"



BG_PinkbinCloudMove::BG_PinkbinCloudMove() : m_Paleyr(nullptr)
{
}


BG_PinkbinCloudMove::~BG_PinkbinCloudMove()
{
}
//

bool BG_PinkbinCloudMove::Init()
{
	return true;
}

void BG_PinkbinCloudMove::Update()
{
	if (nullptr == m_Paleyr)
	{
		return;
	}

	float XPos = GetTransform()->Local_Position().x;

	if (true == Input::DownStay(L"Left"))
	{
		if (m_Paleyr->GetTransform()->Local_Position().x <= GetTransform()->Local_Position().x)
		{
			XPos -= 768;
			if (XPos <= -894.0f)
			{
				return;
			}
			else
			{
				GetTransform()->Local_Move(Vector4::Left * 50.0f * Time::DeltaTime());
			}
		}
	}
	else if (true == Input::DownStay(L"Right"))
	{
		if (m_Paleyr->GetTransform()->Local_Position().x >= GetTransform()->Local_Position().x)
		{
			XPos += 768;
			if (XPos >= 894.0f)
			{
				return;
			}
			else
			{
				GetTransform()->Local_Move(Vector4::Right * 50.0f * Time::DeltaTime());
			}
		}
	}

}

void BG_PinkbinCloudMove::FinalUpdate()
{

}

//

void BG_PinkbinCloudMove::SetPlayer(GameObject* _Obj)
{
	m_Paleyr = _Obj;
}
