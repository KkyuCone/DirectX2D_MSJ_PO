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
	// �Ʒ��� ���漱���� ���� '��'
	class Scene* GetScene();

public:
	// �ش� ������Ʈ�� �̹� �ִ��� üũ
	// ���� ��� Transform ������Ʈ�� ���ӿ�����Ʈ�� �Ѱ��� �����ؾ��ϱ� ������
	// �ߺ����� �߰����� ���ϰ� ���� ���
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

