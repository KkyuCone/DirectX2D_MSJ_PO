#pragma once
#include "RenderOrder.h"

#include <Script.h>
#include <SpriteRenderer2D.h>
#include <Animation2D.h>

class GameObject;
class QuestAlarmElixirNumberScript : public Script
{
private:
	bool m_bQuestCom;
	int m_Count;

private:
	My_PTR<GameObject> m_UnitNumber;
	My_PTR<GameObject> m_TensNumber;

	My_PTR<SpriteRenderer2D> m_UnitNumberRenderer;
	My_PTR<SpriteRenderer2D> m_TensNumberRenderer;

	std::vector<GameObject*> m_ConsumeVector;

private:
	void NumberCheck();
	void SetCount(int _Count);

public:
	void AllNumber_Off();
	void AllNumber_On();

	void Number_Off(int _Value);			// 10, 1�� ��츸 �ش� 1�� 1�� �ڸ� 10�� 10�� �ڸ�
	void Number_On(int _Value);

public:
	bool GetQuestComp();

public:
	void SetInvenConsumeVector(std::vector<GameObject*> _InvenConsumeVector);

public:
	bool Init() override;
	void Update() override;
	void FinalUpdate() override;

public:
	QuestAlarmElixirNumberScript();
	~QuestAlarmElixirNumberScript();
};

