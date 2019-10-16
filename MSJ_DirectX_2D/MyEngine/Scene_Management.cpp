#include "Scene.h"

My_PTR<Scene> Scene::m_CurrentScene = nullptr;
My_PTR<Scene> Scene::m_NextScene = nullptr;

My_PTR<Scene> Scene::Scene_Create(const wchar_t* _SceneName)
{
	// 받은 이름의 Scene 있는지 검사, 있다면 nullptr ( 만들지 않음 )
	if (nullptr != Scene_Find(_SceneName))
	{
		EAssert(true);
		return nullptr;
	}

	My_PTR<Scene> NewScene = ResourceManager<Scene>::Create(_SceneName);
	NewScene->RTTISetting();					// 타입에 대한 정보 넣기
	NewScene->SetName(_SceneName);
	return NewScene;
}

My_PTR<Scene> Scene::Scene_Find(const wchar_t* _SceneName)
{
	return ResourceManager<Scene>::Find(_SceneName);
}

bool Scene::Scene_Change(const wchar_t* _SceneName)
{
	m_NextScene = ResourceManager<Scene>::Find(_SceneName);

	//m_NextScene->Start();

	if (nullptr == m_NextScene)
	{
		EAssert(true);
		return false;
	}

	return true;
}

void Scene::SceneChange_Check()
{
	if (nullptr != m_NextScene)
	{
		if (nullptr != m_CurrentScene)
		{
			m_CurrentScene->End();
		}

		m_CurrentScene = m_NextScene;

		if (false == m_CurrentScene->IsLoading())
		{
			m_CurrentScene->Loading();
		}

		// 씬을 넘나드는지 체크하고 씬을 넘나드는 오브젝트라면 자식들에게도 씬을 넣어준다.
		OverGameObjectSceneChangeCheck();

		m_CurrentScene->Start();

		m_NextScene = nullptr;
	}
}

void Scene::Scene_Update()
{
	SceneChange_Check();

	if (nullptr != m_CurrentScene)
	{
		m_CurrentScene->Progree();

		// 타겟을 지워야한다.
		Device::ClearTarget();

		// 렌더를 없애고 여기서 한꺼번해 렌더를 해준다.
		m_CurrentScene->Render();
		Device::SwapChain_Present();
		m_CurrentScene->Col();				// 충돌처리

		m_CurrentScene->Release();			// 해당 오브젝트가 죽는다면 렌더를하지 않아야하기 때문에 여기서 체크하고 삭제해준다.
	}
}


//void Scene::Scene_Render()
//{
//	int a = 0;
//	// 현재 씬이 존재한다면
//	if (nullptr != m_CurrentScene)
//	{
//		// 렌더를 해줌
//		m_CurrentScene->Render();
//	}
//}