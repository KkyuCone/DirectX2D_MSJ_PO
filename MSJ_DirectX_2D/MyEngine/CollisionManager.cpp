#include "CollisionManager.h"
#include "STLHelper.h"
#include "EnMACRO.h"

///////////////////////////////////////////// ColGroup

void CollisionManager::CollisionGroup::PushCollision(Collision* _Col)
{
	m_ColList.push_back(_Col);
}

// �ݸ��� �׷쿡 �ִ¾ֵ� �ݶ��̴� ������ ( �ڱ��ڽ����� �ƴϸ� �ٸ� �ݸ������� Ȯ���ϰ� �ݸ��� ����)
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

// �ٸ� �ݸ��� �׷�
void CollisionManager::CollisionGroup::OtherCollision(CollisionGroup* _Other)
{
	// ex ���⿡ A (1��)
	std::list<My_PTR<Collision>>::iterator ThisCol_StartIter = m_ColList.begin();
	std::list<My_PTR<Collision>>::iterator ThisCol_EndIter = m_ColList.end();

	// ex ���⿡ B,C (2��)
	std::list<My_PTR<Collision>>::iterator OtherCol_StartIter;
	std::list<My_PTR<Collision>>::iterator OtherCol_EndIter;

	for (; ThisCol_StartIter != ThisCol_EndIter; ++ThisCol_StartIter)
	{
		if (false == (*ThisCol_StartIter)->IsUpdate())
		{
			// ���� ������Ʈ�� �����ʴ� �浹ü (����� �浹ü)�� ��� �����ʿ䰡 �����Ƿ� continue
			continue;
		}

		// �ٸ� �ݸ����� �׶��׶����� �������ؼ� ���⼭ �־������..
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

// �ڱ� �ݸ��� 
void CollisionManager::CollisionGroup::ThisCollision()
{

}

void CollisionManager::CollisionGroup::Release()
{
	// ����
	std::list<My_PTR<Collision>>::iterator Start_Iter = m_ColList.begin();
	std::list<My_PTR<Collision>>::iterator End_Iter = m_ColList.end();

	for (; Start_Iter != End_Iter;)
	{
		(*Start_Iter)->Collision_Release();		// �ݸ��� ���� (�ݸ����¿��� ����)

		if (true == (*Start_Iter)->IsDeath())
		{
			Start_Iter = m_ColList.erase(Start_Iter);		// �ݸ��� ����Ʈ���� �ش� �ݸ��� ����
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
		// ���� �߰��Ѱ�.. 
		if (false == (*Start_Iter)->IsUpdate())
		{
			continue;
		}
		(*Start_Iter)->CollisionDataUpdate();		// �ݸ��������� ������Ʈ ����
	}
}

void CollisionManager::CollisionGroup::DebugRender()
{
	std::list<My_PTR<Collision>>::iterator Start_Iter = m_ColList.begin();
	std::list<My_PTR<Collision>>::iterator End_Iter = m_ColList.end();

	for (; Start_Iter != End_Iter; ++Start_Iter)
	{
		(*Start_Iter)->DebugRender();		// ����� ���� ����
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

// �ݸ��� �׷����
My_PTR<CollisionManager::CollisionGroup> CollisionManager::CreateGroup(const wchar_t* _Name)
{
	return m_ColMap.insert(std::unordered_map<std::wstring, My_PTR<CollisionGroup>>::value_type(_Name, new CollisionGroup())).first->second;
}

// �ݸ��� �������ֱ�
void CollisionManager::Link(const wchar_t* _LeftColName, const wchar_t* _RightColName)
{
	My_PTR<CollisionGroup> LeftCol = Map_Find_PTR<My_PTR<CollisionGroup>>(m_ColMap, _LeftColName);
	My_PTR<CollisionGroup> RightCol = Map_Find_PTR<My_PTR<CollisionGroup>>(m_ColMap, _RightColName);

	if (nullptr == LeftCol)
	{
		// ������ �������
		LeftCol = CreateGroup(_LeftColName);
	}

	if (nullptr == RightCol)
	{
		RightCol = CreateGroup(_RightColName);
	}

	// ������;; �̹��ִ°� �� ��ũ��������~~ üũ���ִ°� ������
	LeftCol->m_OtherColCroup.push_back(RightCol);
}


void CollisionManager::PushCollision(Collision* _Col)
{
	My_PTR<CollisionGroup> ColGroup = Map_Find_PTR<My_PTR<CollisionGroup>>(m_ColMap, _Col->GetGroupName());

	if (nullptr == ColGroup)
	{
		// ���ٸ� �׷� ������ֱ� ������
		// �׸��� �� �Ŀ� �ݸ��� �־��ֱ�
		ColGroup = CreateGroup(_Col->GetGroupName());
	}

	ColGroup->PushCollision(_Col);
}

void CollisionManager::Release()
{
	// ����
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

// ����� ����
void CollisionManager::DebugRender()
{
	std::unordered_map<std::wstring, My_PTR<CollisionGroup>>::iterator Start_Iter = m_ColMap.begin();
	std::unordered_map<std::wstring, My_PTR<CollisionGroup>>::iterator End_Iter = m_ColMap.end();

	for ( ; Start_Iter != End_Iter; ++Start_Iter)
	{
		Start_Iter->second->DebugRender();
	}
}