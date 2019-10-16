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


// 다중 상속
// : 일반적으로 객체화 될 수 없는 것들, 단순한 기능들만 존재
class Component;
class GameObject final : public ReferenceBase, public TypeBase, public NameBase, public UpdateBase, public SceneBase
{
public:
	friend class Scene;

public:
	void Death() override;

private:
	bool m_bOver;			// 씬을 넘나드는 오브젝트인지에 대한 변수 'ㅅ'
	int m_Order;

	std::list<My_PTR<Component>> m_ComponentList;
	std::list<My_PTR<Component>>::iterator Component_StartIter;
	std::list<My_PTR<Component>>::iterator Component_EndIter;

public:
	int GetOrder()
	{
		return m_Order;
	}

	// 넘나드는지..
	bool IsOver()
	{
		if (nullptr != m_pParent)
		{
			// 최종단계로는 어차피 부모가 씬을 넘나드는지에 대해 판단하기 떄문에 
			// 부모가 있다면 해당 부모가 씬을 넘나드는지만 알면된다.
			// 부모 -> 자식 -> 손자 관계라면 타고타고 올라가서 부모에 다다를때까지 들어감 ㅇㅅㅇ
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
	// 트랜스폼 셋팅
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
	// 컴포넌트 추가
	template<typename ComponentType>
	My_PTR<ComponentType> Component_Add()
	{
		ComponentType* NewComponent = Component_Create<ComponentType>();

		//Component* InitPtr = NewComponent;

		// 컴포넌트가 이미 있는지 체크
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

	// 컴포넌트 추가
	template<typename ComponentType, typename InitValue>
	My_PTR<ComponentType> Component_Add(const InitValue& _Data)
	{
		ComponentType* NewComponent = Component_Create<ComponentType>();

		//Component* InitPtr = NewComponent;

		// 컴포넌트가 이미 있는지 체크
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
	// 컴포넌트 찾기
	template<typename FindComType>
	My_PTR<FindComType> GetComponent()
	{
		std::list<My_PTR<Component>>::iterator Find_StartIter = m_ComponentList.begin();
		std::list<My_PTR<Component>>::iterator Find_EndIter = m_ComponentList.end();

		for (; Find_StartIter != Find_EndIter; ++Find_StartIter)
		{
			if (true == (*Find_StartIter)->IsTypeEqual<FindComType>())
			{
				// 찾은 컴포넌트 리턴 
				return (*Find_StartIter);
			}
		}

		// 컴포넌트 못찾았으면  nullptr반환

		return nullptr;
	}


	// 컴포넌트 리스트반환
	template<typename FindComType>
	std::list<My_PTR<FindComType>> GetChildAllComponent()
	{
		std::list<My_PTR<FindComType>> ReturnList;

		// 이 함수 내부에서 ReturnList를 push_back()해줌으로써
		// 컴포넌트타입을 찾아줌
		GetChildAllComponent(ReturnList);

		return ReturnList;
	}

private:
	// 템플릿 함수의 경우 스스로 타입 추론이 가능하다.
	// 클래스인경우는 추론이 불가능하기 때문에 무조건 명시적으로 알려줘야함.
	template<typename FindComType>
	void GetChildAllComponent(std::list<My_PTR<FindComType>>& _List)
	{
		std::list<My_PTR<Component>>::iterator Find_StartIter = m_ComponentList.begin();
		std::list<My_PTR<Component>>::iterator Find_EndIter = m_ComponentList.end();

		// 해당 컴포넌트가 존재하고(FindComType)
		for (; Find_StartIter != Find_EndIter; ++Find_StartIter)
		{
			// 그 컴포넌트가 형변환이 가능하다면 (예를들어 다운캐스팅이 가능한가) -> 업캐스팅은 굳이 캐스터 연산자를 쓸필요가 없으므로 사용할필요없음
			// 상속구조관계일때 dynamic_cast 사용 -> 파생 클래스 or 형제 클래스 타입으로 변환해줌
			// 캐스팅 실패시 NULL포인터 or 0
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


	//////////////////////////////////////////// Child 구조
private:
	// 대기하는 child 그룹
	// 다음 프레임에서 자식이된다면 여기서 하나씩 빼고 죽어있다면 삭제하게 될거임
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

	/////////////////////////////////////////////////////// 씬 넘나드는애들 추가해주기(+관리)

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
		OverOn();			// 정보를..(씬을 넘나들 게임오브젝트정보) 미리 만들어 놓기
		OverPush(Arg...);
	}

	template<typename T, typename ...Rest>
	void OverPush(const T& _SceneName, Rest ...Arg)
	{
		// 씬들추가추가~ -> 현재 오브젝트가 넘나들씬들을 추가하는거임 
		PushOverScene(_SceneName);		//  게임오브젝트(현재)를 해당씬들한테 넘나들게..씬정보 추가
		OverPush(Arg...);				// 재귀..계속 돌거임 (한개남을때까지)
	}

	template<typename T>
	void OverPush(const T& _SceneName)
	{
		// 마지막 씬추가~
		PushOverScene(_SceneName);
	}

};

