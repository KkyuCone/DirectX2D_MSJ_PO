#include "Scene.h"
#include "GameObject.h"
#include "STLHelper.h"
#include "EnMACRO.h"
#include "Transform.h"
#include "Renderer.h"
#include "Collision.h"

std::map<GameObject*, My_PTR<Scene::OverGameObjectInfo>> Scene::m_OverGameObjectMap;

int Scene::SceneScriptCount = 0;


// 해당 씬에서 씬을 넘나드는 게임오브젝트가 있는지여부 파악
bool Scene::OverGameObjectInfo::IsSceneOverLap(const wchar_t* _Name)
{
	// 해당 게임오브젝트가 넘나드는 씬들중에 내가 찾는 씬이 있는지 찾기
	std::list<std::wstring>::iterator Start = m_OverSceneList.begin();
	std::list<std::wstring>::iterator End = m_OverSceneList.end();

	for (; Start != End; ++Start)
	{
		if ((*Start) == _Name)
		{
			return true;
		}
	}

	return false;
}

// 넘나드는 게임오브젝트 정보추가
// PushOverInfo이것보다 먼저 그후 PushOverInfo로 넘나드는씬들을 추가해주기
void Scene::OverGameObjectInfo_Create(GameObject* _Obj)
{
	if (nullptr == _Obj)
	{
		EAssert(true);
		return;
	}

	// 정보를 추가하기전에 중복 검사를한다.
	My_PTR<OverGameObjectInfo> FindInfo = Map_Find_PTR<My_PTR<OverGameObjectInfo>>(m_OverGameObjectMap, _Obj);

	if (nullptr != FindInfo)
	{
		// 그냥 단순히.. 중복검사를하고나서 이미있다면 return을하자
		// 터트리는거보단..
		return;
	}

	My_PTR<OverGameObjectInfo> NewOverInfo = new OverGameObjectInfo ();
	NewOverInfo->m_OverSceneList.push_back(_Obj->GetScene()->GetName());		// 씬이름 추가
	NewOverInfo->m_GameObject = _Obj;		// 오브젝트 추가 ( 추가해줘야됨..안해주면 nullptr이자낭!)
	m_OverGameObjectMap.insert(std::map<GameObject*, My_PTR<Scene::OverGameObjectInfo>>::value_type(_Obj, NewOverInfo));		// 오브젝트 추가
}

// 넘나드는 오브젝트가 어떤씬들에서 넘나드는지.. 씬들 이름 반환해주기
std::list<std::wstring> Scene::OverList(GameObject* _Obj)
{
	My_PTR<OverGameObjectInfo> FindInfo = Map_Find_PTR<My_PTR<OverGameObjectInfo>>(m_OverGameObjectMap, _Obj);

	// 매개변수로 받은 게임오브젝트가 넘나드는 씬들이없다면
	if (nullptr == FindInfo)
	{
		EAssert(true);
		return std::list<std::wstring>();
	}

	return FindInfo->m_OverSceneList;		// 씬들(씬이름들이 들어있는 리스트) 반환
}

