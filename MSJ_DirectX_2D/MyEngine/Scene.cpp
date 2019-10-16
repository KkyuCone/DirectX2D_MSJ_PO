#include "Scene.h"
#include "GameObject.h"
#include "STLHelper.h"
#include "EnMACRO.h"
#include "Transform.h"
#include "Renderer.h"
#include "Collision.h"

std::map<GameObject*, My_PTR<Scene::OverGameObjectInfo>> Scene::m_OverGameObjectMap;

int Scene::SceneScriptCount = 0;


// �ش� ������ ���� �ѳ���� ���ӿ�����Ʈ�� �ִ������� �ľ�
bool Scene::OverGameObjectInfo::IsSceneOverLap(const wchar_t* _Name)
{
	// �ش� ���ӿ�����Ʈ�� �ѳ���� �����߿� ���� ã�� ���� �ִ��� ã��
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

// �ѳ���� ���ӿ�����Ʈ �����߰�
// PushOverInfo�̰ͺ��� ���� ���� PushOverInfo�� �ѳ���¾����� �߰����ֱ�
void Scene::OverGameObjectInfo_Create(GameObject* _Obj)
{
	if (nullptr == _Obj)
	{
		EAssert(true);
		return;
	}

	// ������ �߰��ϱ����� �ߺ� �˻縦�Ѵ�.
	My_PTR<OverGameObjectInfo> FindInfo = Map_Find_PTR<My_PTR<OverGameObjectInfo>>(m_OverGameObjectMap, _Obj);

	if (nullptr != FindInfo)
	{
		// �׳� �ܼ���.. �ߺ��˻縦�ϰ��� �̹��ִٸ� return������
		// ��Ʈ���°ź���..
		return;
	}

	My_PTR<OverGameObjectInfo> NewOverInfo = new OverGameObjectInfo ();
	NewOverInfo->m_OverSceneList.push_back(_Obj->GetScene()->GetName());		// ���̸� �߰�
	NewOverInfo->m_GameObject = _Obj;		// ������Ʈ �߰� ( �߰�����ߵ�..�����ָ� nullptr���ڳ�!)
	m_OverGameObjectMap.insert(std::map<GameObject*, My_PTR<Scene::OverGameObjectInfo>>::value_type(_Obj, NewOverInfo));		// ������Ʈ �߰�
}

// �ѳ���� ������Ʈ�� ����鿡�� �ѳ������.. ���� �̸� ��ȯ���ֱ�
std::list<std::wstring> Scene::OverList(GameObject* _Obj)
{
	My_PTR<OverGameObjectInfo> FindInfo = Map_Find_PTR<My_PTR<OverGameObjectInfo>>(m_OverGameObjectMap, _Obj);

	// �Ű������� ���� ���ӿ�����Ʈ�� �ѳ���� �����̾��ٸ�
	if (nullptr == FindInfo)
	{
		EAssert(true);
		return std::list<std::wstring>();
	}

	return FindInfo->m_OverSceneList;		// ����(���̸����� ����ִ� ����Ʈ) ��ȯ
}

// �ѳ���� ���ӿ�����Ʈ�� �ѳ��� ���� �߰����ֱ�
void Scene::PushOverInfo(GameObject* _Obj, const wchar_t* _SceneName)
{
	My_PTR<OverGameObjectInfo> ObjInfo = Map_Find_PTR<My_PTR<OverGameObjectInfo>>(m_OverGameObjectMap, _Obj);

	// ������Ʈ�� null�̰ų� �������� ���ٸ� ��Ʈ��
	// �ߺ����� ���� �ʱ����� true��!! 
	// �ش� ���ӿ�����Ʈ�� ���� ����� �ѳ���� ���� �̹� �����Ѵٸ� ���̻� ���� ���ƾ���
	if (nullptr == ObjInfo || true == ObjInfo->IsSceneOverLap(_SceneName))
	{
		EAssert(true);
		return;
	}

	// �ѳ��� �� �߰��ϱ�
	ObjInfo->m_OverSceneList.push_back(_SceneName);

	My_PTR<Scene> FindScene = ResourceManager<Scene>::Find(_SceneName);		// �� ã��-

	// �ش���� ����������� ���ٸ� ��Ʈ����
	// ���� �������̾��µ�..��ĳ �������ֳ�;;����;;;
	if (nullptr == FindScene)
	{
		EAssert(true);
		return;
	}

	// map�̱� ������ int�ʿ� �ߺ����� ������ ����
	std::map<int, std::list<My_PTR<GameObject>>>::iterator FindIter = FindScene->m_GameObjectMap.find(_Obj->GetOrder());

	// ���� map-> m_GameObjectMap�� �����ʴٸ� ���� �߰�
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
		// �ѳ���ֵ�..(�׷��پֵ� �߰� ) ������ �߰����ֱ�
		FindScene->m_RenderManager.Insert_Renderer((*Start));
	}

	// �������� �߰������� ���� �ݸ����鵵 �߰�����
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
		// ������̾��ٸ�..����..
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
				// ���ӿ�����Ʈ�� nullptr�̶��.. ��Ʈ����
				EAssert(true);
				continue;
			}

			Start->second->m_GameObject->SetScene(m_CurrentScene);
		}
	}
}

