#include "Component.h"
#include "GameObject.h"



Component::Component()
{
}


Component::~Component()
{
}

//

Scene* Component::GetScene()
{
	if (nullptr == GetGameObject())
	{
		return nullptr;
	}

	return GetGameObject()->GetScene();
}

Transform* Component::GetTransform()
{
	if (nullptr == GetGameObject())
	{
		return nullptr;
	}

	return GetGameObject()->GetTransform();
}

bool Component::IsMultiCheck(GameObject* _GameObject)
{
	return true;
}

bool Component::IsUpdate()
{
	if (nullptr == GetGameObject())
	{
		return false;
	}

	return UpdateBase::IsUpdate() && GetGameObject()->IsUpdate();
}

bool Component::IsDeath()
{
	if (nullptr == GetGameObject())
	{
		return true;
	}

	return UpdateBase::IsDeath() || GetGameObject()->IsDeath();
}
