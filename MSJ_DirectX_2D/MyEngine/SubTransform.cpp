#include "SubTransform.h"
#include "Transform.h"


SubTransform::SubTransform() : m_SubData(nullptr)
{
}


SubTransform::~SubTransform()
{
	if (nullptr != m_SubData)
	{
		delete m_SubData;
	}
}

const Matrix& SubTransform::SubConstRefWorldMatrix()
{
	if (nullptr == m_SubData)
	{
		// ���� Ʈ�������� ������� ������ �θ������ ����(���� ���)
		return GetTransform()->ConstRefWorldMatrix();
	}

	return m_SubData->m_MWorld;
}


void SubTransform::SubTransformUpdate()
{
	// ���� Ʈ������ �������������  return + Transform�� ������� ������ return
	if (nullptr == m_SubData || nullptr == GetTransform())
	{
		return;
	}

	m_SubData->m_MScale.Scale(m_SubData->m_vScale);
	m_SubData->m_MRotationX.Rotate_X_Degree(m_SubData->m_vRotation.x);
	m_SubData->m_MRotationY.Rotate_Y_Degree(m_SubData->m_vRotation.y);
	m_SubData->m_MRotationZ.Rotate_Z_Degree(m_SubData->m_vRotation.z);

	m_SubData->m_MRotation = m_SubData->m_MRotationX * m_SubData->m_MRotationY * m_SubData->m_MRotationZ;
	m_SubData->m_MPosition.Position(m_SubData->m_vPivot + GetTransform()->World_Position());
	m_SubData->m_MWorld = m_SubData->m_MScale * m_SubData->m_MRotation * m_SubData->m_MPosition/** m_MRevolve * m_MParent*/;
}