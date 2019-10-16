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
		CAMERA_PERSPECTIVE,				// ����
		CAMERA_ORTHOGRAPHIC,			// ����
	};

	enum ORTHGRAPHIC_SIZE_MODE
	{
		ORTHOGRAPHIC_WINDOW,			// ������ ��������
		ORTHOGRAPHIC_FREE,				// ��üȭ��
	};

private:
	// ī�޶�
	//Vector4 Camera_Position;		// ī�޶��� ��ġ (��� �����ִ���)
	//Vector4 Camera_Direction;		// ī�޶� ���� ( ��� ������ ������ )
	//Vector4 Camera_UpVector;		// Up����

	bool m_bMain;					// ���� ī�޶� ���� ī�޶����� �Ϲ�ī�޶����� �����ϱ� ���� ����
	CAMERA_MODE m_eCameraMode;
	ORTHGRAPHIC_SIZE_MODE m_eOrthographicSizeMode;
	Vector2 m_OrthographicSize;

	float m_CameraNear;
	float m_CameraFar;


	// ī�޶� �����ߵ� ��ɵ�
	Matrix m_View;				// �����
	Matrix m_Projection;		// ����
	Matrix m_ViewAndProj;		//  m_View * m_Projection

	// Matrix m_WorldViewAndProj;	// ���尪 (������)			-> ��� ī�޶� ���� �ٸ������� ���ٰž� 

	int m_Order;					// ī�޶� ���� 

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
	std::vector<int> m_RenderOrder;			// ������������ ( ������ ������ ������ )

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
		// �̰� �����ϱ� -> �ߺ��˻��ϴ°� �ߺ��Ǹ� �����ɷ�!
		// �����Ѱ�!
		// 	std::vector<int> m_RenderOrder;	
		//std::vector<int>::iterator RenderOrder_StartIter = m_RenderOrder.begin();
		//std::vector<int>::iterator RenderOrder_EndIter = m_RenderOrder.end();

		//for (; RenderOrder_StartIter != RenderOrder_EndIter; ++RenderOrder_StartIter)
		//{
		//	if (RenderOrder_StartIter == _RenderOrder)
		//	{
		//		// �ߺ��Ȱ� ������ ���Ұž� ������
		//		int a = 0;
		//		PushGroup(_Arg...);
		//		return;
		//	}
		//}

		//m_RenderOrder.push_back(_RenderOrder);
		PushOrder(_RenderOrder);			// �갡 �ߺ��˻絵 ���� ����
		PushGroup(_Arg...);
	}

	// ������ ���� ������ �̰ž��ϸ� ��� �ݺ���
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

