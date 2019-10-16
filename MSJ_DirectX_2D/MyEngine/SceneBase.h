#pragma once

class Scene;
class GameObject;
class SceneBase
{
private:
	friend Scene;
	friend GameObject;
	Scene* m_pScene;

public:
	virtual Scene* GetScene()
	{
		return m_pScene;
	}

private:
	void SetScene(Scene* _pScene)
	{
		m_pScene = _pScene;
	}

public:
	SceneBase() {}
	~SceneBase() {}
};