// �� �ѳ���¾ֵ� ���� �߰����ֱ�
//std::list<My_PTR<Scene::OverGameObjectInfo>> Scene::m_OverGameObjectList;

//void Scene::PushOverInfo(std::list<std::wstring>& _List, GameObject* _Obj)
//{
//	My_PTR<Scene::OverGameObjectInfo> NewGameObjInfo = new Scene::OverGameObjectInfo();
//
//	NewGameObjInfo->m_OverSceneList = _List;
//	m_OverGameObjectList.push_back(NewGameObjInfo);
//}

//

bool Scene::m_bDebug = true;			// �ϴ� ����� ���� �ʱ�ȭ..

Scene::Scene() : m_bLoading(false)
{
}

Scene::~Scene()
{
}

/////////////////////

// ���� �ε��Ѵ�.
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

	m_bLoading = true;		// �ε��� �����ϱ�  true
}

bool Scene::Create()
{
	m_GameObjectMap.insert(std::map<int, std::list<My_PTR<GameObject>>>::value_type(0, std::list<My_PTR<GameObject>>()));
	return true;
}


void Scene::Progree()
{
	// ������Ʈ ����
	Script_Update();
	//GameObject_Update();			// ���͵�..�� ���ӿ�����Ʈ�� ������Ʈ ������
	GameObject_Updater();						// �갡 �����ִ°��� �������ƴ�;;
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


// ���� (Release)
void Scene::Release()
{
	// �����Ŵ��� ������! 
	// ������ ������ ȭ�鿡�� ������°Ŵϱ�..���� ȭ�鿡�� ����� ������Ʈ���� ������ �����ؾ���.
	m_RenderManager.Release();
	m_CollisionManager.Release();				// �ݶ��̴� �����(���ʿ��Ѱ� ���������)

	// �׸��� ���ӿ�����Ʈ�� ����~~
	GameObjOrder_StartIter = m_GameObjectMap.begin();
	GameObjOrder_EndIter = m_GameObjectMap.end();

	for (; GameObjOrder_StartIter != GameObjOrder_EndIter; ++GameObjOrder_StartIter)
	{
		ObjList_StartIter = GameObjOrder_StartIter->second.begin();
		ObjList_EndIter = GameObjOrder_StartIter->second.end();

		for ( ; ObjList_StartIter != ObjList_EndIter;)
		{
			if (true == (*ObjList_StartIter)->IsDeath()				// �׾��ٸ�..
				|| nullptr != (*ObjList_StartIter)->m_pParent)		// �θ� �����Ѵٸ�
			{
				int a = 0;
				const wchar_t* GenName = (*ObjList_StartIter)->GetName();
				// ���� �ش� ���ӿ�����Ʈ�� �״´ٸ�
				// ���ӿ�����Ʈ ���� (��������)���� �����ش�. (���̻� ���� ���ʿ䰡����)

				// �ױ� ������ ȣ��
				ObjList_StartIter = GameObjOrder_StartIter->second.erase(ObjList_StartIter);
			}
			else
			{
				int a = 0;
				const wchar_t* GenName = (*ObjList_StartIter)->GetName();
				//(*ObjList_StartIter)->ChildWaitCheck();
				// �ڱ� �ڽĵ��� �׾����� ����ִ��� üũ �˻� �ؾ��� -> ������ ����
				// ���ӿ�����Ʈ�� ���� �ʾҴٸ� 
				++ObjList_StartIter;					// �����ʾ����Ƿ� �������� ������ ������ ��������..
			}
		}

	}
}


// ���� ������Ʈ ���� ( ������ �ʿ��� ��� ) 
// Transform�� �ִ� ���� ������Ʈ
My_PTR<GameObject> Scene::GameObject_Create(int _Order, const wchar_t* _ObjName)
{
	My_PTR<GameObject> NewGameObject = GameObject_NoneTransform_Create(_ObjName, _Order);
	NewGameObject->Component_Add<Transform>();
	return NewGameObject;
}


// ���ӿ�����Ʈ ���� ( ���ӿ�����Ʈ �̸�, ���� )
// Transform�� �ִ� ���� ������Ʈ
My_PTR<GameObject> Scene::GameObject_Create(const wchar_t* _ObjName, int _Order)
{
	My_PTR<GameObject> NewGameObject = GameObject_NoneTransform_Create(_ObjName, _Order);
	NewGameObject->Component_Add<Transform>();
	return NewGameObject;
}


// Transform�� ���� ����ӿ�����Ʈ (����) 
My_PTR<GameObject> Scene::GameObject_NoneTransform_Create(int _Order, const wchar_t* _ObjName)
{
	return GameObject_NoneTransform_Create(_ObjName, _Order);
}


// Transform�� ���� ����ӿ�����Ʈ (�̸�, ���� �Ѵ�)
My_PTR<GameObject> Scene::GameObject_NoneTransform_Create(const wchar_t* _ObjName, int _Order)
{
	// �ش� ������Ʈ�� �̹� �����ϴ��� ���� �ľ�
	std::map<int, std::list<My_PTR<GameObject>>>::iterator Find_Iter = m_GameObjectMap.find(_Order);

	if (Find_Iter == m_GameObjectMap.end())
	{
		// �ش� ���� ������Ʈ�� ���ٸ� list �߰� ( ����, ���ӿ�����Ʈ ����Ʈ )
		m_GameObjectMap.insert(std::map<int, std::list<My_PTR<GameObject>>>::value_type(_Order, std::list<My_PTR<GameObject>>()));
		Find_Iter = m_GameObjectMap.find(_Order);
	}

	// ���� ������Ʈ ���� 
	GameObject* NewObj = new GameObject();
	NewObj->SetOrdrer(_Order);
	NewObj->SetName(_ObjName);
	NewObj->SetScene(this);					// �ش� ���� ������Ʈ�� �ڽ��� ��� ���� �����ƴ��� �� �� �־�� �Ѵ�.
	Find_Iter->second.push_back(NewObj);		// �ش� ���ӿ�����Ʈ ����Ʈ�� ���ӿ�����Ʈ �߰� 

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
	// �ڽ� ó�� ������

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

	// �ٽ� �����·�~
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
		m_CollisionManager.DebugRender();		// ������ʿ��� �����������( �浹ü ��纸�̰�..)


		// ����� ����
		ScriptUpdate_StartIter = m_ScriptList.begin();
		ScriptUpdate_EndIter = m_ScriptList.end();

		for (; ScriptUpdate_StartIter != ScriptUpdate_EndIter; ++ScriptUpdate_StartIter)
		{
			(*ScriptUpdate_StartIter)->DebugRender();
		}
		
	}
}


// �浹
void Scene::Col()
{
	m_CollisionManager.Col();
}

