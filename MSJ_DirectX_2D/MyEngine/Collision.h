#pragma once
//#include "Component.h"
#include "SubTransform.h"
#include "GameMath.h"
#include <unordered_set>
#include <functional>

// 업데이트 도중에 충돌도 할거임 ㅇㅅㅇ
// 움직이지 않는 오브젝트들(장애물같은거..)과 충돌할때 유용함

enum COL_TYPE
{
	CT_NONE,
	CT_POINT_2D,
	CT_RECT_2D,
	CT_CIRCLE_2D,
	CT_POLY_2D,		// 안만듬 만들어 보셈
	CT_RAY_3D,		// 광선
	CT_SPHERE_3D,	// 구
	CT_AABB_3D,		// 회전하지않는 육면체
	CT_OBB_3D,		// 회전하는 육면체
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
	// 충돌체관련 렌더
	static My_PTR<VertexShader> m_ColVertexShader;			// 버텍스쉐이더
	static My_PTR<PixelShader> m_ColPixelShader;			// 픽셀쉐이더
	static My_PTR<Mesh> m_ColMesh;							// 메쉬

protected:
	const COL_TYPE m_eColType;
	std::wstring m_ColGroupName;		// 콜라이더 그룹이름

public:

	// 콜라이더 그룹 이름 Get, Set
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

	virtual void CollisionDataUpdate() {}			// 상속 구조를 내려서 해당하는 콜라이더(사각형인지 원인지..)업데이트들 실행해주려고 virtual

	void Collision_Release();						// 콜리전 삭제

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

