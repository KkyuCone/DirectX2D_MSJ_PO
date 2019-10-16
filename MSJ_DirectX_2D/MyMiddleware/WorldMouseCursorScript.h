#pragma once
#include <Script.h>

class Camera;
class GameObject;
class WorldMouseCursorScript : public Script
{
private:
	Camera* m_Camera;

private:
	void WorldMousePos();

public:
	void SetCamera(Camera* _Camera);

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;
	
public:
	WorldMouseCursorScript();
	~WorldMouseCursorScript();
};

