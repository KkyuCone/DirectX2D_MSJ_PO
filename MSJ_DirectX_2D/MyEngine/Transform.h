#pragma once
#include "Component.h"
#include "DXHeader.h"

#define TRANSFORMCHECK(NAME) Vector4 NAME; bool b##NAME;

enum TRANSFORMUPDATETYPE
{
	TUT_SCALE	= 0b00000000000000000000000000000001,	// 0x00000001
	TUT_ROT		= 0b00000000000000000000000000000010,	// 0x00000002
	TUT_POS		= 0b00000000000000000000000000000100,	// 0x00000004
};

class Transform : public Component
{
public:
	static bool IsMultiCheck(GameObject* _GameObject);
	friend class GameObject;

private:
	// Ʈ�������̶�� ���ϱ����� ���� ����� ����ϴ� ������Ʈ��...

	// ũ�Ⱑ �ٲ���� ����, ����Ʈ����������.. ���
	bool m_bWorldUpdate;
	int m_UpdateGeometry;

	// �⺻ Ʈ������
	// ����
	Vector4 m_VLScale;
	Vector4 m_VLRotation;
	Vector4 m_VLPosition;
	Vector4 m_VectorRevolve;

	// ���� ->  �θ��� ȸ����, ��ġ�� ���� �޾ƿ��� ���ؼ�.. 
	// ���� �޸𸮸� �� ���� ���� �޾ƿ÷��� ������ ����������� ����Ѵ�.
	// ���� �̰��� ���Ұ�� �θ��� ��ġ, ȸ��, ũ�Ⱚ�� �޾ƿͼ� �ڽĿ�����Ʈ�鿡�� �����Ϸ���
	// ���� ���������� (����������)
	Vector4 m_VWScale;
	Vector4 m_VWRotation;
	Vector4 m_VWPosition;


	// ��Ʈ����(���)
	Matrix m_MatrixScale;

	Matrix m_MatrixRotationX;
	Matrix m_MatrixRotationY;
	Matrix m_MatrixRotationZ;
	Matrix m_MatrixRotation;


	// ���� ���
	Matrix m_MatrixRevolveX;
	Matrix m_MatrixRevolveY;
	Matrix m_MatrixRevolveZ;

	Matrix m_MatrixRevolve;


	//

	Matrix m_MatrixPosition;


	Matrix m_MatrixWorld;		// ������ ( ���δ� ���Ѱ� )

private:
	void CalWScale();
	//void CalWRatation();
	//void CalWPosition();

public:
	// 2D�ϱ�.. X,Y�� �ʿ��ؼ� 2D ���� ��������
	// ����ϱ� ���ҷ���
	Vector2 Local_Position2D()	{ return m_VLPosition.vector2; }
	Vector2 Local_Scale2D()		{ return m_VLScale.vector2; }


	Vector2 World_Positon2D()
	{
		Vector4 aaa = m_MatrixWorld.WorldPosition();
		Vector2 returnValue = Vector2(aaa.x, aaa.y);
		return returnValue;
	}

	void Local_Scale2D(Vector2 _NewSize) { m_VLScale.vector2 = _NewSize; }

	//
	Vector4 Local_Scale() { return m_VLScale; }
	//Vector4 World_Scale() { return m_MatrixWorld.WorldScale(); }
	Vector4 World_Scale() { return m_VWScale; }

	Vector4 Local_Rotation() { return m_VLRotation; }
	Vector4 World_Rotation() { return m_VLRotation; }

	Vector4 Local_Position() { return m_VLPosition; }
	Vector4 World_Position() { return m_MatrixWorld.WorldPosition(); }

	Vector4 Local_Revolve() { return m_VLRotation; }
	

public:
	void Local_Scale(Vector4 _Value) 
	{ 
		m_VLScale = _Value; m_UpdateGeometry |= TUT_SCALE; 
		CalWScale();				// ���嵵������ֱ�
	}
	void World_Scale(Vector4 _Value);

	void Local_Rotation(Vector4 _Value) { m_VLRotation = _Value; m_UpdateGeometry |= TUT_ROT; }
	void World_Rotation(Vector4 _Value) { m_VLRotation = _Value; m_UpdateGeometry |= TUT_ROT; }

	void Local_Position(Vector4 _Value) { m_VLPosition = _Value; m_UpdateGeometry |= TUT_POS; }
	void World_Position(Vector4 _Value) { m_VLPosition = _Value; m_UpdateGeometry |= TUT_POS; }



	Vector4 Local_Forward()		{ return m_MatrixRotation.vec2; }
	Vector4 Local_Right()		{ return m_MatrixRotation.vec0; }
	Vector4 Local_Up()			{ return m_MatrixRotation.vec1; }

	Vector4 Local_Back()		{ return -m_MatrixRotation.vec2; }
	Vector4 Local_Left()		{ return -m_MatrixRotation.vec0; }
	Vector4 Local_Down()		{ return -m_MatrixRotation.vec1; }

	Vector4 World_Forward()		{ return m_MatrixRotation.vec2.RNormalize(); }
	Vector4 World_Right()		{ return m_MatrixRotation.vec0.RNormalize(); }
	Vector4 World_Up()			{ return m_MatrixRotation.vec1.RNormalize(); }

	Vector4 World_Back()		{ return -m_MatrixRotation.vec2.RNormalize(); }
	Vector4 World_Left()		{ return -m_MatrixRotation.vec0.RNormalize(); }
	Vector4 World_Down()		{ return -m_MatrixRotation.vec1.RNormalize(); }

	Matrix WorldMatrix() { return m_MatrixWorld; }
	const Matrix& ConstRefWorldMatrix()
	{
		return m_MatrixWorld;
	}

public:
	// ���� �̵�
	void Local_Move(const Vector4& _MoveValue) { m_VLPosition += _MoveValue; m_UpdateGeometry |= TUT_POS; }

	// ���� ȸ��
	void Local_Rotate(const Vector4& _RotateValue) { m_VLRotation += _RotateValue; m_UpdateGeometry |= TUT_ROT; }
	void Local_RotateX(const float _RotateValue) { m_VLRotation.x += _RotateValue; m_UpdateGeometry |= TUT_ROT; }
	void Local_RotateY(const float _RotateValue) { m_VLRotation.y += _RotateValue; m_UpdateGeometry |= TUT_ROT; }
	void Local_RotateZ(const float _RotateValue) { m_VLRotation.z += _RotateValue; m_UpdateGeometry |= TUT_ROT; }

	// ���� ����
	void Local_Revolve(const Vector4& _RevolveValue) { m_VectorRevolve += _RevolveValue; }


public:
	bool Init() override;			// Update�� virtual Init()����
	//void PrevUpdate();
	//void Update();
	//void LateUpdate();
	void EndUpdate();
	void FinalUpdate();

public:
	Transform();
	~Transform();
};

