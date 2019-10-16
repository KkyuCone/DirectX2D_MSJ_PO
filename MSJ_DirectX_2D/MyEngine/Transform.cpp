#include "Transform.h"
#include "GameObject.h"


void Transform::World_Scale(Vector4 _Value)
{
	// ���� ũ�� ���
	if (nullptr != GetGameObject()->GetParent())
	{
		// �θ� ������Ʈ�� �������
		// �ڽĿ�����Ʈ�� �θ��� ũ�⿡ ������ �޴´�.
		// ��� ����ؾߵ� ������
		// �ϴ� Xũ�⸸ ���� ��������
		// �θ�ũ�Ⱑ 50, �ڽ� ũ�Ⱑ 100�̶�� �ϰ�ʹ�. (���忡��..)
		// �׷��� ����� �����ٰ�� �ڽ�ũ��� 50*100 = 5000�� �ȴ�..
		// ��������.. �׷��� ����� ���ִ� ���̴�.
		// �ڽ��� ���忡�� ��Ÿ�� �� / �θ��� ũ�⸣ �������ָ� ����ũ��� �ϼ��ȴ�.
		// 100 / 50 = 2
		// �ڽ��� ����ũ��� 2�̴�.
		m_VLScale = _Value / GetGameObject()->GetTransform()->World_Scale();			// _Value / �θ� ũ��
		CalWScale();		// ����ϱ� (���忡�� ���ð�..����������)
	}
	else
	{
		// �θ� ���� ��� ����ũ�Ⱑ �� ����ũ���̴�.
		m_VLScale = _Value;
		m_VWScale = m_VLScale;
	}

	m_UpdateGeometry |= TUT_SCALE;
}


void Transform::CalWScale()
{
	// ��ӹް� �ִ� ������Ʈ���� ũ�⸦ �θ��� ũ�⿡���� �������ִ� �Լ� (���)
	// ����ϴ°��� ���Ƽ�..�ѹ��� �θ� �ִ��� �Ǵ�
	if (nullptr != GetGameObject()->GetParent())
	{
		// �θ� 50 �ڽ� 100�� ���
		// �ڽ��� ������ 2..
		// 2 * 50 = 100
		// ����ũ�� �ϼ�!~
		m_VWScale = m_VLScale * GetGameObject()->GetParent()->GetTransform()->World_Scale();
	}
	else
	{
		// �θ� ���� ��� ����ũ�Ⱑ �� ����ũ���̴�.
		m_VWScale = m_VLScale;
	}
}

//void Transform::CalWRatation()
//{
//
//}
//
//void Transform::CalWPosition()
//{
//
//}


bool Transform::IsMultiCheck(GameObject* _GameObject)
{
	if (nullptr != _GameObject->GetComponent<Transform>())
	{
		return false;
	}

	return true;
}



Transform::Transform() : m_VLScale(Vector4::One), m_VLRotation(Vector4::Zero), m_VLPosition(Vector4::Zero)
{
}


Transform::~Transform()
{
}

//

bool Transform::Init()
{
	if (nullptr != GetGameObject()->GetTransform())
	{
		EAssert(true);
		return false;
	}

	GetGameObject()->SetTransform(this);
	return true;
}

void Transform::EndUpdate()
{
	// ���� �����̽� -> ���� �����̽��� �ű�� ���� ��� ����� '��'
	// Ʈ������

	// & ���� �Ѵ� ���ƾ� 1..  ��� 1-> true, 0�� false
	if (m_UpdateGeometry & TUT_SCALE)
	{
		m_MatrixScale.Scale(m_VLScale);
		m_bWorldUpdate = true;
	}

	if (m_UpdateGeometry & TUT_ROT)
	{
		m_MatrixRotationX.Rotate_X_Degree(m_VLRotation.x);
		m_MatrixRotationY.Rotate_Y_Degree(m_VLRotation.y);
		m_MatrixRotationZ.Rotate_Z_Degree(m_VLRotation.z);

		m_MatrixRotation = m_MatrixRotationX * m_MatrixRotationY * m_MatrixRotationZ;
		m_bWorldUpdate = true;
	}

	if (m_UpdateGeometry & TUT_POS)
	{
		m_MatrixPosition.Position(m_VLPosition);
		m_bWorldUpdate = true;
	}


	
	// ���� ���
	m_MatrixRevolveX.Rotate_X_Degree(m_VectorRevolve.x);
	m_MatrixRevolveY.Rotate_Y_Degree(m_VectorRevolve.y);
	m_MatrixRevolveZ.Rotate_Z_Degree(m_VectorRevolve.z);
	m_MatrixRevolve = m_MatrixRevolveX * m_MatrixRevolveY * m_MatrixRevolveZ;


	
	/*Matrix Parent;
	Parent.Position();*/

	// m_World : ���ð��� ���ؽ��� ���� �������� �ű�� ���� ���
	// m_MatrixWorld = m_MatrixScale * m_MatrixRotation * m_MatrixPosition *m_MatrixRevolve ;

	// ũ���̰���
	if ( nullptr == GetGameObject()->GetParent() && true == m_bWorldUpdate )
	{
		// �θ� ���� ���
		//m_MatrixWorld = m_MatrixScale * m_MatrixRotation * m_MatrixPosition *m_MatrixRevolve;
		m_MatrixWorld = m_MatrixScale * m_MatrixRotation * m_MatrixPosition;
	}
	else if (nullptr != GetGameObject()->GetParent() && nullptr != GetGameObject()->GetParent()->GetTransform())
	{
		// �θ� ���� ��� �θ���ı��� ������
		if (true == m_bWorldUpdate || true == GetGameObject()->GetParent()->GetTransform()->m_bWorldUpdate)
		{
			//m_MatrixWorld = m_MatrixScale * m_MatrixRotation * m_MatrixPosition *m_MatrixRevolve * GetGameObject()->GetParent()->GetTransform()->ConstRefWorldMatrix();
			m_MatrixWorld = m_MatrixScale * m_MatrixRotation * m_MatrixPosition * GetGameObject()->GetParent()->GetTransform()->ConstRefWorldMatrix();

			if (0 != GetGameObject()->m_ChildList.size())
			{
				// �ڽ��� �Ѱ��� �ִٸ�
				m_bWorldUpdate = true;		// �̰� �����ָ�.. 3��° �ڽĺ��ʹ� ������;;;;; ���� ������Ʈ�� ������..
			}
		}
	}

	m_UpdateGeometry = 0;
}

void Transform::FinalUpdate()
{
	m_bWorldUpdate = false;
}