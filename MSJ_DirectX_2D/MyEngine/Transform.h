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
	// 트랜스폼이라는 기하구조에 대한 기능을 담당하는 컴포넌트들...

	// 크기가 바뀌는지 여부, 서브트랜스때문에.. 사용
	bool m_bWorldUpdate;
	int m_UpdateGeometry;

	// 기본 트랜스폼
	// 로컬
	Vector4 m_VLScale;
	Vector4 m_VLRotation;
	Vector4 m_VLPosition;
	Vector4 m_VectorRevolve;

	// 월드 ->  부모의 회전값, 위치값 등을 받아오기 위해서.. 
	// 현재 메모리를 더 쓰고 쉽게 받아올려고 다음의 멤버변수들을 사용한다.
	// 만약 이것을 안할경우 부모의 위치, 회전, 크기값을 받아와서 자식오브젝트들에게 적용하려면
	// 직접 계산해줘야함 (수학적으로)
	Vector4 m_VWScale;
	Vector4 m_VWRotation;
	Vector4 m_VWPosition;


	// 매트릭스(행렬)
	Matrix m_MatrixScale;

	Matrix m_MatrixRotationX;
	Matrix m_MatrixRotationY;
	Matrix m_MatrixRotationZ;
	Matrix m_MatrixRotation;


	// 공전 행렬
	Matrix m_MatrixRevolveX;
	Matrix m_MatrixRevolveY;
	Matrix m_MatrixRevolveZ;

	Matrix m_MatrixRevolve;


	//

	Matrix m_MatrixPosition;


	Matrix m_MatrixWorld;		// 마지막 ( 전부다 곱한값 )

private:
	void CalWScale();
	//void CalWRatation();
	//void CalWPosition();

public:
	// 2D니까.. X,Y만 필요해서 2D 따로 만들어놓음
	// 사용하기 편할려고
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
		CalWScale();				// 월드도계산해주기
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
	// 로컬 이동
	void Local_Move(const Vector4& _MoveValue) { m_VLPosition += _MoveValue; m_UpdateGeometry |= TUT_POS; }

	// 로컬 회전
	void Local_Rotate(const Vector4& _RotateValue) { m_VLRotation += _RotateValue; m_UpdateGeometry |= TUT_ROT; }
	void Local_RotateX(const float _RotateValue) { m_VLRotation.x += _RotateValue; m_UpdateGeometry |= TUT_ROT; }
	void Local_RotateY(const float _RotateValue) { m_VLRotation.y += _RotateValue; m_UpdateGeometry |= TUT_ROT; }
	void Local_RotateZ(const float _RotateValue) { m_VLRotation.z += _RotateValue; m_UpdateGeometry |= TUT_ROT; }

	// 로컬 공전
	void Local_Revolve(const Vector4& _RevolveValue) { m_VectorRevolve += _RevolveValue; }


public:
	bool Init() override;			// Update에 virtual Init()있음
	//void PrevUpdate();
	//void Update();
	//void LateUpdate();
	void EndUpdate();
	void FinalUpdate();

public:
	Transform();
	~Transform();
};

