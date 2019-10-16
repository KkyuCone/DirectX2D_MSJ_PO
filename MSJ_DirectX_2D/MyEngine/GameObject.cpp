#include "GameObject.h"
#include "Scene.h"
#include "Component.h"
#include "Transform.h"



GameObject::GameObject() : m_Transform(nullptr), m_Order(0)
{
}


GameObject::~GameObject()
{
}


// 해당 오브젝트의 씬을 Set해준다.
// SetScene에 대해.. -ㅅ- 좀꼬여서 이렇게 만들었음
// 왜냐면 그냥 SetScene만 할경우 내꺼만 씬을 설정해주고 자식들한테는 설정을 못해주기때문이다.
// 'ㅅ' 자식들에게도 해당 씬이 뭔지 알려주기 위해서 돌린다~
void GameObject::GameObject_SetScene(Scene* _pScene)
{
	SetScene(_pScene);		// 일단 씬 넣어 주공~

	// 자식들에 대해서도 씬을 설정해줘야함!!
	std::list<My_PTR<GameObject>>::iterator MyChild_StartIter = m_ChildList.begin();
	std::list<My_PTR<GameObject>>::iterator MyChild_EndIter = m_ChildList.end();

	for (; MyChild_StartIter != MyChild_EndIter; ++MyChild_StartIter)
	{
		(*MyChild_StartIter)->GameObject_SetScene(_pScene);
	}
}

//

void GameObject::Death()
{
	// 사망
	std::list<My_PTR<Component>>::iterator Component_StartIter = m_ComponentList.begin();
	std::list<My_PTR<Component>>::iterator Component_EndIter = m_ComponentList.end();

	// 해당 컴포넌트들 해제
	for (; Component_StartIter != Component_EndIter; ++Component_StartIter)
	{
		(*Component_StartIter)->SetGameObject(nullptr);
		(*Component_StartIter)->Death();
	}

	// 자식들 지우기
	std::list<My_PTR<GameObject>>::iterator Child_StartIter = m_ChildList.begin();
	std::list<My_PTR<GameObject>>::iterator Child_EndIter = m_ChildList.end();

	for (; Child_StartIter != Child_EndIter; ++Child_StartIter)
	{
		(*Child_StartIter)->m_pParent = nullptr;			// 자식의 부모 지우기(나를 지우는것)
		(*Child_StartIter)->Death();						// 자식 사망 ( 자식이 내부에서 자기의 자식, 컴포넌트 해제함)
	}

	UpdateBase::Death();			// 이제 본인(게임오브젝트)지우기
}

void GameObject::PrevUpdate()
{
	Component_StartIter = m_ComponentList.begin();
	Component_EndIter = m_ComponentList.end();

	for (; Component_StartIter != Component_EndIter; ++Component_StartIter)
	{
		if (true == (*Component_StartIter)->IsUpdate())
		{
			(*Component_StartIter)->PrevUpdate();
		}
	}


	// 자식 리스트 검사
	Child_StartIter = m_ChildList.begin();
	Child_EndIter = m_ChildList.end();


	for (; Child_StartIter != Child_EndIter; ++Child_StartIter)
	{
		if (true == (*Child_StartIter)->IsUpdate())
		{
			const wchar_t* GenName = (*Child_StartIter)->GetName();
			// 자식이 있다면 업데이트 실행
			(*Child_StartIter)->PrevUpdate();
		}
	}
}

void GameObject::Update()
{
	Component_StartIter = m_ComponentList.begin();
	Component_EndIter = m_ComponentList.end();

	for (; Component_StartIter != Component_EndIter; ++Component_StartIter)
	{
		if (true == (*Component_StartIter)->IsUpdate())
		{
			(*Component_StartIter)->Update();
		}
	}

	// 자식 리스트 검사
	Child_StartIter = m_ChildList.begin();
	Child_EndIter = m_ChildList.end();

	for (; Child_StartIter != Child_EndIter; ++Child_StartIter)
	{
		if (true == (*Child_StartIter)->IsUpdate())
		{
			// 자식이 있다면 업데이트 실행
			(*Child_StartIter)->Update();
		}
	}
}

void GameObject::LateUpdate()
{
	Component_StartIter = m_ComponentList.begin();
	Component_EndIter = m_ComponentList.end();

	for (; Component_StartIter != Component_EndIter; ++Component_StartIter)
	{
		if (true == (*Component_StartIter)->IsUpdate())
		{
			(*Component_StartIter)->LateUpdate();
		}
	}

	// 자식 리스트 검사
	Child_StartIter = m_ChildList.begin();
	Child_EndIter = m_ChildList.end();

	for (; Child_StartIter != Child_EndIter; ++Child_StartIter)
	{
		if (true == (*Child_StartIter)->IsUpdate())
		{
			// 자식이 있다면 업데이트 실행
			(*Child_StartIter)->LateUpdate();
		}
	}
}

