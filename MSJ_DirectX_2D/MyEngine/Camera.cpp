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
	// 카메라관련
	// 내가 메인 카메라라면 nullptr로 설정해줘야한다. 
	// (메인카메라가 사라졌으니까 그걸 가리키는것도 없애줘야함)
	if (true == m_bMain)
	{
		GetScene()->SetCamera(nullptr);
	}
}

//
bool Camera::Init(int _Order/* = 0*/) // override
{
	// 카메라는 반드시 Transfrom이 필요한 오브젝트다. 없다면 터트려야지~
	if (nullptr == GetTransform())
	{
		EAssert(true);
		return false;
	}

	if (nullptr == GetScene()->GetCamera())
	{
		// 해당 씬 (현재씬)에 메인카메라가 없다면 현재 카메라를 메인카메라로 설정.
		// 카메라가 없다면 안보임..
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

	// 카메라 모드
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
		// ViewLookToLH( 바라보는 나의 위치 (카메라위치)    ,   바라보는 방향(정규화된벡터),    UP벡터(정규호ㅏ된..) )
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
			// 중복되면 안함 ㅇㅅㅇ
			return;
		}
	}

	GetScene()->m_RenderManager.RenderOrder_Create(_Order);
	m_RenderOrder.push_back(_Order);
}

Vector2 Camera::WMousePosition()
{
	// 스크린 좌표
	Vector2 ScreenMousePos = Input::MousePos_F();
	Vector2 ScreenSize = Window::FloatSize();

	// 이제 월드 좌표계로 바꾸자 'ㅅ'
	// 계산해보면 알아~ ㅇㅅㅇ 카메라 이동할때의 경우는 넣어주지않아서..
	// 나중에 추가해주기 (더하기 빼기해주면됨 ㅇㅅㅇ)
	return Vector2(ScreenMousePos.x - ScreenSize.HalfX(), ScreenSize.HalfY() - ScreenMousePos.y);

}

Vector2 Camera::WMosuePositionCamera()
{
	// 스크린 좌표
	Vector2 ScreenMousePos = Input::MousePos_F();
	Vector2 ScreenSize = Window::FloatSize();

	// 이제 월드 좌표계로 바꾸자 'ㅅ'
	// 계산해보면 알아~ ㅇㅅㅇ 카메라 이동할때의 경우는 넣어주지않아서..
	// 나중에 추가해주기 (더하기 빼기해주면됨 ㅇㅅㅇ)
	return Vector2(ScreenMousePos.x - ScreenSize.HalfX() + GetTransform()->Local_Position().x, ScreenSize.HalfY() - ScreenMousePos.y + GetTransform()->Local_Position().y);
}
