#pragma once
class UpdateBase
{
public:
	friend class GameObject;
	friend class Scene;


private:
	bool m_bUpdate;
	bool m_bDeath;

public:
	virtual bool IsUpdate()
	{
		return true == m_bUpdate && false == m_bDeath;
	}

	void Update_On()
	{
		m_bUpdate = true;
	}

	void Update_Off()
	{
		m_bUpdate = false;
	}

	virtual bool IsDeath()
	{
		return m_bDeath;
	}

	virtual void Death()
	{
		m_bDeath = true;
	}

public:
	virtual bool Init() { return true; }
	virtual void PrevUpdate() {}
	virtual void Update() {}
	virtual void LateUpdate() {}
	virtual void EndUpdate() {}
	virtual void FinalUpdate() {}

//#ifdef _DEBUG
	virtual void DebugRender() {}				// ����� ���� ( ��带 ���� �Ѱ�..�ؼ� ������ �ݶ��̴� �� ���̰� �Ұ��� ������)
//#endif // _DEBUG


public:
	UpdateBase() : m_bUpdate(true), m_bDeath(false)
	{

	}

	virtual ~UpdateBase() = 0 {}

};


