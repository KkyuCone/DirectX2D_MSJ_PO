#pragma once
//#include "Component.h"
#include "SubTransform.h"
#include "GameMath.h"
#include <unordered_set>
#include <functional>

// ������Ʈ ���߿� �浹�� �Ұ��� ������
// �������� �ʴ� ������Ʈ��(��ֹ�������..)�� �浹�Ҷ� ������

enum COL_TYPE
{
	CT_NONE,
	CT_POINT_2D,
	CT_RECT_2D,
	CT_CIRCLE_2D,
	CT_POLY_2D,		// �ȸ��� ����� ����
	CT_RAY_3D,		// ����
	CT_SPHERE_3D,	// ��
	CT_AABB_3D,		// ȸ�������ʴ� ����ü
	CT_OBB_3D,		// ȸ���ϴ� ����ü
	CT_MESH_3D,
	CT_MAX
};


class VertexShader;
class PixelShader;
class Mesh;
class Collision : public SubTransform
{
	friend class CollisionManager;


protected:
	// �浹ü���� ����
	static My_PTR<VertexShader> m_ColVertexShader;			// ���ؽ����̴�
	static My_PTR<PixelShader> m_ColPixelShader;			// �ȼ����̴�
	static My_PTR<Mesh> m_ColMesh;							// �޽�

protected:
	const COL_TYPE m_eColType;
	std::wstring m_ColGroupName;		// �ݶ��̴� �׷��̸�

public:

	// �ݶ��̴� �׷� �̸� Get, Set
	const wchar_t* GetGroupName()
	{
		return m_ColGroupName.c_str();
	}

	void SetGroupName(const wchar_t* _Name)
	{
		m_ColGroupName = _Name;
	}

private:
	std::list<std::function<void(Collision*, Collision*)>>::iterator Col_StartIter;
	std::list<std::function<void(Collision*, Collision*)>>::iterator Col_EndIter;

	std::list<std::function<void(Collision*, Collision*)>> m_EnterColList;
	std::list<std::function<void(Collision*, Collision*)>> m_StayColList;
	std::list<std::function<void(Collision*, Collision*)>> m_ExitColList;

private:
	void Collision_Enter(Collision* _OtherCol);
	void Collision_Stay(Collision* _OtherCol);
	void Collision_Exit(Collision* _OtherCol);

	virtual void CollisionDataUpdate() {}			// ��� ������ ������ �ش��ϴ� �ݶ��̴�(�簢������ ������..)������Ʈ�� �������ַ��� virtual

	void Collision_Release();						// �ݸ��� ����

public:
	// Enter
	template<class T>
	void PushCollisionEnter(void(T::*pFunc)(Collision*, Collision*), T* _Obj)
	{
		// 
		std::function<void(Collision*, Collision*)> NewColFunc = std::bind(pFunc, _Obj, std::placeholders::_1, std::placeholders::_2);

		if (nullptr == NewColFunc)
		{
			EAssert(true);
			return;
		}
		m_EnterColList.push_back(NewColFunc);
	}

	// Stay
	template<class T>
	void PushCollisionStay(void(T::*pFunc)(Collision*, Collision*), T* _Obj)
	{
		// 
		std::function<void(Collision*, Collision*)> NewColFunc = std::bind(pFunc, _Obj, std::placeholders::_1, std::placeholders::_2);

		if (nullptr == NewColFunc)
		{
			EAssert(true);
			return;
		}
		m_StayColList.push_back(NewColFunc);
	}

	// Exit
	template<class T>
	void PushCollisionExit(void(T::*pFunc)(Collision*, Collision*), T* _Obj)
	{
		// 
		std::function<void(Collision*, Collision*)> NewColFunc = std::bind(pFunc, _Obj, std::placeholders::_1, std::placeholders::_2);

		if (nullptr == NewColFunc)
		{
			EAssert(true);
			return;
		}
		m_ExitColList.push_back(NewColFunc);
	}

public:
	inline COL_TYPE GetColType() { return m_eColType; }


private:
	std::unordered_set<Collision*> m_ColSet;
	

public:
	bool Init(const wchar_t* _Name = L"Default");

protected:
	virtual void Collision_Init();

public:
	virtual bool IsCollide(Collision* _OtherCol);
	virtual bool Collision_Check(Collision* _OtherCol) = 0;

public:
	Collision(COL_TYPE _eType);
	~Collision();
};

