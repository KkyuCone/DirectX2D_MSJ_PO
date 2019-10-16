#pragma once
#include "ReferenceBase.h"
#include "TypeBase.h"
#include "NameBase.h"
#include "UpdateBase.h"
#include "SceneBase.h"
//#include "Component.h"
//#include "Transform.h"

#include <list>
#include <set>


// ���� ���
// : �Ϲ������� ��üȭ �� �� ���� �͵�, �ܼ��� ��ɵ鸸 ����
class Component;
class GameObject final : public ReferenceBase, public TypeBase, public NameBase, public UpdateBase, public SceneBase
{
public:
	friend class Scene;

public:
	void Death() override;

private:
	bool m_bOver;			// ���� �ѳ���� ������Ʈ������ ���� ���� '��'
	int m_Order;

	std::list<My_PTR<Component>> m_ComponentList;
	std::list<My_PTR<Component>>::iterator Component_StartIter;
	std::list<My_PTR<Component>>::iterator Component_EndIter;

public:
	int GetOrder()
	{
		return m_Order;
	}

	// �ѳ������..
	bool IsOver()
	{
		if (nullptr != m_pParent)
		{
			// �����ܰ�δ� ������ �θ� ���� �ѳ�������� ���� �Ǵ��ϱ� ������ 
			// �θ� �ִٸ� �ش� �θ� ���� �ѳ�������� �˸�ȴ�.
			// �θ� -> �ڽ� -> ���� ������ Ÿ��Ÿ�� �ö󰡼� �θ� �ٴٸ������� �� ������
			return m_pParent->IsOver();
		}

		return m_bOver;
	}

private:
	void SetOrdrer(int _Order)
	{
		m_Order = _Order;
	}

	void GameObject_SetScene(Scene* _pScene);

private:
	void PrevUpdate() override;
	void Update() override;
	void LateUpdate() override;
	void EndUpdate() override;
	void FinalUpdate() override;

private:
	friend class Transform;
	Transform* m_Transform;

private:
	// Ʈ������ ����
	void SetTransform(Transform* _Transform)
	{
		m_Transform = _Transform;
	}

public:
	Transform* GetTransform()
	{
		return m_Transform;
	}

private:
	template<typename ComponentType>
	ComponentType* Component_Create()
	{
		ComponentType* NewComponent = new ComponentType();
		NewComponent->RTTISetting();
		NewComponent->SetName(NewComponent->Get_TypeName_UNICODE().c_str());
		//NewComponent->SetScene(GetScene());
		NewComponent->SetGameObject(this);
		return NewComponent;
	}

public:
	// ������Ʈ �߰�
	template<typename ComponentType>
	My_PTR<ComponentType> Component_Add()
	{
		ComponentType* NewComponent = Component_Create<ComponentType>();

		//Component* InitPtr = NewComponent;

		// ������Ʈ�� �̹� �ִ��� üũ
		if (false == ComponentType::IsMultiCheck(this))
		{
			delete NewComponent;
			return nullptr;
		}

		if (false == NewComponent->Init())
		{
			delete NewComponent;
			return nullptr;
		}

		m_ComponentList.push_back(NewComponent);
		return NewComponent;
	}

	// ������Ʈ �߰�
	template<typename ComponentType, typename InitValue>
	My_PTR<ComponentType> Component_Add(const InitValue& _Data)
	{
		ComponentType* NewComponent = Component_Create<ComponentType>();

		//Component* InitPtr = NewComponent;

		// ������Ʈ�� �̹� �ִ��� üũ
		if (false == ComponentType::IsMultiCheck(this))
		{
			delete NewComponent;
			return nullptr;
		}

		if (false == NewComponent->Init(_Data))
		{
			delete NewComponent;
			return nullptr;
		}

		m_ComponentList.push_back(NewComponent);
		return NewComponent;
	}


public:
	// ������Ʈ ã��
	template<typename FindComType>
	My_PTR<FindComType> GetComponent()
	{
		std::list<My_PTR<Component>>::iterator Find_StartIter = m_ComponentList.begin();
		std::list<My_PTR<Component>>::iterator Find_EndIter = m_ComponentList.end();

		for (; Find_StartIter != Find_EndIter; ++Find_StartIter)
		{
			if (true == (*Find_StartIter)->IsTypeEqual<FindComType>())
			{
				// ã�� ������Ʈ ���� 
				return (*Find_StartIter);
			}
		}

		// ������Ʈ ��ã������  nullptr��ȯ

		return nullptr;
	}


