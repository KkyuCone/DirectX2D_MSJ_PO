#pragma once
#include "EnMACRO.h"
#include "ResourceManager.h"
#include "Resource.h"
#include "NameBase.h"
#include "SceneBase.h"
#include "RenderManager.h"
#include "CollisionManager.h"

#include "STLHelper.h"

#include <map>
#include <list>

// final�� C++11���� ���� ���� ����.
// �ش� Ŭ������ ���̻� ��� ���ϰ� ���´�.

class Camera;
class GameObject;
class Scene final : public Resource, public NameBase
{
public:
	friend class Window;
	friend class GameObject;

private:
	static int SceneScriptCount;

private:
	// �ش� ���ӿ�����Ʈ�� �ѳ���� �ִ� �������� ���� �̳� Ŭ����(����ü�� ���)
	class OverGameObjectInfo : public ReferenceBase
	{
	public:
		std::list<std::wstring> m_OverSceneList;	// �ش� ���ӿ�����Ʈ�� �ѳ���� �ִ� ������ �̸� ������
		My_PTR<GameObject> m_GameObject;			// �ش� ���ӿ�����Ʈ

	public:
		bool IsSceneOverLap(const wchar_t* _Name);	// _Name���� �ش� ���ӿ�����Ʈ�� �ѳ���� �ִ� ���ΰ�?
	};

private:
	// ���� �ѳ���� �ִ� ���ӿ�����Ʈ�� �� ���ӿ�����Ʈ�� ���� ����(�ѳ���� ������..)
	static std::map<GameObject*, My_PTR<OverGameObjectInfo>> m_OverGameObjectMap;

public:
	static void OverGameObjectInfo_Create(GameObject* _Obj);
	static void PushOverInfo(GameObject* _Obj, const wchar_t* _SceneName);
private:
	static void OverGameObjectSceneChangeCheck();

private:
	//static std::list<My_PTR<OverGameObjectInfo>> m_OverGameObjectList;

private:
	//static void PushOverInfo(std::list<std::wstring>& _List, GameObject* _Obj);

private:
	static My_PTR<Scene> m_CurrentScene;		// �����
	static My_PTR<Scene> m_NextScene;			// ������

public:
	static std::list<std::wstring> OverList(GameObject* _Obj);

public:
	static My_PTR<Scene> Get_CurrentScene()
	{
		return m_CurrentScene;
	}

public:
	static My_PTR<Scene> Scene_Create(const wchar_t* _SceneName);

	template<typename T>
	static My_PTR<Scene> Scene_Create(const wchar_t* _SceneName)
	{
		My_PTR<Scene> ScenePTR = Scene_Create(_SceneName);

		if (nullptr == ScenePTR)
		{
			EAssert(true);
		}

		ScenePTR->Script_Create<T>();
		return ScenePTR;
	}

	static My_PTR<Scene> Scene_Find(const wchar_t* _SceneName);
	static bool Scene_Change(const wchar_t* _SceneName);

private:
	static void SceneChange_Check();

public:
	static void Scene_Update();
	//static void Scene_Render();		// �� ����


	//////////////////////////// Member
public:

	class SceneScript : public ReferenceBase, public SceneBase
	{
	public:
		virtual bool Init() { return true; }
		virtual bool Loading() { return true; }			//  Scene�� �ε����� �ʾ��� �� �ѹ��� ���� (�ε��Ǿ������� ����)
		virtual void Start() {}							// Scene�� ü���� �Ǿ����� �ѹ��� ����
		virtual void Update() {}						// ��� ���� ������Ʈ��(Actor��...)�� ������Ʈ�� ����Ǳ� ���� �ѹ� ����
		virtual void LateUpdate() {}					// ��� ���� ������Ʈ���� ������Ʈ�� ������ �ѹ� ����
		virtual void End() {}							// ���� �������� ������ �ٸ������� ����Ǿ����� �ѹ��� ����
		virtual void DebugRender() {}