void GameObject::EndUpdate()
{
	Component_StartIter = m_ComponentList.begin();
	Component_EndIter = m_ComponentList.end();

	for (; Component_StartIter != Component_EndIter; ++Component_StartIter)
	{
		if (true == (*Component_StartIter)->IsUpdate())
		{
			(*Component_StartIter)->EndUpdate();
		}
	}

	// 자식 리스트 검사 ( 자식 업데이트 돌려주는거임 ㅇㅅㅇ)
	Child_StartIter = m_ChildList.begin();
	Child_EndIter = m_ChildList.end();

	for (; Child_StartIter != Child_EndIter; ++Child_StartIter)
	{
		if (true == (*Child_StartIter)->IsUpdate())
		{
			// 자식이 있다면 업데이트 실행
			(*Child_StartIter)->EndUpdate();
		}
	}
}

void GameObject::FinalUpdate()
{
	Component_StartIter = m_ComponentList.begin();
	Component_EndIter = m_ComponentList.end();

	for (; Component_StartIter != Component_EndIter; ++Component_StartIter)
	{
		if (true == (*Component_StartIter)->IsUpdate())
		{
			(*Component_StartIter)->FinalUpdate();
		}
	}

	// 자식 리스트 검사 ( 자식 업데이트 돌려주는거임 ㅇㅅㅇ)
	Child_StartIter = m_ChildList.begin();
	Child_EndIter = m_ChildList.end();

	for (; Child_StartIter != Child_EndIter; ++Child_StartIter)
	{
		if (true == (*Child_StartIter)->IsUpdate())
		{
			// 자식이 있다면 업데이트 실행
			(*Child_StartIter)->FinalUpdate();
		}
	}
}

void GameObject::SetParent(My_PTR<GameObject> _ParentObj)
{
	// 내가 부모로 해줄 오브젝트에게 
	// 내가 네 자식이얌!!
	// 하고 알려줌 ㅇㅅㅇ
	//_ParentObj->m_ChildList.push_back(this);

	// 같은 부모를 두번 넣어주기 않기 위해 찾기시작~
	std::set<My_PTR<GameObject>>::iterator Find_Iter = _ParentObj->m_WaitChildGroup.find(this);

	if (Find_Iter != _ParentObj->m_WaitChildGroup.end())
	{
		// 해당 부모가 이미 Set되어있다면 return.
		// or 없다면..
		return;
	}

	m_pParent = _ParentObj;			// 부모설정

	if (nullptr != GetTransform())
	{
		// 월드 크기 계산
		GetTransform()->CalWScale();
	}

	// 자식오브젝트에 부모가 누군지 먼저 설정해준후에
	// 부모 오브젝트에서 자식 대기그룹에 자식이될 오브젝트를 넣어주고
	// 자식 오브젝트리스트에 하나씩 자식들을 넣어준다.
	_ParentObj->m_WaitChildGroup.insert(this);
}


void GameObject::ChildWaitCheck()
{
	// 예비자식들이 하나도 없다면 ㅇㅅㅇ
	if (true == m_WaitChildGroup.empty())
	{
		return;
	}

	std::set<My_PTR<GameObject>>::iterator Start_Iter = m_WaitChildGroup.begin();
	std::set<My_PTR<GameObject>>::iterator End_Iter = m_WaitChildGroup.end();

	for (; Start_Iter != End_Iter; )
	{
		// 예비 자식이 있다면
		// 그 자식에게 부모를 나로 지정
		(*Start_Iter)->m_pParent = this;

		(*Start_Iter)->GetTransform()->m_bWorldUpdate = true;			// 월드 업데이트(계산)을 위해서 true로 설정(부모계산도 합해서..)
		(*Start_Iter)->GetTransform()->m_UpdateGeometry = -1;

		m_ChildList.push_back(*Start_Iter);
		Start_Iter = m_WaitChildGroup.erase(Start_Iter);		// 예비자식에게 부모를 설정해줬으므로 예비자식그룹에서는 나가야함
	}
}

// 이 함수를 사용하는 오브젝트는
// 씬을 넘나드는 오브젝트로 설정해줄것임.
// OverOn() -> 씬을 넘나드는 오브젝트로 설정해주겠다는 함수
void GameObject::OverOn()
{
	Scene::OverGameObjectInfo_Create(this);
	m_bOver = true; 
}

void GameObject::PushOverScene(const wchar_t* _SceneName)
{
	Scene::PushOverInfo(this, _SceneName);
}
