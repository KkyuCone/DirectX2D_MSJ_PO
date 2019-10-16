#include "Camera.h"
#include "Window.h"
#include "Scene.h"
#include "Input.h"



Camera::Camera()
	: m_CameraNear(0.3f), m_CameraFar(1000.0f),
	m_eCameraMode(CAMERA_ORTHOGRAPHIC), m_eOrthographicSizeMode(ORTHOGRAPHIC_WINDOW),
	m_bMain(false)
{
}


Camera::~Camera()
{
	// ī�޶����
	// ���� ���� ī�޶��� nullptr�� ����������Ѵ�. 
	// (����ī�޶� ��������ϱ� �װ� ����Ű�°͵� ���������)
	if (true == m_bMain)
	{
		GetScene()->SetCamera(nullptr);
	}
}

//
bool Camera::Init(int _Order/* = 0*/) // override
{
	// ī�޶�� �ݵ�� Transfrom�� �ʿ��� ������Ʈ��. ���ٸ� ��Ʈ������~
	if (nullptr == GetTransform())
	{
		EAssert(true);
		return false;
	}

	if (nullptr == GetScene()->GetCamera())
	{
		// �ش� �� (�����)�� ����ī�޶� ���ٸ� ���� ī�޶� ����ī�޶�� ����.
		// ī�޶� ���ٸ� �Ⱥ���..
		GetScene()->SetCamera(this);
		m_bMain = true;
	}

	m_Order = _Order;

	GetScene()->m_RenderManager.Insert_Camera(this);

	return true;
}

void Camera::EndUpdate()
{
	if (nullptr == GetTransform())
	{
		EAssert(true);
		return;
	}

	// ī�޶� ���
	switch (m_eCameraMode)
	{
	case Camera::CAMERA_PERSPECTIVE:
		break;
	case Camera::CAMERA_ORTHOGRAPHIC:
		switch (m_eOrthographicSizeMode)
		{
		case Camera::ORTHOGRAPHIC_WINDOW:
			m_OrthographicSize.x = Window::FloatSize().x;
			m_OrthographicSize.y = Window::FloatSize().y;
			break;

		case Camera::ORTHOGRAPHIC_FREE:
			break;
		default:
			break;
		}
		// ViewLookToLH( �ٶ󺸴� ���� ��ġ (ī�޶���ġ)    ,   �ٶ󺸴� ����(����ȭ�Ⱥ���),    UP����(����ȣ����..) )
		m_View.ViewLookToLH(GetTransform()->World_Position(), GetTransform()->World_Forward(), GetTransform()->World_Up());
		m_Projection.Orthographic(m_OrthographicSize.x * ZoomInOut, m_OrthographicSize.y * ZoomInOut, m_CameraNear, m_CameraFar);
		break;
	default:
		break;
	}

	m_ViewAndProj = m_View * m_Projection;

}

void Camera::PushOrder(int _Order)
{
	for (size_t i = 0; i < m_RenderOrder.size(); ++i)
	{
		if (_Order == m_RenderOrder[i])
		{
			// �ߺ��Ǹ� ���� ������
			return;
		}
	}

	GetScene()->m_RenderManager.RenderOrder_Create(_Order);
	m_RenderOrder.push_back(_Order);
}

Vector2 Camera::WMousePosition()
{
	// ��ũ�� ��ǥ
	Vector2 ScreenMousePos = Input::MousePos_F();
	Vector2 ScreenSize = Window::FloatSize();

	// ���� ���� ��ǥ��� �ٲ��� '��'
	// ����غ��� �˾�~ ������ ī�޶� �̵��Ҷ��� ���� �־������ʾƼ�..
	// ���߿� �߰����ֱ� (���ϱ� �������ָ�� ������)
	return Vector2(ScreenMousePos.x - ScreenSize.HalfX(), ScreenSize.HalfY() - ScreenMousePos.y);

}

Vector2 Camera::WMosuePositionCamera()
{
	// ��ũ�� ��ǥ
	Vector2 ScreenMousePos = Input::MousePos_F();
	Vector2 ScreenSize = Window::FloatSize();

	// ���� ���� ��ǥ��� �ٲ��� '��'
	// ����غ��� �˾�~ ������ ī�޶� �̵��Ҷ��� ���� �־������ʾƼ�..
	// ���߿� �߰����ֱ� (���ϱ� �������ָ�� ������)
	return Vector2(ScreenMousePos.x - ScreenSize.HalfX() + GetTransform()->Local_Position().x, ScreenSize.HalfY() - ScreenMousePos.y + GetTransform()->Local_Position().y);
}