	public:
		SceneScript() {}
		~SceneScript() {}
	};

public:
	static bool m_bDebug;			// ����� üũ (�浹 ��..������������ ������)

public:
	bool m_bLoading;
	std::list<My_PTR<SceneScript>> m_ScriptList;
	std::list<My_PTR<SceneScript>>::iterator ScriptUpdate_StartIter;
	std::list<My_PTR<SceneScript>>::iterator ScriptUpdate_EndIter;

public:
	// Loading
	// �ε��� �Ǿ����� ����
	bool IsLoading()
	{
		return m_bLoading;
	}

	// �ε� 
	void Loading();

public:
	template<typename T>
	My_PTR<T> Script_Create(void* _pArg = nullptr)
	{
		// ���ο� �� ����� 
		T* NewScript = new T();
		m_ScriptList.push_back(NewScript);
		NewScript->SetScene(this);		// � ������ �˷������ (���ξ��̱⋚����  this)
		NewScript->Init();
		++SceneScriptCount;
		return NewScript;
	}

	//template<typename T>
	//My_PTR<T> Updater_Create()			// ���� ���� �ִ� ���ӿ�����Ʈ ������Ʈ���� ���Ƶ��� ������� (��������)
	//{
	//	int a = 0;
	//	T* NewUpdater = new T();
	//	m_ScriptList.push_back(NewUpdater);
	//	SceneScript* SceneScript = NewUpdater;			// ��� ������ ������Ʈ�ؾ��ϴ��� �˷��ֱ� ���ؼ�..
	//	SceneScript->SetScene(this);
	//	return NewUpdater;
	//}

public:
	DEFINITION_CREATOR(Scene, std::wstring)
		DEFINITION_CREATOR_FUCTION()
		DEFINITION_CREATOR_FUNCTION_PARAMETER()
		DEFINITION_CREATOR_END

private:
	bool Create();
	void Start();
	void End();
	void Script_Update();
	void Script_LateUpdate();
	void Release();


private:
	std::map<int, std::list<My_PTR<GameObject>>> m_GameObjectMap;
	std::map<int, std::list<My_PTR<GameObject>>>::iterator GameObjOrder_StartIter;
	std::map<int, std::list<My_PTR<GameObject>>>::iterator GameObjOrder_EndIter;

	std::list<My_PTR<GameObject>>::iterator ObjList_StartIter;
	std::list<My_PTR<GameObject>>::iterator ObjList_EndIter;

public:
	My_PTR<GameObject> GameObject_Create(const wchar_t* _ObjName = L"GameObject", int _Order = 0);		// ������Ʈ ����
	My_PTR<GameObject> GameObject_Create(int _Order, const wchar_t* _ObjName = L"GameObject");			// ������Ʈ ����-> ������ �ְ������

	My_PTR<GameObject> GameObject_NoneTransform_Create(int _Order, const wchar_t* _ObjName = L"GameObject");			// Transform�� ���� ������Ʈ ����(�������Ʈ)
	My_PTR<GameObject> GameObject_NoneTransform_Create(const wchar_t* _ObjName = L"GameObject", int _Order = 0);		// Transform�� ���� ������Ʈ ����(�������Ʈ)


public:
	void Progree();
	void GameObject_Updater();		// ���ӿ�����Ʈ���� ������Ʈ�� ������ (�Ʒ��ִ� �Լ��� ���� ������)

	void GameObject_PrevUpdate();
	void GameObject_Update();
	void GameObject_LateUpdate();
	void GameObject_EndUpdate();
	void GameObject_FinalUpdate();

private:
	// ī�޶� ����
	Camera* m_MainCamera;
	//std::vector<Camera*>

private:
	friend class Camera;

	// ���� ī�޶� Set
	void SetCamera(Camera* _MainCamera)
	{
		// ���� nullptr�� ������ ���� ���� nullptr�� �ƴ϶��
		// ���ο� ����ī�޶� ã�� ������ ������ �Ѵ�.
		m_MainCamera = _MainCamera;
	}

public:
	// ī�޶� Get
	Camera* GetCamera() 
	{ 
		return m_MainCamera;
	}

public:
	// �����Ŵ���
	RenderManager m_RenderManager;

private:
	void Render();

public:
	CollisionManager m_CollisionManager;

private:
	void Col();


private:
	Scene();
	~Scene();
};