// 넘나드는 게임오브젝트에 넘나들 씬들 추가해주기
void Scene::PushOverInfo(GameObject* _Obj, const wchar_t* _SceneName)
{
	My_PTR<OverGameObjectInfo> ObjInfo = Map_Find_PTR<My_PTR<OverGameObjectInfo>>(m_OverGameObjectMap, _Obj);

	// 오브젝트가 null이거나 씬정보가 없다면 터트림
	// 중복씬을 넣지 않기위해 true임!! 
	// 해당 게임오브젝트에 내가 등록할 넘나드는 씬이 이미 존재한다면 더이상 넣지 말아야함
	if (nullptr == ObjInfo || true == ObjInfo->IsSceneOverLap(_SceneName))
	{
		EAssert(true);
		return;
	}

	// 넘나들 씬 추가하기
	ObjInfo->m_OverSceneList.push_back(_SceneName);

	My_PTR<Scene> FindScene = ResourceManager<Scene>::Find(_SceneName);		// 씬 찾기-

	// 해당씬이 만들어진적이 없다면 터트리기
	// 씬을 만든적이없는데..어캐 넣을수있냐;;ㄷㄷ;;;
	if (nullptr == FindScene)
	{
		EAssert(true);
		return;
	}

	// map이기 때문에 int쪽에 중복으로 넣을수 없음
	std::map<int, std::list<My_PTR<GameObject>>>::iterator FindIter = FindScene->m_GameObjectMap.find(_Obj->GetOrder());

	// 만약 map-> m_GameObjectMap에 있지않다면 새로 추가
	if (FindIter == FindScene->m_GameObjectMap.end())
	{
		FindScene->m_GameObjectMap.insert(std::map<int, std::list<My_PTR<GameObject>>>::value_type(_Obj->GetOrder(), std::list<My_PTR<GameObject>>()));
		FindIter = FindScene->m_GameObjectMap.find(_Obj->GetOrder());
	}

	FindIter->second.push_back(_Obj);

	std::list<My_PTR<Renderer>> RenderList = _Obj->GetChildAllComponent<Renderer>();

	std::list<My_PTR<Renderer>>::iterator Start = RenderList.begin();
	std::list<My_PTR<Renderer>>::iterator End = RenderList.end();

	for (; Start != End; ++Start)
	{
		// 넘나들애들..(그려줄애들 추가 ) 렌더들 추가해주기
		FindScene->m_RenderManager.Insert_Renderer((*Start));
	}

	// 렌더들을 추가헀으니 이제 콜리전들도 추가하자
	std::list<My_PTR<Collision>> ColList = _Obj->GetChildAllComponent<Collision>();

	std::list<My_PTR<Collision>>::iterator ColList_StartIter = ColList.begin();
	std::list<My_PTR<Collision>>::iterator ColList_EndIter = ColList.end();

	for (; ColList_StartIter != ColList_EndIter; ++ColList_StartIter)
	{
		FindScene->m_CollisionManager.PushCollision((*ColList_StartIter));
	}

}

void Scene::OverGameObjectSceneChangeCheck()
{
	if (nullptr == m_CurrentScene)
	{
		// 현재씬이없다면..ㅃㅃ..
		return;
	}

	std::map<GameObject*, My_PTR<OverGameObjectInfo>>::iterator Start = m_OverGameObjectMap.begin();
	std::map<GameObject*, My_PTR<OverGameObjectInfo>>::iterator End = m_OverGameObjectMap.end();

	for (; Start != End; ++Start)
	{
		if (true == Start->second->IsSceneOverLap(m_CurrentScene->GetName()))
		{
			if (nullptr == Start->second->m_GameObject)
			{
				// 게임오브젝트가 nullptr이라면.. 터트리장
				EAssert(true);
				continue;
			}

			Start->second->m_GameObject->SetScene(m_CurrentScene);
		}
	}
}

// 씬 넘나드는애들 정보 추가해주기
//std::list<My_PTR<Scene::OverGameObjectInfo>> Scene::m_OverGameObjectList;

//void Scene::PushOverInfo(std::list<std::wstring>& _List, GameObject* _Obj)
//{
//	My_PTR<Scene::OverGameObjectInfo> NewGameObjInfo = new Scene::OverGameObjectInfo();
//
//	NewGameObjInfo->m_OverSceneList = _List;
//	m_OverGameObjectList.push_back(NewGameObjInfo);
//}

//

bool Scene::m_bDebug = true;			// 일단 디버그 모드로 초기화..

Scene::Scene() : m_bLoading(false)
{
}

Scene::~Scene()
{
}

/////////////////////

