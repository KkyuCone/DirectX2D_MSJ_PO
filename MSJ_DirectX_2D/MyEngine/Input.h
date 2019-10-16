#pragma once
#include "DXHeader.h"
#include <Windows.h>
#include <vector>
#include <map>
#include "ReferenceBase.h"

class Input
{
public:
	class KeyData : public ReferenceBase
	{
	private:
		friend Input;
	private:
		std::vector<int> m_InputKey;  // 입력받은 키 저장
		bool m_ButtonUp;
		bool m_ButtonUpStay;
		bool m_ButtonDown;
		bool m_ButttonDownStay;

		float PushTime;

	private:
		template<typename... Variable_Argument>
		void Push_Key(const int _val, Variable_Argument... _arg)
		{
			m_InputKey.push_back(_val);
			Push_Key(_arg...);
		}
		// 재귀를 빠져나오기 위해 사용
		void Push_Key()
		{

		}

	public:
		// 키 체크
		void Update();
		bool Key_Check();

	public:
		KeyData(size_t _number) : m_ButtonUpStay(true), m_ButtonUp(false), m_ButtonDown(false), m_ButttonDownStay(false)
		{
			m_InputKey.reserve(_number);
		}
		~KeyData() {}
	};

public:
	friend class Window;

private:
	static std::map<std::wstring, My_PTR<KeyData>>::iterator Start_Iter;
	static std::map<std::wstring, My_PTR<KeyData>>::iterator End_Iter;
	static std::map<std::wstring, My_PTR<KeyData>> m_InputMapKeyData;

	static POINT MousePosition;

public:
	static Vector2_Int MousePos()
	{
		return{ (int)MousePosition.x, (int)MousePosition.y };
	}

	static Vector2 MousePos_F()
	{
		return{ (float)MousePosition.x, (float)MousePosition.y };
	}

private:
	static My_PTR<KeyData> Find_Key(const wchar_t* _key)
	{
		std::map<std::wstring, My_PTR<KeyData>>::iterator KeyFind_Iter = m_InputMapKeyData.find(_key);

		if (KeyFind_Iter == m_InputMapKeyData.end())
		{
			return nullptr;
		}

		return KeyFind_Iter->second;
	}

public:
	template<typename... Variable_Argument>
	static bool Create_Key(const wchar_t* _key, Variable_Argument... _arg)
	{
		My_PTR<KeyData> NewKey = Find_Key(_key); // 키찾기

		if (NewKey != nullptr)
		{
			// 이미 키가 존재한다면 터진다.
			EAssert(true);
			return false;
		}

		// 키가 없다면 키 추가
		NewKey = new KeyData(sizeof...(_arg));	// 몇개의 조합키인지 알기위해
		NewKey->Push_Key(_arg...);	// 키넣기

		m_InputMapKeyData.insert(std::map<std::wstring, My_PTR<KeyData>>::value_type(_key, NewKey));

		return true;

	}

private:
	// 아래의 Update함수는 절대로 다른곳에서 실행하면 안된다.(터짐)
	static void Update();

public:
	static bool IsKey(const wchar_t* _Name);

	static bool Up(const wchar_t* _name);
	static bool Down(const wchar_t* _name);
	static bool UpStay(const wchar_t* _name);
	static bool DownStay(const wchar_t* _name);

	static float PushTimer(const wchar_t* Name);
	static bool PushTimerOverReset(const wchar_t* _Name, float _Time);
public:
	Input();
	~Input();
};

