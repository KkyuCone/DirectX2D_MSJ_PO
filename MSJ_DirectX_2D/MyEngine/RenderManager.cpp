#include "RenderManager.h"
#include "Window.h"


///////////////////////// 카메라의 집합이 있다.

// 카메라 하나가 랜더링할
// Cam을 관리하는 멀티맵의 키는
// 캠의 순서를 의미하게 되고
// 멀티맵이므로 순서가 같은 녀석들이 있을수 있지만.
// 그때의 순서는 신경쓰지 않기로 한다.

// 랜더러들의 집합이 있는 것이다.
// 랜더러들의 집합은 각 랜더그룹을 의미하며
// 랜더의 순서를 의미하게 된다.
// 나중에 알파를 사용하게 되면
// 이 z의 순서나 혹은 z버퍼를 꺼야 할수도 있다.
// 그건 여러분들이 알아서 하시오.
// std::map<int, std::list<HPTR<HRenderer>>> 이 stl중
// std::list<HPTR<HRenderer>>을 z값에 따라서 정렬해야하 할수도 있다.
// 아니면 2d모드일때는 z버퍼

RenderManager::RenderManager()
{
}


RenderManager::~RenderManager()
{
}

// 카메라 넣기
void RenderManager::Insert_Camera(Camera* _Camera)
{
	// 받은 카메라가 없다면 리턴
	if (nullptr == _Camera)
	{
		return;
	}

	// 카메라 그룹에 이 카메라가 있는지 검사
	if (m_CameraOrderGroup.end() == m_CameraOrderGroup.find(_Camera->m_Order))
	{
		// 같은 카메라가 존재하지 않다면 카메라 넣어주기
		m_CameraOrderGroup.insert(std::multimap<int, My_PTR<Camera>>::value_type(_Camera->m_Order, _Camera));
		return;
	}

	// 카메라가 있다면
	// lower_bound : multimap에서 지정된 키보다 같거나 큰 키를 가진 첫 번째 요소에 반복기를 반환합니다.
	// upper_bound : multimap에서 지정된 키보다 큰 키를 가진 첫 번째 요소에 반복기를 반환합니다.
	std::multimap<int, My_PTR<Camera>>::iterator Start_Iter = m_CameraOrderGroup.lower_bound(_Camera->m_Order);
	std::multimap<int, My_PTR<Camera>>::iterator End_Iter = m_CameraOrderGroup.upper_bound(_Camera->m_Order);

	for (; Start_Iter != End_Iter; ++Start_Iter)
	{
		// 같은 카메라가 존재한다면 return;
		if (_Camera == Start_Iter->second)
		{
			return;
		}
	}

	m_CameraOrderGroup.insert(std::multimap<int, My_PTR<Camera>>::value_type(_Camera->m_Order, _Camera));
}


// 렌더 Init

void RenderManager::Init()
{
	// 템플릿의 인자 추론에는 3가지 추론 방식이 존재
	// 1. 디폴트 (알아서 알수 있음)
	// 2. 명시적 추론
	// 3. 함수에서만 쓰이는..
}


//

void RenderManager::Render()
{
	// 상수버퍼와 관련
	//WorldMatrixUpdate();
	//ViewProjectionMatrixUpdate();
	//
	//
	// 월드 공간의 버텍스를 뷰공간으로 옮기기위한 행렬
	//m_WorldViewAndProj = m_World * m_View * m_Projection;
	//
	//m_WorldViewAndProj.Transpos();
	//My_PTR<VertexShader> VTX = ResourceManager<VertexShader>::Find(L"COLORVERTEXSHADER");
	//VTX->UpdateConstBuffer<Matrix>(L"WVP", m_WorldViewAndProj);		// 쉐이더에서 쓰는 변수를 외부에서 세팅해주려고..
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

		// 렌더돌기 ( 각 카메라에 있는 렌더러들을 돌림 )
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
			// 업데이트가 false라면 ( 업데이트 안할거면! )
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
	// 카메라 먼저 돌기
	// 카메라가 먼저 지워졌는지 확인하고 지워지지 않았다면 지운다.
	m_StartCamera = m_CameraOrderGroup.begin();
	m_EndCamera = m_CameraOrderGroup.end();

	for (; m_StartCamera != m_EndCamera;)
	{
		// 카메라가 죽었다면 카메라 그룹에서 삭제
		// 그 카메라가 가지고있는 렌더러들을 볼 필요가 없으니까
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
			// 렌더러가 아무것도 렌더하지 않는다면 굳이 다음을 실행할 필요가 없다.
			continue;
		}

		RendererGroup_StartIter = RendererGroupOrder_StartIter->second.begin();
		RendererGroup_EndIter = RendererGroupOrder_StartIter->second.end();

		for (; RendererGroup_StartIter != RendererGroup_EndIter; )
		{
			if (true == (*RendererGroup_StartIter)->IsDeath())
			{
				// 만약 죽었다면..
				// 삭제하고 다음걸 가리켜야한다.
				RendererGroup_StartIter = RendererGroupOrder_StartIter->second.erase(RendererGroup_StartIter);
			}
			else
			{
				++RendererGroup_StartIter;
			}
		}
	}
}
