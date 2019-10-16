#pragma once
#include "Collision.h"
#include "ReferenceBase.h"
#include "NameBase.h"

#include <list>
#include <unordered_map>

class CollisionManager
{
public:
	friend class Scene;
	friend Collision;

private:
	class CollisionGroup : public ReferenceBase, public NameBase
	{
	public:
		std::list<CollisionGroup*> m_OtherColCroup;
		std::list<My_PTR<Collision>> m_ColList;

	public:
		void PushCollision(Collision* _Col);
		void Col();
		void Release();

		void OtherCollision(CollisionGroup* _Other);
		void ThisCollision();

		// 충돌 데이터 업데이트 및 디버그 렌더러 + 콜라이더 삭제함수 (충돌그룹)
		void CollisionDataUpdate();
		void DebugRender();
		void Collision_Release();

	public:
		CollisionGroup() {}
		~CollisionGroup() {}
	};

private:
	std::unordered_map<std::wstring, My_PTR<CollisionGroup>> m_ColMap;

private:
	void PushCollision(Collision* _Col);
	My_PTR<CollisionGroup> CreateGroup(const wchar_t* _Name);

public:
	// 콜라이더 그룹 링크(연결)해주기
	void Link(const wchar_t* _LeftColName, const wchar_t* _RightColName);

public:
	void Release();
	void Col();
	void DebugRender();
public:
	CollisionManager();
	~CollisionManager();
};

