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
		Matrix m_MWorld;			// 월드 
	};

private:
	SubTransformData* m_SubData;

public:
	SubTransformData* GetSubTransform()
	{
		return m_SubData;
	}

public:

	// 서브데이터가 있는지 여부
	bool IsSubData()
	{
		if (nullptr == m_SubData)
		{
			return false;
		}
		return true;
	}

public:
	// 위치(피봇) - Get
	Vector4 Local_SubPosition()
	{
		if (nullptr == m_SubData)
		{
			return Vector4::Zero;
		}
		return m_SubData->m_vPivot;
	}

	// 회전 - Get
	Vector4 Local_SubRotation()
	{
		if (nullptr == m_SubData)
		{
			return Vector4::Zero;
		}
		return m_SubData->m_vRotation;
	}

	// 크기 Get
	Vector4 Local_SubScale()
	{
		if (nullptr == m_SubData)
		{
			return Vector4::Zero;
		}
		return m_SubData->m_vScale;
	}

	//
	// 위치(피봇) - Set
	void Local_SubPosition(const Vector4& _Value)
	{
		if (nullptr == m_SubData)
		{
			m_SubData = new SubTransformData();
		}
		m_SubData->m_vPivot = _Value;
	}

	// 회전 - Set
	void Local_SubRotation(const Vector4& _Value)
	{
		if (nullptr == m_SubData)
		{
			m_SubData = new SubTransformData();
		}
		m_SubData->m_vRotation = _Value;
	}

	// 크기 Set
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

