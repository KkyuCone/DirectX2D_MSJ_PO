#include "Collision.h"
#include "GameMath.h"
#include "Scene.h"

// �浹 �׸��׸���
#include "ResourceManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Mesh.h"

// �浹ü���� ����
My_PTR<VertexShader> Collision::m_ColVertexShader;			// ���ؽ����̴�
My_PTR<PixelShader> Collision::m_ColPixelShader;			// �ȼ����̴�
My_PTR<Mesh> Collision::m_ColMesh;							// �޽�

//
Collision::Collision(COL_TYPE _eType) : m_eColType(_eType)
{
}


Collision::~Collision()
{
	std::unordered_set<Collision*>::iterator ColSet_StartIter = m_ColSet.begin();
	std::unordered_set<Collision*>::iterator ColSet_EndIter = m_ColSet.end();

	for (; ColSet_StartIter != ColSet_EndIter; ++ColSet_StartIter)
	{
		Collision_Exit(*ColSet_StartIter);
	}
}

// �浹ü ����(�ڱⰡ �׾�����..��Ȯ���� ������Ʈ�� false�϶� ����-> ������Ʈ�� ���ϴ� ���¶�� �浹ü�� ���� �ʿ䰡 �����Ƿ� ����)
// ����ִ¾ֲ�.. �ݸ�������Ʈ ������..
// �����ֵ� ������..
void Collision::Collision_Release()
{
	std::unordered_set<Collision*>::iterator ColSet_StartIter = m_ColSet.begin();
	std::unordered_set<Collision*>::iterator ColSet_EndIter = m_ColSet.end();

	for (; ColSet_StartIter != ColSet_EndIter;)
	{
		if (false == (*ColSet_StartIter)->IsUpdate())
		{
			Collision_Exit(*ColSet_StartIter);
			ColSet_StartIter = m_ColSet.erase(ColSet_StartIter);
		}
		else
		{
			++ColSet_StartIter;
		}
	}
}

// Enter, Stay, Exit
void Collision::Collision_Enter(Collision* _OtherCol)
{
	Col_StartIter = m_EnterColList.begin();
	Col_EndIter = m_EnterColList.end();

	for (; Col_StartIter != Col_EndIter; ++Col_StartIter)
	{
		(*Col_StartIter)(this, _OtherCol);
	}
}

void Collision::Collision_Stay(Collision* _OtherCol)
{
	Col_StartIter = m_StayColList.begin();
	Col_EndIter = m_StayColList.end();

	for (; Col_StartIter != Col_EndIter; ++Col_StartIter)
	{
		(*Col_StartIter)(this, _OtherCol);
	}
}

void Collision::Collision_Exit(Collision* _OtherCol)
{
	Col_StartIter = m_ExitColList.begin();
	Col_EndIter = m_ExitColList.end();

	for (; Col_StartIter != Col_EndIter; ++Col_StartIter)
	{
		(*Col_StartIter)(this, _OtherCol);
	}
}

//

bool Collision::Init(const wchar_t* _Name)
{
	SetGroupName(_Name);		// �׷� �̸� ���� ����Ʈ���� Default

	Collision_Init();			// �ݸ��� �ʱ�ȭ

	if (true == GetGameObject()->IsOver())
	{
		// ���ѳ���� ���� ������Ʈ�� �ѳ��� ���� �̸� ��������~
		std::list<std::wstring> OverGameObject_SceneList = Scene::OverList(GetGameObject());
		std::list<std::wstring>::iterator StartIter = OverGameObject_SceneList.begin();
		std::list<std::wstring>::iterator EndIter = OverGameObject_SceneList.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			// �ش�� �����ͼ� '��' �� �ݸ��� �־��ֱ�!!
			My_PTR<Scene> FindScene = Scene::Scene_Find((*StartIter).c_str());
			FindScene->m_CollisionManager.PushCollision(this);
		}
	}
	else
	{
		GetScene()->m_CollisionManager.PushCollision(this);
	}


	//GetScene()->m_CollisionManager.PushCollision(_Name, this);

	return true;
}

void Collision::Collision_Init()
{
	m_ColVertexShader = ResourceManager<VertexShader>::Find(L"COLORUVVERTEXSHADER");
	m_ColPixelShader = ResourceManager<PixelShader>::Find(L"COLORUVPIXELSHADER");
	m_ColMesh = ResourceManager<Mesh>::Find(L"COLORUVMESH");
}

bool Collision::IsCollide(Collision* _OtherCol)
{
	std::unordered_set<Collision*>::iterator Find_Iter = m_ColSet.find(_OtherCol);

	if (true == Collision_Check(_OtherCol))
	{
		// �浹�ߴٸ�..
		if (Find_Iter == m_ColSet.end())
		{
			// ó������ �浿������
			m_ColSet.insert(_OtherCol);
			Collision_Enter(_OtherCol);
		}
		else
		{
			// �浹�� �ӹ��� ������ 
			Collision_Stay(_OtherCol);
		}
		return true;
	}
	else
	{
		// �浹�������� ������� 
		if (Find_Iter != m_ColSet.end())
		{
			Collision_Exit(_OtherCol);
			m_ColSet.erase(_OtherCol);
		}
	}

	return false;
}
