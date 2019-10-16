#include "RenderManager.h"
#include "Window.h"


///////////////////////// ī�޶��� ������ �ִ�.

// ī�޶� �ϳ��� ��������
// Cam�� �����ϴ� ��Ƽ���� Ű��
// ķ�� ������ �ǹ��ϰ� �ǰ�
// ��Ƽ���̹Ƿ� ������ ���� �༮���� ������ ������.
// �׶��� ������ �Ű澲�� �ʱ�� �Ѵ�.

// ���������� ������ �ִ� ���̴�.
// ���������� ������ �� �����׷��� �ǹ��ϸ�
// ������ ������ �ǹ��ϰ� �ȴ�.
// ���߿� ���ĸ� ����ϰ� �Ǹ�
// �� z�� ������ Ȥ�� z���۸� ���� �Ҽ��� �ִ�.
// �װ� �����е��� �˾Ƽ� �Ͻÿ�.
// std::map<int, std::list<HPTR<HRenderer>>> �� stl��
// std::list<HPTR<HRenderer>>�� z���� ���� �����ؾ��� �Ҽ��� �ִ�.
// �ƴϸ� 2d����϶��� z����

RenderManager::RenderManager()
{
}


RenderManager::~RenderManager()
{
}

// ī�޶� �ֱ�
void RenderManager::Insert_Camera(Camera* _Camera)
{
	// ���� ī�޶� ���ٸ� ����
	if (nullptr == _Camera)
	{
		return;
	}

	// ī�޶� �׷쿡 �� ī�޶� �ִ��� �˻�
	if (m_CameraOrderGroup.end() == m_CameraOrderGroup.find(_Camera->m_Order))
	{
		// ���� ī�޶� �������� �ʴٸ� ī�޶� �־��ֱ�
		m_CameraOrderGroup.insert(std::multimap<int, My_PTR<Camera>>::value_type(_Camera->m_Order, _Camera));
		return;
	}

	// ī�޶� �ִٸ�
	// lower_bound : multimap���� ������ Ű���� ���ų� ū Ű�� ���� ù ��° ��ҿ� �ݺ��⸦ ��ȯ�մϴ�.
	// upper_bound : multimap���� ������ Ű���� ū Ű�� ���� ù ��° ��ҿ� �ݺ��⸦ ��ȯ�մϴ�.
	std::multimap<int, My_PTR<Camera>>::iterator Start_Iter = m_CameraOrderGroup.lower_bound(_Camera->m_Order);
	std::multimap<int, My_PTR<Camera>>::iterator End_Iter = m_CameraOrderGroup.upper_bound(_Camera->m_Order);

	for (; Start_Iter != End_Iter; ++Start_Iter)
	{
		// ���� ī�޶� �����Ѵٸ� return;
		if (_Camera == Start_Iter->second)
		{
			return;
		}
	}

	m_CameraOrderGroup.insert(std::multimap<int, My_PTR<Camera>>::value_type(_Camera->m_Order, _Camera));
}


// ���� Init

void RenderManager::Init()
{
	// ���ø��� ���� �߷п��� 3���� �߷� ����� ����
	// 1. ����Ʈ (�˾Ƽ� �˼� ����)
	// 2. ����� �߷�
	// 3. �Լ������� ���̴�..
}


//

void RenderManager::Render()
{
	// ������ۿ� ����
	//WorldMatrixUpdate();
	//ViewProjectionMatrixUpdate();
	//
	//
	// ���� ������ ���ؽ��� ��������� �ű������ ���
	//m_WorldViewAndProj = m_World * m_View * m_Projection;
	//
	//m_WorldViewAndProj.Transpos();
	//My_PTR<VertexShader> VTX = ResourceManager<VertexShader>::Find(L"COLORVERTEXSHADER");
	//VTX->UpdateConstBuffer<Matrix>(L"WVP", m_WorldViewAndProj);		// ���̴����� ���� ������ �ܺο��� �������ַ���..
	//
	//My_PTR<Mesh> FindMesh = ResourceManager<Mesh>::Find(L"COLORMESH");
	//
	//My_PTR<PixelShader> PIX = ResourceManager<PixelShader>::Find(L"COLORPIXELSHADER");
	//
	//VTX->Update();
	//PIX->Update();
	//
	//FindMesh->Update();
	//
	//
	//FindMesh->Render();

	m_StartCamera = m_CameraOrderGroup.begin();
	m_EndCamera = m_CameraOrderGroup.end();

	for (; m_StartCamera != m_EndCamera; ++m_StartCamera)
	{
		if (false == m_StartCamera->second->IsUpdate())
		{
			continue;
		}

		// �������� ( �� ī�޶� �ִ� ���������� ���� )
		Camera_Render(m_StartCamera);
	}

}


