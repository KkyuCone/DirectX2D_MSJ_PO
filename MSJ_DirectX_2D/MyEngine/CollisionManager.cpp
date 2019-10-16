#include "CollisionManager.h"
#include "STLHelper.h"
#include "EnMACRO.h"

///////////////////////////////////////////// ColGroup

void CollisionManager::CollisionGroup::PushCollision(Collision* _Col)
{
	m_ColList.push_back(_Col);
}

// 콜리전 그룹에 있는애들 콜라이더 돌리기 ( 자기자신인지 아니면 다른 콜리전인지 확인하고 콜리전 실행)
void CollisionManager::CollisionGroup::Col()
{
	std::list<CollisionGroup*>::iterator Col_StartIter = m_OtherColCroup.begin();
	std::list<CollisionGroup*>::iterator Col_EndIter = m_OtherColCroup.end();

	for (; Col_StartIter != Col_EndIter; ++Col_StartIter)
	{
		if (this != (*Col_StartIter))
		{
			OtherCollision((*Col_StartIter));
		}
		else
		{
			ThisCollision();
		}
	}
}

// 다른 콜리전 그룹
void CollisionManager::CollisionGroup::OtherCollision(CollisionGroup* _Other)
{
	// ex 여기에 A (1개)
	std::list<My_PTR<Collision>>::iterator ThisCol_StartIter = m_ColList.begin();
	std::list<My_PTR<Collision>>::iterator ThisCol_EndIter = m_ColList.end();

	// ex 여기에 B,C (2개)
	std::list<My_PTR<Collision>>::iterator OtherCol_StartIter;
	std::list<My_PTR<Collision>>::iterator OtherCol_EndIter;

	for (; ThisCol_StartIter != ThisCol_EndIter; ++ThisCol_StartIter)
	{
		if (false == (*ThisCol_StartIter)->IsUpdate())
		{
			// 만약 업데이트를 하지않는 충돌체 (사망한 충돌체)의 경우 돌릴필요가 없으므로 continue
			continue;
		}

		// 다른 콜리전은 그때그때마다 돌려야해서 여기서 넣어줘야함..
		OtherCol_StartIter = _Other->m_ColList.begin();
		OtherCol_EndIter = _Other->m_ColList.end();
		for (; OtherCol_StartIter != OtherCol_EndIter; ++OtherCol_StartIter)
		{
			if (false == (*ThisCol_StartIter)->IsUpdate())
			{
				break;
			}

			if (false == (*OtherCol_StartIter)->IsUpdate())
			{
				continue;
			}

			(*ThisCol_StartIter)->IsCollide(*OtherCol_StartIter);
		}
	}
}

// 자기 콜리전 
void CollisionManager::CollisionGroup::ThisCollision()
{

}

void CollisionManager::CollisionGroup::Release()
{
	// 삭제
	std::list<My_PTR<Collision>>::iterator Start_Iter = m_ColList.begin();
	std::list<My_PTR<Collision>>::iterator End_Iter = m_ColList.end();

	for (; Start_Iter != End_Iter;)
	{
		(*Start_Iter)->Collision_Release();		// 콜리전 삭제 (콜리전셋에서 삭제)

		if (true == (*Start_Iter)->IsDeath())
		{
			Start_Iter = m_ColList.erase(Start_Iter);		// 콜리전 리스트에서 해당 콜리전 삭제
		}
		else
		{
			++Start_Iter;
		}
	}
}



void CollisionManager::CollisionGroup::CollisionDataUpdate()
{
	std::list<My_PTR<Collision>>::iterator Start_Iter = m_ColList.begin();
	std::list<My_PTR<Collision>>::iterator End_Iter = m_ColList.end();

	for (; Start_Iter != End_Iter; ++Start_Iter)
	{
		// 내가 추가한거.. 
		if (false == (*Start_Iter)->IsUpdate())
		{
			continue;
		}
		(*Start_Iter)->CollisionDataUpdate();		// 콜리전데이터 업데이트 실행
	}
}

void CollisionManager::CollisionGroup::DebugRender()
{
	std::list<My_PTR<Collision>>::iterator Start_Iter = m_ColList.begin();
	std::list<My_PTR<Collision>>::iterator End_Iter = m_ColList.end();

	for (; Start_Iter != End_Iter; ++Start_Iter)
	{
		(*Start_Iter)->DebugRender();		// 디버그 렌더 실행
	}
}

void CollisionManager::CollisionGroup::Collision_Release()
{

}

///////////////////////////////////////// CollisionManager

CollisionManager::CollisionManager()
{
}


CollisionManager::~CollisionManager()
{
}

// 콜리전 그룹생성
My_PTR<CollisionManager::CollisionGroup> CollisionManager::CreateGroup(const wchar_t* _Name)
{
	return m_ColMap.insert(std::unordered_map<std::wstring, My_PTR<CollisionGroup>>::value_type(_Name, new CollisionGroup())).first->second;
}

// 콜리전 연결해주기
void CollisionManager::Link(const wchar_t* _LeftColName, const wchar_t* _RightColName)
{
	My_PTR<CollisionGroup> LeftCol = Map_Find_PTR<My_PTR<CollisionGroup>>(m_ColMap, _LeftColName);
	My_PTR<CollisionGroup> RightCol = Map_Find_PTR<My_PTR<CollisionGroup>>(m_ColMap, _RightColName);

	if (nullptr == LeftCol)
	{
		// 없으면 만들어줌
		LeftCol = CreateGroup(_LeftColName);
	}

	if (nullptr == RightCol)
	{
		RightCol = CreateGroup(_RightColName);
	}

	// ㅇㅅㅇ;; 이미있는거 또 링크걸지말자~~ 체크해주는거 안했음
	LeftCol->m_OtherColCroup.push_back(RightCol);
}


void CollisionManager::PushCollision(Collision* _Col)
{
	My_PTR<CollisionGroup> ColGroup = Map_Find_PTR<My_PTR<CollisionGroup>>(m_ColMap, _Col->GetGroupName());

	if (nullptr == ColGroup)
	{
		// 없다면 그룹 만들어주기 ㅇㅅㅇ
		// 그리고 난 후에 콜리전 넣어주기
		ColGroup = CreateGroup(_Col->GetGroupName());
	}

	ColGroup->PushCollision(_Col);
}

void CollisionManager::Release()
{
	// 삭제
	std::unordered_map<std::wstring, My_PTR<CollisionGroup>>::iterator Start_Iter = m_ColMap.begin();
	std::unordered_map<std::wstring, My_PTR<CollisionGroup>>::iterator End_Iter = m_ColMap.end();

	for (; Start_Iter != End_Iter; ++Start_Iter)
	{
		Start_Iter->second->Release();
	}
}

void CollisionManager::Col()
{
	std::unordered_map<std::wstring, My_PTR<CollisionGroup>>::iterator StartIter = m_ColMap.begin();
	std::unordered_map<std::wstring, My_PTR<CollisionGroup>>::iterator EndIter = m_ColMap.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		StartIter->second->CollisionDataUpdate();
	}

	StartIter = m_ColMap.begin();
	EndIter = m_ColMap.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		StartIter->second->Col();
	}
}

// 디버그 렌더
void CollisionManager::DebugRender()
{
	std::unordered_map<std::wstring, My_PTR<CollisionGroup>>::iterator Start_Iter = m_ColMap.begin();
	std::unordered_map<std::wstring, My_PTR<CollisionGroup>>::iterator End_Iter = m_ColMap.end();

	for ( ; Start_Iter != End_Iter; ++Start_Iter)
	{
		Start_Iter->second->DebugRender();
	}
}