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
	// 朝五虞淫恵
	// 鎧亜 五昔 朝五虞虞檎 nullptr稽 竺舛背操醤廃陥. 
	// (五昔朝五虞亜 紫虞然生艦猿 益杏 亜軒徹澗依亀 蒸蕉操醤敗)
	if (true == m_bMain)
	{
		GetScene()->SetCamera(nullptr);
	}
}

//
bool Camera::Init(int _Order/* = 0*/) // override
{
	// 朝五虞澗 鋼球獣 Transfrom戚 琶推廃 神崎詮闘陥. 蒸陥檎 斗闘形醤走~
	if (nullptr == GetTransform())
	{
		EAssert(true);
		return false;
	}

	if (nullptr == GetScene()->GetCamera())
	{
		// 背雁 樟 (薄仙樟)拭 五昔朝五虞亜 蒸陥檎 薄仙 朝五虞研 五昔朝五虞稽 竺舛.
		// 朝五虞亜 蒸陥檎 照左績..
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

	// 朝五虞 乞球
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
		// ViewLookToLH( 郊虞左澗 蟹税 是帖 (朝五虞是帖)    ,   郊虞左澗 号狽(舛鋭鉢吉困斗),    UP困斗(舛鋭硲た吉..) )
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
			// 掻差鞠檎 照敗 しさし
			return;
		}
	}

	GetScene()->m_RenderManager.RenderOrder_Create(_Order);
	m_RenderOrder.push_back(_Order);
}

Vector2 Camera::WMousePosition()
{
	// 什滴鍵 疎妊
	Vector2 ScreenMousePos = Input::MousePos_F();
	Vector2 ScreenSize = Window::FloatSize();

	// 戚薦 杉球 疎妊域稽 郊荷切 'さ'
	// 域至背左檎 硝焼~ しさし 朝五虞 戚疑拝凶税 井酔澗 隔嬢爽走省焼辞..
	// 蟹掻拭 蓄亜背爽奄 (希馬奄 皐奄背爽檎喫 しさし)
	return Vector2(ScreenMousePos.x - ScreenSize.HalfX(), ScreenSize.HalfY() - ScreenMousePos.y);

}

Vector2 Camera::WMosuePositionCamera()
{
	// 什滴鍵 疎妊
	Vector2 ScreenMousePos = Input::MousePos_F();
	Vector2 ScreenSize = Window::FloatSize();

	// 戚薦 杉球 疎妊域稽 郊荷切 'さ'
	// 域至背左檎 硝焼~ しさし 朝五虞 戚疑拝凶税 井酔澗 隔嬢爽走省焼辞..
	// 蟹掻拭 蓄亜背爽奄 (希馬奄 皐奄背爽檎喫 しさし)
	return Vector2(ScreenMousePos.x - ScreenSize.HalfX() + GetTransform()->Local_Position().x, ScreenSize.HalfY() - ScreenMousePos.y + GetTransform()->Local_Position().y);
}
