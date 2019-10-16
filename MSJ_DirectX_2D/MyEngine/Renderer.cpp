#include "Renderer.h"
#include "Scene.h"
#include "RenderManager.h"



Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

//

bool Renderer::Init(int _RenderGroup /*= 0*/)
{
	m_RenderGroup = _RenderGroup;

	//렌더 추가
	if (true == GetGameObject()->IsOver())
	{
		// 씬을 넘나드는 오브젝트라면
		std::list<std::wstring> OverGameObject_SceneList = Scene::OverList(GetGameObject());
		std::list<std::wstring>::iterator StartIter = OverGameObject_SceneList.begin();
		std::list<std::wstring>::iterator EndIter = OverGameObject_SceneList.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			// 해당씬 찾고 그씬에다가 렌더 추가해주기
			My_PTR<Scene> FindScene = Scene::Scene_Find((*StartIter).c_str());
			FindScene->m_RenderManager.Insert_Renderer(this);
		}
	}
	else
	{
		// 씬을 넘나들지 않고 오직 하나의 씬에서만 존재하는 오브젝트라면
		GetScene()->m_RenderManager.Insert_Renderer(this);
	}

	return true;
}