void RenderManager::Camera_Render(const std::multimap<int, My_PTR<Camera>>::iterator& _Iter)
{
	std::map<int, std::list<My_PTR<Renderer>>>::iterator Find_Group;

	int a = 0;

	for (size_t i = 0; i < _Iter->second->m_RenderOrder.size(); ++i)
	{
		Find_Group = m_RendererOrderGroup.find(_Iter->second->m_RenderOrder[i]);

		if (Find_Group == m_RendererOrderGroup.end())
		{
			EAssert(true);
			continue;
		}

		if (0 == Find_Group->second.size())
		{
			continue;
		}
		

		Group_Render(_Iter->second, Find_Group->second);
	}

}

void RenderManager::Group_Render(My_PTR<Camera> _Camera, std::list<My_PTR<Renderer>>& _List)
{
	int a = 0;
	RendererGroup_StartIter = _List.begin();
	RendererGroup_EndIter = _List.end();

	for (; RendererGroup_StartIter != RendererGroup_EndIter; ++RendererGroup_StartIter)
	{
		if (false == (*RendererGroup_StartIter)->IsUpdate())
		{
			// ������Ʈ�� false��� ( ������Ʈ ���ҰŸ�! )
			continue;
		}
		(*RendererGroup_StartIter)->Render(_Camera->m_ViewAndProj);
	}
}


void RenderManager::Insert_Renderer(Renderer* _Renderer)
{
	if (nullptr == _Renderer)
	{
		EAssert(true);
		return;
	}

	std::map<int, std::list<My_PTR<Renderer>>>::iterator Find_Group = m_RendererOrderGroup.find(_Renderer->m_RenderGroup);

	if (Find_Group == m_RendererOrderGroup.end())
	{
		m_RendererOrderGroup.insert(std::map<int, std::list<My_PTR<Renderer>>>::value_type(_Renderer->m_RenderGroup, std::list<My_PTR<Renderer>>()));
		Find_Group = m_RendererOrderGroup.find(_Renderer->m_RenderGroup);
	}

	Find_Group->second.push_back(_Renderer);
}

void RenderManager::RenderOrder_Create(int _Order)
{
	std::map<int, std::list<My_PTR<Renderer>>>::iterator Find_Group = m_RendererOrderGroup.find(_Order);

	if (Find_Group == m_RendererOrderGroup.end())
	{
		m_RendererOrderGroup.insert(std::map<int, std::list<My_PTR<Renderer>>>::value_type(_Order, std::list<My_PTR<Renderer>>()));
	}
}

void RenderManager::Release()
{
	// ī�޶� ���� ����
	// ī�޶� ���� ���������� Ȯ���ϰ� �������� �ʾҴٸ� �����.
	m_StartCamera = m_CameraOrderGroup.begin();
	m_EndCamera = m_CameraOrderGroup.end();

	for (; m_StartCamera != m_EndCamera;)
	{
		// ī�޶� �׾��ٸ� ī�޶� �׷쿡�� ����
		// �� ī�޶� �������ִ� ���������� �� �ʿ䰡 �����ϱ�
		if (true == m_StartCamera->second->IsDeath())
		{
			m_StartCamera = m_CameraOrderGroup.erase(m_StartCamera);
		}
		else
		{
			++m_StartCamera;
		}
	}

	RendererGroupOrder_StartIter = m_RendererOrderGroup.begin();
	RendererGroupOrder_EndIter = m_RendererOrderGroup.end();

	for (; RendererGroupOrder_StartIter != RendererGroupOrder_EndIter; ++RendererGroupOrder_StartIter)
	{
		if (0 >= RendererGroupOrder_StartIter->second.size())
		{
			// �������� �ƹ��͵� �������� �ʴ´ٸ� ���� ������ ������ �ʿ䰡 ����.
			continue;
		}

		RendererGroup_StartIter = RendererGroupOrder_StartIter->second.begin();
		RendererGroup_EndIter = RendererGroupOrder_StartIter->second.end();

		for (; RendererGroup_StartIter != RendererGroup_EndIter; )
		{
			if (true == (*RendererGroup_StartIter)->IsDeath())
			{
				// ���� �׾��ٸ�..
				// �����ϰ� ������ �����Ѿ��Ѵ�.
				RendererGroup_StartIter = RendererGroupOrder_StartIter->second.erase(RendererGroup_StartIter);
			}
			else
			{
				++RendererGroup_StartIter;
			}
		}
	}
}
