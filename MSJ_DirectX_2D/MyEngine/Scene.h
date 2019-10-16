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

// final은 C++11에서 새로 생긴 문법.
// 해당 클래스를 더이상 상속 못하게 막는다.

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
	// 해당 게임오브젝트가 넘나들고 있는 씬정보에 대한 이너 클래스(구조체로 사용)
	class OverGameObjectInfo : public ReferenceBase
	{
	public:
		std::list<std::wstring> m_OverSceneList;	// 해당 게임오브젝트가 넘나들고 있는 씬들의 이름 정보들
		My_PTR<GameObject> m_GameObject;			// 해당 게임오브젝트

	public:
		bool IsSceneOverLap(const wchar_t* _Name);	// _Name씬이 해당 게임오브젝트가 넘나들고 있는 씬인가?
	};

private:
	// 씬을 넘나들고 있는 게임오브젝트와 그 게임오브젝트에 대한 정보(넘나드는 씬정보..)
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
	static My_PTR<Scene> m_CurrentScene;		// 현재씬
	static My_PTR<Scene> m_NextScene;			// 다음씬

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
	//static void Scene_Render();		// 씬 렌더


	//////////////////////////// Member
public:

	class SceneScript : public ReferenceBase, public SceneBase
	{
	public:
		virtual bool Init() { return true; }
		virtual bool Loading() { return true; }			//  Scene이 로딩되지 않았을 때 한번만 실행 (로딩되어있으면 안함)
		virtual void Start() {}							// Scene이 체인지 되었을때 한번만 실행
		virtual void Update() {}						// 모든 게임 오브젝트들(Actor들...)의 업데이트가 실행되기 전에 한번 실행
		virtual void LateUpdate() {}					// 모든 게임 오브젝트들의 업데이트가 끝나고 한번 실행
		virtual void End() {}							// 현재 실행중인 씬에서 다른씬으로 변경되었을때 한번만 실행
		virtual void DebugRender() {}

	public:
		SceneScript() {}
		~SceneScript() {}
	};

public:
	static bool m_bDebug;			// 디버그 체크 (충돌 선..구분지을려고 ㅇㅅㅇ)

public:
	bool m_bLoading;
	std::list<My_PTR<SceneScript>> m_ScriptList;
	std::list<My_PTR<SceneScript>>::iterator ScriptUpdate_StartIter;
	std::list<My_PTR<SceneScript>>::iterator ScriptUpdate_EndIter;

public:
	// Loading
	// 로딩이 되었는지 여부
	bool IsLoading()
	{
		return m_bLoading;
	}

	// 로딩 
	void Loading();

public:
	template<typename T>
	My_PTR<T> Script_Create(void* _pArg = nullptr)
	{
		// 새로운 씬 만들기 
		T* NewScript = new T();
		m_ScriptList.push_back(NewScript);
		NewScript->SetScene(this);		// 어떤 씬인지 알려줘야함 (본인씬이기떄문에  this)
		NewScript->Init();
		++SceneScriptCount;
		return NewScript;
	}

	//template<typename T>
	//My_PTR<T> Updater_Create()			// 만들 씬에 있는 게임오브젝트 업데이트들이 돌아도록 만들어줌 (업데이터)
	//{
	//	int a = 0;
	//	T* NewUpdater = new T();
	//	m_ScriptList.push_back(NewUpdater);
	//	SceneScript* SceneScript = NewUpdater;			// 어디 씬에서 업데이트해야하는지 알려주기 위해서..
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
	My_PTR<GameObject> GameObject_Create(const wchar_t* _ObjName = L"GameObject", int _Order = 0);		// 오브젝트 생성
	My_PTR<GameObject> GameObject_Create(int _Order, const wchar_t* _ObjName = L"GameObject");			// 오브젝트 생성-> 순서만 넣고싶을때

	My_PTR<GameObject> GameObject_NoneTransform_Create(int _Order, const wchar_t* _ObjName = L"GameObject");			// Transform이 없는 오브젝트 생성(빈오브젝트)
	My_PTR<GameObject> GameObject_NoneTransform_Create(const wchar_t* _ObjName = L"GameObject", int _Order = 0);		// Transform이 없는 오브젝트 생성(빈오브젝트)


public:
	void Progree();
	void GameObject_Updater();		// 게임오브젝트들의 업데이트를 돌려줌 (아래있는 함수들 돌림 ㅇㅅㅇ)

	void GameObject_PrevUpdate();
	void GameObject_Update();
	void GameObject_LateUpdate();
	void GameObject_EndUpdate();
	void GameObject_FinalUpdate();

private:
	// 카메라 관련
	Camera* m_MainCamera;
	//std::vector<Camera*>

private:
	friend class Camera;

	// 메인 카메라 Set
	void SetCamera(Camera* _MainCamera)
	{
		// 만약 nullptr이 들어오고 내가 현재 nullptr이 아니라면
		// 새로운 메인카메라를 찾는 로직을 만들어야 한다.
		m_MainCamera = _MainCamera;
	}

public:
	// 카메라 Get
	Camera* GetCamera() 
	{ 
		return m_MainCamera;
	}

public:
	// 렌더매니저
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

