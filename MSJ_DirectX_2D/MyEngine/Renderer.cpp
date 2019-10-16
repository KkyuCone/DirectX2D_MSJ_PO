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

	//���� �߰�
	if (true == GetGameObject()->IsOver())
	{
		// ���� �ѳ���� ������Ʈ���
		std::list<std::wstring> OverGameObject_SceneList = Scene::OverList(GetGameObject());
		std::list<std::wstring>::iterator StartIter = OverGameObject_SceneList.begin();
		std::list<std::wstring>::iterator EndIter = OverGameObject_SceneList.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			// �ش�� ã�� �׾����ٰ� ���� �߰����ֱ�
			My_PTR<Scene> FindScene = Scene::Scene_Find((*StartIter).c_str());
			FindScene->m_RenderManager.Insert_Renderer(this);
		}
	}
	else
	{
		// ���� �ѳ����� �ʰ� ���� �ϳ��� �������� �����ϴ� ������Ʈ���
		GetScene()->m_RenderManager.Insert_Renderer(this);
	}

	return true;
}
