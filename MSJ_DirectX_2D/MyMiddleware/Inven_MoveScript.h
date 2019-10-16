#pragma once
#include <Script.h>

class GameObject;
class Camera;
class RectCol2D;
class Inven_MoveScript : public Script
{
private:
	bool m_bMove;
	Vector4 m_BasePos;
	Vector2 m_MouseBasePos;

	std::vector<RectCol2D*> m_InvenButtonCol;
	std::vector<RectCol2D*> m_InvenSlotCol_Equip;
	//std::vector<RectCol2D*> m_InvenSlotCol_;
	//std::vector<RectCol2D*> m_InvenSlotCol_Equip;
	//std::vector<RectCol2D*> m_InvenSlotCol_Equip;

private:
	GameObject* m_InventoryMoveObject;
	Camera* m_UICamera;

private:
	void Move();

public:
	void SetMoveObject(GameObject* _MoveOBject);
	GameObject* GetMoveObejct();

	void SetCamera(Camera* _Camera);

	void SetbMove(bool _Value);
	bool GetbMove();

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	Inven_MoveScript();
	~Inven_MoveScript();
};