// 씬을 로딩한다.
void Scene::Loading()
{
	if (0 < m_ScriptList.size())
	{
		ScriptUpdate_StartIter = m_ScriptList.begin();
		ScriptUpdate_EndIter = m_ScriptList.end();

		for (; ScriptUpdate_StartIter != ScriptUpdate_EndIter; ++ScriptUpdate_StartIter)
		{
			(*ScriptUpdate_StartIter)->Loading();
		}
	}

	m_bLoading = true;		// 로딩을 했으니까  true
}

bool Scene::Create()
{
	m_GameObjectMap.insert(std::map<int, std::list<My_PTR<GameObject>>>::value_type(0, std::list<My_PTR<GameObject>>()));
	return true;
}


void Scene::Progree()
{
	// 업데이트 순서
	Script_Update();
	//GameObject_Update();			// 액터들..즉 게임오브젝트들 업데이트 돌리기
	GameObject_Updater();						// 얘가 돌려주는거임 위에꺼아님;;
	Script_LateUpdate();
}


void Scene::Start()
{
	ScriptUpdate_StartIter = m_ScriptList.begin();
	ScriptUpdate_EndIter = m_ScriptList.end();

	for (; ScriptUpdate_StartIter != ScriptUpdate_EndIter; ++ScriptUpdate_StartIter)
	{
		(*ScriptUpdate_StartIter)->Start();
	}
}

void Scene::End()
{
	ScriptUpdate_StartIter = m_ScriptList.begin();
	ScriptUpdate_EndIter = m_ScriptList.end();

	for (; ScriptUpdate_StartIter != ScriptUpdate_EndIter; ++ScriptUpdate_StartIter)
	{
		(*ScriptUpdate_StartIter)->End();
	}
}

void Scene::Script_Update()
{
	ScriptUpdate_StartIter = m_ScriptList.begin();
	ScriptUpdate_EndIter = m_ScriptList.end();

	for (; ScriptUpdate_StartIter != ScriptUpdate_EndIter; ++ScriptUpdate_StartIter)
	{
		(*ScriptUpdate_StartIter)->Update();
	}
}

void Scene::Script_LateUpdate()
{
	ScriptUpdate_StartIter = m_ScriptList.begin();
	ScriptUpdate_EndIter = m_ScriptList.end();

	for (; ScriptUpdate_StartIter != ScriptUpdate_EndIter; ++ScriptUpdate_StartIter)
	{
		(*ScriptUpdate_StartIter)->LateUpdate();
	}
}


// 삭제 (Release)
void Scene::Release()
{
	// 랜더매니저 릴리즈! 
	// 렌더를 지워야 화면에서 사라지는거니까..먼저 화면에서 지우고 오브젝트들을 완전히 삭제해야함.
	m_RenderManager.Release();
	m_CollisionManager.Release();				// 콜라이더 지우기(불필요한거 지워줘야함)

	// 그리고 게임오브젝트들 삭제~~
	GameObjOrder_StartIter = m_GameObjectMap.begin();
	GameObjOrder_EndIter = m_GameObjectMap.end();

	for (; GameObjOrder_StartIter != GameObjOrder_EndIter; ++GameObjOrder_StartIter)
	{
		ObjList_StartIter = GameObjOrder_StartIter->second.begin();
		ObjList_EndIter = GameObjOrder_StartIter->second.end();

		for ( ; ObjList_StartIter != ObjList_EndIter;)
		{
			if (true == (*ObjList_StartIter)->IsDeath()				// 죽었다면..
				|| nullptr != (*ObjList_StartIter)->m_pParent)		// 부모가 존재한다면
			{
				int a = 0;
				const wchar_t* GenName = (*ObjList_StartIter)->GetName();
				// 만약 해당 게임오브젝트가 죽는다면
				// 게임오브젝트 순서 (렌더순서)에서 지워준다. (더이상 렌더 할필요가없음)

				// 죽기 직전에 호출
				ObjList_StartIter = GameObjOrder_StartIter->second.erase(ObjList_StartIter);
			}
			else
			{
				int a = 0;
				const wchar_t* GenName = (*ObjList_StartIter)->GetName();
				//(*ObjList_StartIter)->ChildWaitCheck();
				// 자기 자식들이 죽었는지 살아있는지 체크 검사 해야함 -> 위에서 해줌
				// 게임오브젝트가 죽지 않았다면 
				++ObjList_StartIter;					// 죽지않았으므로 삭제해줄 이유가 없으로 다음으로..
			}
		}

	}
}