	// ������Ʈ ����Ʈ��ȯ
	template<typename FindComType>
	std::list<My_PTR<FindComType>> GetChildAllComponent()
	{
		std::list<My_PTR<FindComType>> ReturnList;

		// �� �Լ� ���ο��� ReturnList�� push_back()�������ν�
		// ������ƮŸ���� ã����
		GetChildAllComponent(ReturnList);

		return ReturnList;
	}

private:
	// ���ø� �Լ��� ��� ������ Ÿ�� �߷��� �����ϴ�.
	// Ŭ�����ΰ��� �߷��� �Ұ����ϱ� ������ ������ ��������� �˷������.
	template<typename FindComType>
	void GetChildAllComponent(std::list<My_PTR<FindComType>>& _List)
	{
		std::list<My_PTR<Component>>::iterator Find_StartIter = m_ComponentList.begin();
		std::list<My_PTR<Component>>::iterator Find_EndIter = m_ComponentList.end();

		// �ش� ������Ʈ�� �����ϰ�(FindComType)
		for (; Find_StartIter != Find_EndIter; ++Find_StartIter)
		{
			// �� ������Ʈ�� ����ȯ�� �����ϴٸ� (������� �ٿ�ĳ������ �����Ѱ�) -> ��ĳ������ ���� ĳ���� �����ڸ� ���ʿ䰡 �����Ƿ� ������ʿ����
			// ��ӱ��������϶� dynamic_cast ��� -> �Ļ� Ŭ���� or ���� Ŭ���� Ÿ������ ��ȯ����
			// ĳ���� ���н� NULL������ or 0
			if (nullptr != (*Find_StartIter).IsConvert<FindComType>())
			{
				_List.push_back((*Find_StartIter));
			}
		}

		for (; Child_StartIter != Child_EndIter; ++Child_StartIter)
		{
			(*Child_StartIter)->GetChildAllComponent(_List);
		}
	}
private:
	GameObject();
	~GameObject();


	//////////////////////////////////////////// Child ����
private:
	// ����ϴ� child �׷�
	// ���� �����ӿ��� �ڽ��̵ȴٸ� ���⼭ �ϳ��� ���� �׾��ִٸ� �����ϰ� �ɰ���
	std::set<My_PTR<GameObject>> m_WaitChildGroup;

	GameObject* m_pParent;
	std::list<My_PTR<GameObject>> m_ChildList;
	std::list<My_PTR<GameObject>>::iterator Child_StartIter;
	std::list<My_PTR<GameObject>>::iterator Child_EndIter;

public:
	GameObject* GetParent() { return m_pParent; }
	void SetParent(My_PTR<GameObject> _ParentObj);

private:
	void ChildWaitCheck();

	/////////////////////////////////////////////////////// �� �ѳ���¾ֵ� �߰����ֱ�(+����)

private:
	void OverOn();
	void PushOverScene(const wchar_t* _SceneName);
public:
	template<typename ...Rest>
	void Over(Rest ...Arg)
	{
		if (nullptr != GetParent())
		{
			EAssert(true);
			return;
		}
		OverOn();			// ������..(���� �ѳ��� ���ӿ�����Ʈ����) �̸� ����� ����
		OverPush(Arg...);
	}

	template<typename T, typename ...Rest>
	void OverPush(const T& _SceneName, Rest ...Arg)
	{
		// �����߰��߰�~ -> ���� ������Ʈ�� �ѳ�������� �߰��ϴ°��� 
		PushOverScene(_SceneName);		//  ���ӿ�����Ʈ(����)�� �ش�������� �ѳ����..������ �߰�
		OverPush(Arg...);				// ���..��� ������ (�Ѱ�����������)
	}

	template<typename T>
	void OverPush(const T& _SceneName)
	{
		// ������ ���߰�~
		PushOverScene(_SceneName);
	}

};

