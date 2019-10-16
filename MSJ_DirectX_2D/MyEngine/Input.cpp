#include "Input.h"
#include "Window.h"
#include "Time.h"

std::map<std::wstring, My_PTR<Input::KeyData>>::iterator Input::Start_Iter;
std::map<std::wstring, My_PTR<Input::KeyData>>::iterator Input::End_Iter;
std::map<std::wstring, My_PTR<Input::KeyData>> Input::m_InputMapKeyData;

POINT Input::MousePosition;



/////////////////////////////////////////////// KeyData

bool Input::KeyData::Key_Check()
{
	for (size_t i = 0; i < m_InputKey.size(); i++)
	{
		// 해당 키가 눌리지 않았다면
		if (0 == GetAsyncKeyState(m_InputKey[i]))
		{
			return false;
		}
	}
	return true;
}

void Input::KeyData::Update()
{
	bool KeyCheck_Vlaue = Key_Check();	// 키가 눌렸는지 체크

	if (true == KeyCheck_Vlaue)
	{
		// 키가 눌렸을때
		if (true == m_ButtonUpStay)
		{
			PushTime += Time::DeltaTime();

			// 눌린 그 순간
			m_ButtonUp = false;
			m_ButtonUpStay = false;
			m_ButtonDown = true;		// 키가 눌린 한순간
			m_ButttonDownStay = true;
		}
		else if (false == m_ButtonUpStay)
		{
			PushTime += Time::DeltaTime();

			// 눌린 순간이 지났을때 ( 키를 누르고 뗐을때 )
			m_ButtonUp = false;
			m_ButtonUpStay = false;
			m_ButtonDown = false;
			m_ButttonDownStay = true;
		}
	}
	else if (false == KeyCheck_Vlaue)
	{
		// 키가 안눌렸을때
		if (true == m_ButttonDownStay)
		{
			m_ButtonUp = true;
			m_ButtonUpStay = true;
			m_ButtonDown = false;
			m_ButttonDownStay = false;
		}
		else if (false == m_ButttonDownStay)
		{
			m_ButtonUp = false;
			m_ButtonUpStay = true;
			m_ButtonDown = false;
			m_ButttonDownStay = false;
		}
	}
}

/////////////////////////////////////////////// Input


Input::Input()
{
}


Input::~Input()
{
}


bool Input::IsKey(const wchar_t* _Name)
{
	My_PTR<KeyData> pData = Find_Key(_Name);

	if (nullptr == pData)
	{
		return false;
	}

	return true;
}


void Input::Update()
{
	GetCursorPos(&MousePosition);
	ScreenToClient(Window::WindowHWND(), &MousePosition);

	Start_Iter = m_InputMapKeyData.begin();
	End_Iter = m_InputMapKeyData.end();

	for (; Start_Iter != End_Iter; Start_Iter++)
	{
		if (nullptr != Start_Iter->second)
		{
			Start_Iter->second->Update();
		}
	}
}

bool Input::Up(const wchar_t * _name)
{
	My_PTR<KeyData> mp_KeyData = Find_Key(_name);	// 키 찾기
	EAssert(nullptr == mp_KeyData);

	if (nullptr == mp_KeyData)
	{
		return false;
	}
	return mp_KeyData->m_ButtonUp;
}

bool Input::Down(const wchar_t * _name)
{
	My_PTR<KeyData> mp_KeyData = Find_Key(_name);	// 키 찾기
	EAssert(nullptr == mp_KeyData);

	if (nullptr == mp_KeyData)
	{
		return false;
	}
	return mp_KeyData->m_ButtonDown;
}

bool Input::UpStay(const wchar_t * _name)
{
	My_PTR<KeyData> mp_KeyData = Find_Key(_name);	// 키 찾기
	EAssert(nullptr == mp_KeyData);

	if (nullptr == mp_KeyData)
	{
		return false;
	}
	return mp_KeyData->m_ButtonUpStay;
}

bool Input::DownStay(const wchar_t * _name)
{
	My_PTR<KeyData> mp_KeyData = Find_Key(_name);	// 키 찾기
	EAssert(nullptr == mp_KeyData);

	if (nullptr == mp_KeyData)
	{
		return false;
	}
	return mp_KeyData->m_ButttonDownStay;
}

float Input::PushTimer(const wchar_t * _Name)
{
	My_PTR<KeyData> mp_KeyData = Find_Key(_Name);
	EAssert(nullptr == mp_KeyData);

	if (nullptr == mp_KeyData)
	{
		return false;
	}

	return mp_KeyData->PushTime;
}

bool Input::PushTimerOverReset(const wchar_t * _Name, float _Time)
{
	My_PTR<KeyData> mp_KeyData = Find_Key(_Name);
	EAssert(nullptr == mp_KeyData);

	if (nullptr == mp_KeyData)
	{
		return false;
	}

	if (_Time <= mp_KeyData->PushTime)
	{
		mp_KeyData->PushTime = 0.0f;
		return true;
	}


	return false;
}