// 게임 오브젝트 생성 ( 순서만 필요한 경우 ) 
// Transform이 있는 게임 오브젝트
My_PTR<GameObject> Scene::GameObject_Create(int _Order, const wchar_t* _ObjName)
{
	My_PTR<GameObject> NewGameObject = GameObject_NoneTransform_Create(_ObjName, _Order);
	NewGameObject->Component_Add<Transform>();
	return NewGameObject;
}


// 게임오브젝트 생성 ( 게임오브젝트 이름, 순서 )
// Transform이 있는 게임 오브젝트
My_PTR<GameObject> Scene::GameObject_Create(const wchar_t* _ObjName, int _Order)
{
	My_PTR<GameObject> NewGameObject = GameObject_NoneTransform_Create(_ObjName, _Order);
	NewGameObject->Component_Add<Transform>();
	return NewGameObject;
}


// Transform이 없는 빈게임오브젝트 (순서) 
My_PTR<GameObject> Scene::GameObject_NoneTransform_Create(int _Order, const wchar_t* _ObjName)
{
	return GameObject_NoneTransform_Create(_ObjName, _Order);
}


// Transform이 없는 빈게임오브젝트 (이름, 순서 둘다)
My_PTR<GameObject> Scene::GameObject_NoneTransform_Create(const wchar_t* _ObjName, int _Order)
{
	// 해당 오브젝트가 이미 존재하는지 여부 파악
	std::map<int, std::list<My_PTR<GameObject>>>::iterator Find_Iter = m_GameObjectMap.find(_Order);

	if (Find_Iter == m_GameObjectMap.end())
	{
		// 해당 게임 오브젝트가 없다면 list 추가 ( 순서, 게임오브젝트 리스트 )
		m_GameObjectMap.insert(std::map<int, std::list<My_PTR<GameObject>>>::value_type(_Order, std::list<My_PTR<GameObject>>()));
		Find_Iter = m_GameObjectMap.find(_Order);
	}

	// 게임 오브젝트 생성 
	GameObject* NewObj = new GameObject();
	NewObj->SetOrdrer(_Order);
	NewObj->SetName(_ObjName);
	NewObj->SetScene(this);					// 해당 게임 오브젝트가 자신이 어디 씬에 생성됐는지 알 수 있어야 한다.
	Find_Iter->second.push_back(NewObj);		// 해당 게임오브젝트 리스트에 게임오브젝트 추가 

	return NewObj;
}

void Scene::GameObject_Updater()
{
	GameObject_PrevUpdate();
	GameObject_Update();
	GameObject_LateUpdate();
	GameObject_EndUpdate();
	GameObject_FinalUpdate();
}

void Scene::GameObject_PrevUpdate()
{
	// 자식 처리 ㅇㅅㅇ

	GameObjOrder_StartIter = m_GameObjectMap.begin();
	GameObjOrder_EndIter = m_GameObjectMap.end();

	for (; GameObjOrder_StartIter != GameObjOrder_EndIter; ++GameObjOrder_StartIter)
	{
		ObjList_StartIter = GameObjOrder_StartIter->second.begin();
		ObjList_EndIter = GameObjOrder_StartIter->second.end();

		for (; ObjList_StartIter != ObjList_EndIter; ++ObjList_StartIter)
		{
			(*ObjList_StartIter)->ChildWaitCheck();
		}
	}

	// 다시 원상태로~
	GameObjOrder_StartIter = m_GameObjectMap.begin();
	GameObjOrder_EndIter = m_GameObjectMap.end();

	for (; GameObjOrder_StartIter != GameObjOrder_EndIter; ++GameObjOrder_StartIter)
	{
		ObjList_StartIter = GameObjOrder_StartIter->second.begin();
		ObjList_EndIter = GameObjOrder_StartIter->second.end();

		for (; ObjList_StartIter != ObjList_EndIter; ++ObjList_StartIter)
		{
			if ((*ObjList_StartIter)->IsUpdate())
			{
				(*ObjList_StartIter)->PrevUpdate();
			}
		}
	}
}

