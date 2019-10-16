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


// �ش� ������Ʈ�� ���� Set���ش�.
// SetScene�� ����.. -��- �������� �̷��� �������
// �ֳĸ� �׳� SetScene�� �Ұ�� ������ ���� �������ְ� �ڽĵ����״� ������ �����ֱ⶧���̴�.
// '��' �ڽĵ鿡�Ե� �ش� ���� ���� �˷��ֱ� ���ؼ� ������~
void GameObject::GameObject_SetScene(Scene* _pScene)
{
	SetScene(_pScene);		// �ϴ� �� �־� �ְ�~

	// �ڽĵ鿡 ���ؼ��� ���� �����������!!
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
	// ���
	std::list<My_PTR<Component>>::iterator Component_StartIter = m_ComponentList.begin();
	std::list<My_PTR<Component>>::iterator Component_EndIter = m_ComponentList.end();

	// �ش� ������Ʈ�� ����
	for (; Component_StartIter != Component_EndIter; ++Component_StartIter)
	{
		(*Component_StartIter)->SetGameObject(nullptr);
		(*Component_StartIter)->Death();
	}

	// �ڽĵ� �����
	std::list<My_PTR<GameObject>>::iterator Child_StartIter = m_ChildList.begin();
	std::list<My_PTR<GameObject>>::iterator Child_EndIter = m_ChildList.end();

	for (; Child_StartIter != Child_EndIter; ++Child_StartIter)
	{
		(*Child_StartIter)->m_pParent = nullptr;			// �ڽ��� �θ� �����(���� ����°�)
		(*Child_StartIter)->Death();						// �ڽ� ��� ( �ڽ��� ���ο��� �ڱ��� �ڽ�, ������Ʈ ������)
	}

	UpdateBase::Death();			// ���� ����(���ӿ�����Ʈ)�����
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


	// �ڽ� ����Ʈ �˻�
	Child_StartIter = m_ChildList.begin();
	Child_EndIter = m_ChildList.end();


	for (; Child_StartIter != Child_EndIter; ++Child_StartIter)
	{
		if (true == (*Child_StartIter)->IsUpdate())
		{
			const wchar_t* GenName = (*Child_StartIter)->GetName();
			// �ڽ��� �ִٸ� ������Ʈ ����
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

	// �ڽ� ����Ʈ �˻�
	Child_StartIter = m_ChildList.begin();
	Child_EndIter = m_ChildList.end();

	for (; Child_StartIter != Child_EndIter; ++Child_StartIter)
	{
		if (true == (*Child_StartIter)->IsUpdate())
		{
			// �ڽ��� �ִٸ� ������Ʈ ����
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

	// �ڽ� ����Ʈ �˻�
	Child_StartIter = m_ChildList.begin();
	Child_EndIter = m_ChildList.end();

	for (; Child_StartIter != Child_EndIter; ++Child_StartIter)
	{
		if (true == (*Child_StartIter)->IsUpdate())
		{
			// �ڽ��� �ִٸ� ������Ʈ ����
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

	// �ڽ� ����Ʈ �˻� ( �ڽ� ������Ʈ �����ִ°��� ������)
	Child_StartIter = m_ChildList.begin();
	Child_EndIter = m_ChildList.end();

	for (; Child_StartIter != Child_EndIter; ++Child_StartIter)
	{
		if (true == (*Child_StartIter)->IsUpdate())
		{
			// �ڽ��� �ִٸ� ������Ʈ ����
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

	// �ڽ� ����Ʈ �˻� ( �ڽ� ������Ʈ �����ִ°��� ������)
	Child_StartIter = m_ChildList.begin();
	Child_EndIter = m_ChildList.end();

	for (; Child_StartIter != Child_EndIter; ++Child_StartIter)
	{
		if (true == (*Child_StartIter)->IsUpdate())
		{
			// �ڽ��� �ִٸ� ������Ʈ ����
			(*Child_StartIter)->FinalUpdate();
		}
	}
}

void GameObject::SetParent(My_PTR<GameObject> _ParentObj)
{
	// ���� �θ�� ���� ������Ʈ���� 
	// ���� �� �ڽ��̾�!!
	// �ϰ� �˷��� ������
	//_ParentObj->m_ChildList.push_back(this);

	// ���� �θ� �ι� �־��ֱ� �ʱ� ���� ã�����~
	std::set<My_PTR<GameObject>>::iterator Find_Iter = _ParentObj->m_WaitChildGroup.find(this);

	if (Find_Iter != _ParentObj->m_WaitChildGroup.end())
	{
		// �ش� �θ� �̹� Set�Ǿ��ִٸ� return.
		// or ���ٸ�..
		return;
	}

	m_pParent = _ParentObj;			// �θ���

	if (nullptr != GetTransform())
	{
		// ���� ũ�� ���
		GetTransform()->CalWScale();
	}

	// �ڽĿ�����Ʈ�� �θ� ������ ���� ���������Ŀ�
	// �θ� ������Ʈ���� �ڽ� ���׷쿡 �ڽ��̵� ������Ʈ�� �־��ְ�
	// �ڽ� ������Ʈ����Ʈ�� �ϳ��� �ڽĵ��� �־��ش�.
	_ParentObj->m_WaitChildGroup.insert(this);
}


void GameObject::ChildWaitCheck()
{
	// �����ڽĵ��� �ϳ��� ���ٸ� ������
	if (true == m_WaitChildGroup.empty())
	{
		return;
	}

	std::set<My_PTR<GameObject>>::iterator Start_Iter = m_WaitChildGroup.begin();
	std::set<My_PTR<GameObject>>::iterator End_Iter = m_WaitChildGroup.end();

	for (; Start_Iter != End_Iter; )
	{
		// ���� �ڽ��� �ִٸ�
		// �� �ڽĿ��� �θ� ���� ����
		(*Start_Iter)->m_pParent = this;

		(*Start_Iter)->GetTransform()->m_bWorldUpdate = true;			// ���� ������Ʈ(���)�� ���ؼ� true�� ����(�θ��굵 ���ؼ�..)
		(*Start_Iter)->GetTransform()->m_UpdateGeometry = -1;

		m_ChildList.push_back(*Start_Iter);
		Start_Iter = m_WaitChildGroup.erase(Start_Iter);		// �����ڽĿ��� �θ� �����������Ƿ� �����ڽı׷쿡���� ��������
	}
}

// �� �Լ��� ����ϴ� ������Ʈ��
// ���� �ѳ���� ������Ʈ�� �������ٰ���.
// OverOn() -> ���� �ѳ���� ������Ʈ�� �������ְڴٴ� �Լ�
void GameObject::OverOn()
{
	Scene::OverGameObjectInfo_Create(this);
	m_bOver = true; 
}

void GameObject::PushOverScene(const wchar_t* _SceneName)
{
	Scene::PushOverInfo(this, _SceneName);
}
