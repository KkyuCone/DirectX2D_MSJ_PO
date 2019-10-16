#include "Transform.h"
#include "GameObject.h"


void Transform::World_Scale(Vector4 _Value)
{
	// 월드 크기 계산
	if (nullptr != GetGameObject()->GetParent())
	{
		// 부모 오브젝트가 있을경우
		// 자식오브젝트는 부모의 크기에 영향을 받는다.
		// 고로 계산해야됨 ㅇㅅㅇ
		// 일단 X크기만 놓고 생각하자
		// 부모크기가 50, 자식 크기가 100이라고 하고싶다. (월드에서..)
		// 그런데 계산을 안해줄경우 자식크기는 50*100 = 5000이 된다..
		// 절레절레.. 그래서 계산을 해주는 것이다.
		// 자식이 월드에서 나타낼 값 / 부모의 크기르 ㄹ나눠주면 로컬크기는 완성된다.
		// 100 / 50 = 2
		// 자식의 로컬크기는 2이다.
		m_VLScale = _Value / GetGameObject()->GetTransform()->World_Scale();			// _Value / 부모 크기
		CalWScale();		// 계산하기 (월드에서 나올거..계산해줘야함)
	}
	else
	{
		// 부모가 없을 경우 로컬크기가 곧 월드크기이다.
		m_VLScale = _Value;
		m_VWScale = m_VLScale;
	}

	m_UpdateGeometry |= TUT_SCALE;
}


void Transform::CalWScale()
{
	// 상속받고 있는 오브젝트들의 크기를 부모의 크기에따라 변경해주는 함수 (계산)
	// 사용하는곳이 많아서..한번더 부모가 있는지 판단
	if (nullptr != GetGameObject()->GetParent())
	{
		// 부모 50 자식 100의 경우
		// 자식의 로컬이 2..
		// 2 * 50 = 100
		// 월드크기 완성!~
		m_VWScale = m_VLScale * GetGameObject()->GetParent()->GetTransform()->World_Scale();
	}
	else
	{
		// 부모가 없을 경우 로컬크기가 곧 월드크기이다.
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
	// 로컬 스페이스 -> 월드 스페이스로 옮기기 위한 행렬 만들기 'ㅅ'
	// 트랜스폼

	// & 양쪽 둘다 같아야 1..  결과 1-> true, 0은 false
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


	
	// 공전 행렬
	m_MatrixRevolveX.Rotate_X_Degree(m_VectorRevolve.x);
	m_MatrixRevolveY.Rotate_Y_Degree(m_VectorRevolve.y);
	m_MatrixRevolveZ.Rotate_Z_Degree(m_VectorRevolve.z);
	m_MatrixRevolve = m_MatrixRevolveX * m_MatrixRevolveY * m_MatrixRevolveZ;


	
	/*Matrix Parent;
	Parent.Position();*/

	// m_World : 로컬공간 버텍스를 월드 공간으로 옮기기 위한 행렬
	// m_MatrixWorld = m_MatrixScale * m_MatrixRotation * m_MatrixPosition *m_MatrixRevolve ;

	// 크자이공부
	if ( nullptr == GetGameObject()->GetParent() && true == m_bWorldUpdate )
	{
		// 부모가 없을 경우
		//m_MatrixWorld = m_MatrixScale * m_MatrixRotation * m_MatrixPosition *m_MatrixRevolve;
		m_MatrixWorld = m_MatrixScale * m_MatrixRotation * m_MatrixPosition;
	}
	else if (nullptr != GetGameObject()->GetParent() && nullptr != GetGameObject()->GetParent()->GetTransform())
	{
		// 부모가 있을 경우 부모행렬까지 곱해줌
		if (true == m_bWorldUpdate || true == GetGameObject()->GetParent()->GetTransform()->m_bWorldUpdate)
		{
			//m_MatrixWorld = m_MatrixScale * m_MatrixRotation * m_MatrixPosition *m_MatrixRevolve * GetGameObject()->GetParent()->GetTransform()->ConstRefWorldMatrix();
			m_MatrixWorld = m_MatrixScale * m_MatrixRotation * m_MatrixPosition * GetGameObject()->GetParent()->GetTransform()->ConstRefWorldMatrix();

			if (0 != GetGameObject()->m_ChildList.size())
			{
				// 자식이 한개라도 있다면
				m_bWorldUpdate = true;		// 이걸 안해주면.. 3번째 자식부터는 ㅇㅅㅇ;;;;; 월드 업데이트를 안해줌..
			}
		}
	}

	m_UpdateGeometry = 0;
}

void Transform::FinalUpdate()
{
	m_bWorldUpdate = false;
}