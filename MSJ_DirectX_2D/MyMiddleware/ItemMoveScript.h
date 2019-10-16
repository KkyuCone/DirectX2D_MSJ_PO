#pragma once
#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

#define ITEMFORCE 400

class GameObject;
class ItemMoveScript : public Script
{
private:
	My_PTR<SpriteRenderer2D> m_Renderer;
	My_PTR<Animation2D> m_Animation;

private:
	bool m_bDrop;
	bool m_bDownCheck;
private:
	float m_JumpForce;
	GameObject* m_Player;
	GameObject* m_Floors;			// �ȼ��浹�Ϸ��� �߰�

private:
	float m_CurTime;

private:
	int m_ItemState;			// 1�� ���Ͱ� �������� ������, 2�� �������� ȹ��������

private:
	void ItemJumpUp();
	void ItemJumpDown();

	void GDown();

public:
	void SetItemState(int _Value);
	void SetPlayer(GameObject* _Obj);
	void SetFloor(GameObject* _Floor);
	void SetDrop(bool _Value);

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	ItemMoveScript();
	~ItemMoveScript();
};

