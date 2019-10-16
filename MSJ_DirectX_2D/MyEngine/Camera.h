#pragma once
#include "Component.h"
#include "Transform.h"
#include <vector>
#include "Time.h"

class Camera : public Component
{
public:
	friend class GameObject;
	friend class RenderManager;

private:
	enum CAMERA_MODE
	{
		CAMERA_PERSPECTIVE,				// 원근
		CAMERA_ORTHOGRAPHIC,			// 직교
	};

	enum ORTHGRAPHIC_SIZE_MODE
	{
		ORTHOGRAPHIC_WINDOW,			// 윈도우 사이즈모드
		ORTHOGRAPHIC_FREE,				// 전체화면
	};

private:
	// 카메라
	//Vector4 Camera_Position;		// 카메라의 위치 (어디서 보고있는지)
	//Vector4 Camera_Direction;		// 카메라 방향 ( 어디 방향을 보는지 )
	//Vector4 Camera_UpVector;		// Up벡터

	bool m_bMain;					// 현재 카메라가 메인 카메라인지 일반카메라인지 구분하기 위한 변수
	CAMERA_MODE m_eCameraMode;
	ORTHGRAPHIC_SIZE_MODE m_eOrthographicSizeMode;
	Vector2 m_OrthographicSize;

	float m_CameraNear;
	float m_CameraFar;


	// 카메라가 가져야될 기능들
	Matrix m_View;				// 뷰행렬
	Matrix m_Projection;		// 투영
	Matrix m_ViewAndProj;		//  m_View * m_Projection

	// Matrix m_WorldViewAndProj;	// 월드값 (최종값)			-> 얘는 카메라 말고 다른곳에서 해줄거얌 

	int m_Order;					// 카메라 순서 

private:
	float ZoomInOut = 1.0f;

public:
	void ZoomIn()
	{
		ZoomInOut += -2.0f * Time::DeltaTime();
		if (ZoomInOut <= 1.0f)
		{
			ZoomInOut = 1.0f;
			return;
		}
	}

	void ZoomOut()
	{
		ZoomInOut += 2.0f * Time::DeltaTime();
	}


private:
	std::vector<int> m_RenderOrder;			// 렌더러순서들 ( 렌더러 순서가 들어가있음 )

public:
	template<typename ...Rest>
	void SettingViewGroup(Rest ..._Arg)
	{
		m_RenderOrder.reserve(sizeof...(_Arg));
		PushGroup(_Arg...);
	}

private:
	template<typename ...Rest>
	void PushGroup(int _RenderOrder, Rest ..._Arg)
	{
		// 이건 내가하기 -> 중복검사하는거 중복되면 다음걸로!
		// 내가한거!
		// 	std::vector<int> m_RenderOrder;	
		//std::vector<int>::iterator RenderOrder_StartIter = m_RenderOrder.begin();
		//std::vector<int>::iterator RenderOrder_EndIter = m_RenderOrder.end();

		//for (; RenderOrder_StartIter != RenderOrder_EndIter; ++RenderOrder_StartIter)
		//{
		//	if (RenderOrder_StartIter == _RenderOrder)
		//	{
		//		// 중복된거 있으면 안할거얌 ㅇㅅㅇ
		//		int a = 0;
		//		PushGroup(_Arg...);
		//		return;
		//	}
		//}

		//m_RenderOrder.push_back(_RenderOrder);
		PushOrder(_RenderOrder);			// 얘가 중복검사도 같이 해줌
		PushGroup(_Arg...);
	}

	// 마지막 순서 넣을때 이거안하면 계속 반복됨
	void PushGroup(int _RenderOrder)
	{
		//m_RenderOrder.push_back(_RenderOrder);
		PushOrder(_RenderOrder);
	}

public:
	void PushOrder(int _Order);

	Vector2 WMousePosition();
	Vector2 WMosuePositionCamera();


private:
	bool Init(int _Order = 0);
	void EndUpdate() override;


public:
	const Matrix GetViewProj() { return m_ViewAndProj; }


public:
	Camera();
	~Camera();
};

