#include "Collision.h"
#include "GameMath.h"
#include "Scene.h"

// 충돌 그림그리기
#include "ResourceManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Mesh.h"

// 충돌체관련 렌더
My_PTR<VertexShader> Collision::m_ColVertexShader;			// 버텍스쉐이더
My_PTR<PixelShader> Collision::m_ColPixelShader;			// 픽셀쉐이더
My_PTR<Mesh> Collision::m_ColMesh;							// 메쉬

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

// 충돌체 삭제(자기가 죽었을때..정확히는 업데이트가 false일때 제거-> 업데이트를 안하는 상태라면 충돌체가 있을 필요가 없으므로 삭제)
// 살아있는애꺼.. 콜리전리스트 돌려서..
// 죽은애들 없애줌..
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
	SetGroupName(_Name);		// 그룹 이름 설정 디폴트값은 Default

	Collision_Init();			// 콜리전 초기화

	if (true == GetGameObject()->IsOver())
	{
		// 씬넘나드는 현재 오브젝트의 넘나들 씬들 이름 가져오깅~
		std::list<std::wstring> OverGameObject_SceneList = Scene::OverList(GetGameObject());
		std::list<std::wstring>::iterator StartIter = OverGameObject_SceneList.begin();
		std::list<std::wstring>::iterator EndIter = OverGameObject_SceneList.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			// 해당씬 가져와서 'ㅅ' 내 콜리전 넣어주기!!
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
		// 충돌했다면..
		if (Find_Iter == m_ColSet.end())
		{
			// 처음으로 충동했을때
			m_ColSet.insert(_OtherCol);
			Collision_Enter(_OtherCol);
		}
		else
		{
			// 충돌이 머물러 있을때 
			Collision_Stay(_OtherCol);
		}
		return true;
	}
	else
	{
		// 충돌범위에서 벗어났을때 
		if (Find_Iter != m_ColSet.end())
		{
			Collision_Exit(_OtherCol);
			m_ColSet.erase(_OtherCol);
		}
	}

	return false;
}
