#pragma once
#include "ReferenceBase.h"
#include "TypeBase.h"
#include "NameBase.h"
#include "UpdateBase.h"
#include "SceneBase.h"
#include "GameObjectBase.h"
#include "GameObject.h"

class Transform;
class Component 
	: public ReferenceBase, public TypeBase, public NameBase, public UpdateBase, public GameObjectBase
{
public:
	friend class GameObject;

public:
	Transform* GetTransform();

public:
	// 아래도 전방선언의 종류 'ㅅ'
	class Scene* GetScene();

public:
	// 해당 컴포넌트가 이미 있는지 체크
	// 예를 들어 Transform 컴포넌트는 게임오브젝트당 한개만 존재해야하기 때문에
	// 중복으로 추가하지 못하게 위해 사용
	static bool IsMultiCheck(GameObject* _GameObject);

public:
	virtual bool IsUpdate();
	virtual bool IsDeath();


public:
	template<typename ComponentType>
	My_PTR<ComponentType> Component_Add()
	{
		return GetGameObject()->Component_Add<ComponentType>();
	}

	template<typename ComponentType, typename InitValue>
	My_PTR<ComponentType> Component_Add(const InitValue& _Data)
	{
		return GetGameObject()->Component_Add<ComponentType, InitValue>(_Data);
	}

	template<typename ComponentType>
	My_PTR<ComponentType> GetComponent()
	{
		return GetGameObject()->GetComponent<ComponentType>();
	}

public:
	Component();
	virtual ~Component() = 0;
};

