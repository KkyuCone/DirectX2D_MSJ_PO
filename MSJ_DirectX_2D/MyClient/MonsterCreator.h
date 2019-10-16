#pragma once
#include <Scene.h>

class MonsterDataInit
{
public:
	int MonsterMaxCount;
	float MonsterGenTime;
};

class GameObject;
class MonsterCreator : public Scene::SceneScript
{
private:
	My_PTR<GameObject> NewActor;
	My_PTR<GameObject> TestActor;


public:
	My_PTR<GameObject> GameObject2D_Create(const wchar_t* _Name);


public:
	bool Loading() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void End() override;

public:
	MonsterCreator();
	~MonsterCreator();
};

