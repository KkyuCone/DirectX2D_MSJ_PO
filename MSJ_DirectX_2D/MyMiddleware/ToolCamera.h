#pragma once
#include <Script.h>
#include <Input.h>

class Camera;
class ToolCamera : public Script
{

private:
	int m_LimitX;
	int m_LimitY;
	float m_LX;
	float m_LY;
	float m_MoveSpeed;

	My_PTR<Camera> m_CameraPTR;

private:
	void InitKey();
	void Move();
	void ZoomInOut();

public:
	void SetLimitPos(int _SceneX, int _Scene_Y);
	void SetCamera(My_PTR<Camera> _Camera);

public:
	bool Init() override;
	void Update() override;

public:
	ToolCamera();
	~ToolCamera();
};

