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
		std::vector<int> m_InputKey;  // �Է¹��� Ű ����
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
		// ��͸� ���������� ���� ���
		void Push_Key()
		{

		}

	public:
		// Ű üũ
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
		My_PTR<KeyData> NewKey = Find_Key(_key); // Űã��

		if (NewKey != nullptr)
		{
			// �̹� Ű�� �����Ѵٸ� ������.
			EAssert(true);
			return false;
		}

		// Ű�� ���ٸ� Ű �߰�
		NewKey = new KeyData(sizeof...(_arg));	// ��� ����Ű���� �˱�����
		NewKey->Push_Key(_arg...);	// Ű�ֱ�

		m_InputMapKeyData.insert(std::map<std::wstring, My_PTR<KeyData>>::value_type(_key, NewKey));

		return true;

	}

private:
	// �Ʒ��� Update�Լ��� ����� �ٸ������� �����ϸ� �ȵȴ�.(����)
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

