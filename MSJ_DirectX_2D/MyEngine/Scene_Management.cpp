#include "Scene.h"

My_PTR<Scene> Scene::m_CurrentScene = nullptr;
My_PTR<Scene> Scene::m_NextScene = nullptr;

My_PTR<Scene> Scene::Scene_Create(const wchar_t* _SceneName)
{
	// ���� �̸��� Scene �ִ��� �˻�, �ִٸ� nullptr ( ������ ���� )
	if (nullptr != Scene_Find(_SceneName))
	{
		EAssert(true);
		return nullptr;
	}

	My_PTR<Scene> NewScene = ResourceManager<Scene>::Create(_SceneName);
	NewScene->RTTISetting();					// Ÿ�Կ� ���� ���� �ֱ�
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

		// ���� �ѳ������ üũ�ϰ� ���� �ѳ���� ������Ʈ��� �ڽĵ鿡�Ե� ���� �־��ش�.
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

		// Ÿ���� �������Ѵ�.
		Device::ClearTarget();

		// ������ ���ְ� ���⼭ �Ѳ����� ������ ���ش�.
		m_CurrentScene->Render();
		Device::SwapChain_Present();
		m_CurrentScene->Col();				// �浹ó��

		m_CurrentScene->Release();			// �ش� ������Ʈ�� �״´ٸ� ���������� �ʾƾ��ϱ� ������ ���⼭ üũ�ϰ� �������ش�.
	}
}


//void Scene::Scene_Render()
//{
//	int a = 0;
//	// ���� ���� �����Ѵٸ�
//	if (nullptr != m_CurrentScene)
//	{
//		// ������ ����
//		m_CurrentScene->Render();
//	}
//}