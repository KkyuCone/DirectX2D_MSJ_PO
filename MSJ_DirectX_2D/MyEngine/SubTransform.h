#pragma once
#include "Component.h"
#include "DXHeader.h"

class SubTransform : public Component
{
private:
	class SubTransformData
	{
	public:
		Vector4 m_vPivot;
		Vector4 m_vScale;
		Vector4 m_vRotation;

		Matrix m_MScale;
		Matrix m_MRotationX;
		Matrix m_MRotationY;
		Matrix m_MRotationZ;
		Matrix m_MRotation;
		Matrix m_MPosition;
		Matrix m_MWorld;			// ���� 
	};

private:
	SubTransformData* m_SubData;

public:
	SubTransformData* GetSubTransform()
	{
		return m_SubData;
	}

public:

	// ���굥���Ͱ� �ִ��� ����
	bool IsSubData()
	{
		if (nullptr == m_SubData)
		{
			return false;
		}
		return true;
	}

public:
	// ��ġ(�Ǻ�) - Get
	Vector4 Local_SubPosition()
	{
		if (nullptr == m_SubData)
		{
			return Vector4::Zero;
		}
		return m_SubData->m_vPivot;
	}

	// ȸ�� - Get
	Vector4 Local_SubRotation()
	{
		if (nullptr == m_SubData)
		{
			return Vector4::Zero;
		}
		return m_SubData->m_vRotation;
	}

	// ũ�� Get
	Vector4 Local_SubScale()
	{
		if (nullptr == m_SubData)
		{
			return Vector4::Zero;
		}
		return m_SubData->m_vScale;
	}

	//
	// ��ġ(�Ǻ�) - Set
	void Local_SubPosition(const Vector4& _Value)
	{
		if (nullptr == m_SubData)
		{
			m_SubData = new SubTransformData();
		}
		m_SubData->m_vPivot = _Value;
	}

	// ȸ�� - Set
	void Local_SubRotation(const Vector4& _Value)
	{
		if (nullptr == m_SubData)
		{
			m_SubData = new SubTransformData();
		}
		m_SubData->m_vRotation = _Value;
	}

	// ũ�� Set
	void Local_SubScale(const Vector4& _Value)
	{
		if (nullptr == m_SubData)
		{
			m_SubData = new SubTransformData();
		}
		m_SubData->m_vScale = _Value;
	}

	const Matrix& SubConstRefWorldMatrix();

protected:
	void SubTransformUpdate();
public:
	SubTransform();
	~SubTransform();
};

