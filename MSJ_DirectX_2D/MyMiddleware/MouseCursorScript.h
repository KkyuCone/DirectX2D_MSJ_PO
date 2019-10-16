#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class Camera;
class MouseCursorScript : public Script
{
private:
	Camera* m_Camera;

private:
	My_PTR<SpriteRenderer2D> m_Renderer;
	My_PTR<Animation2D> m_Animation;
	 
private:
	void Move();

public:
	void SetCamera(Camera* _Camera);

public:
	void MouseStateChange(std::wstring _State);			// ���콺 �ִϸ��̼� �ٲٱ�

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	MouseCursorScript();
	~MouseCursorScript();
};

