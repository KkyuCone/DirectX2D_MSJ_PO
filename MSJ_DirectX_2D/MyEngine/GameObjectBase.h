#pragma once

class GameObject;
class GameObjectBase
{
private:
	friend GameObject;
	GameObject* m_pGameObject;

public:
	GameObject* GetGameObject()
	{
		return m_pGameObject;
	}

private:
	void SetGameObject(GameObject* _GameObject)
	{
		m_pGameObject = _GameObject;
	}

public:
	GameObjectBase() {}
	~GameObjectBase() {}
};