void Scene::GameObject_Update()
{
	GameObjOrder_StartIter = m_GameObjectMap.begin();
	GameObjOrder_EndIter = m_GameObjectMap.end();

	for (; GameObjOrder_StartIter != GameObjOrder_EndIter; ++GameObjOrder_StartIter)
	{
		ObjList_StartIter = GameObjOrder_StartIter->second.begin();
		ObjList_EndIter = GameObjOrder_StartIter->second.end();

		for (; ObjList_StartIter != ObjList_EndIter; ++ObjList_StartIter)
		{
			if ((*ObjList_StartIter)->IsUpdate())
			{
				(*ObjList_StartIter)->Update();
			}
		}
	}
}

void Scene::GameObject_LateUpdate()
{
	GameObjOrder_StartIter = m_GameObjectMap.begin();
	GameObjOrder_EndIter = m_GameObjectMap.end();

	for (; GameObjOrder_StartIter != GameObjOrder_EndIter; ++GameObjOrder_StartIter)
	{
		ObjList_StartIter = GameObjOrder_StartIter->second.begin();
		ObjList_EndIter = GameObjOrder_StartIter->second.end();

		for (; ObjList_StartIter != ObjList_EndIter; ++ObjList_StartIter)
		{
			if ((*ObjList_StartIter)->IsUpdate())
			{
				(*ObjList_StartIter)->LateUpdate();
			}
		}
	}
}

void Scene::GameObject_EndUpdate()
{
	GameObjOrder_StartIter = m_GameObjectMap.begin();
	GameObjOrder_EndIter = m_GameObjectMap.end();

	for (; GameObjOrder_StartIter != GameObjOrder_EndIter; ++GameObjOrder_StartIter)
	{
		ObjList_StartIter = GameObjOrder_StartIter->second.begin();
		ObjList_EndIter = GameObjOrder_StartIter->second.end();

		for (; ObjList_StartIter != ObjList_EndIter; ++ObjList_StartIter)
		{
			if ((*ObjList_StartIter)->IsUpdate())
			{
				(*ObjList_StartIter)->EndUpdate();
			}
		}
	}
}

void Scene::GameObject_FinalUpdate()
{
	GameObjOrder_StartIter = m_GameObjectMap.begin();
	GameObjOrder_EndIter = m_GameObjectMap.end();

	for (; GameObjOrder_StartIter != GameObjOrder_EndIter; ++GameObjOrder_StartIter)
	{
		ObjList_StartIter = GameObjOrder_StartIter->second.begin();
		ObjList_EndIter = GameObjOrder_StartIter->second.end();

		for (; ObjList_StartIter != ObjList_EndIter; ++ObjList_StartIter)
		{
			if ((*ObjList_StartIter)->IsUpdate())
			{
				(*ObjList_StartIter)->FinalUpdate();
			}
		}
	}
}



void Scene::Render()
{
	int a = 0;
	m_RenderManager.Render();

	if (true == m_bDebug)
	{
		m_CollisionManager.DebugRender();		// 디버그쪽에도 렌더해줘야함( 충돌체 모양보이게..)


		// 디버그 렌더
		ScriptUpdate_StartIter = m_ScriptList.begin();
		ScriptUpdate_EndIter = m_ScriptList.end();

		for (; ScriptUpdate_StartIter != ScriptUpdate_EndIter; ++ScriptUpdate_StartIter)
		{
			(*ScriptUpdate_StartIter)->DebugRender();
		}
		
	}
}


// 충돌
void Scene::Col()
{
	m_CollisionManager.Col();
}

