#pragma once
#include <math.h>

#include <d3d11.h>					// 다이렉트11 리소스 및 자료형 헤더
#include <d3dcompiler.h>			// 쉐이더 컴파일용 헤더
#include <DirectXPackedVector.h>	// 다이렉트용 simd 방식을 사용하는 연산을 위한 벡터용 헤더


// 일반적인 코드 연산이 아니라 16바이트 정렬을 이용해서 모든 클래스가 16바이트 정렬을 해줘야함.
// -> 16바이트 전용 레지스터를 이용해서 벡터나 행렬의 연산을 이용해서..

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")

#include "EnMACRO.h"

const float PI = 3.14159265359f;
const float DegreeToRadian = 3.14159265359f / 180.0f;
const float RadianToDegree = 180.0f / 3.14159265359f;

class Vector2
{
public:
	union
	{
		float p_float[2];

		struct
		{
			float x;
			float y;
		};

		int p_int[2];
		struct
		{
			int ix;
			int iy;
		};

		__int64 i64;
	};

public:
	// 절대값으로 바꾸기
	void FABS()
	{
		x = fabsf(x);
		y = fabsf(y);
	}

	Vector2 ConvertToFloat()
	{
		return{ (float)ix, (float)iy };
	}

	Vector2 ConvertToInt()
	{
		return{ (int)x, (int)y };
	}

	float HalfX()
	{
		return x * 0.5f;
	}

	float HalfY()
	{
		return y * 0.5f;
	}


public:
	Vector2() : x(0), y(0) {}
	Vector2(float _x, float _y) : x(_x), y(_y) {}
	Vector2(int _x, int _y) : ix(_x), iy(_y) {}
};

class Vector3
{
	union
	{
		Vector2 vector2;
		float p_float[3];
		struct
		{
			float x;
			float y;
			float z;
		};

		int p_int[3];
		struct
		{
			float ix;
			float iy;
			float iz;
		};
	};

	float HalfX()
	{
		return x * 0.5f;
	}

	float HalfY()
	{
		return y * 0.5f;
	}

	float HalfZ()
	{
		return z * 0.5f;
	}

public:
	Vector3() :p_float{0.f,} {}
	Vector3(const float& _X , const float& _Y, const float& _Z) 
		:x(_X),y(_Y),z(_Z)
	{}
};

// 벡터에 대한 이해
// 1. 벡터는 위치를 표현하기 위한 것이 절대로 아님
// 2. 원점을 기준으로 함

class Vector4
{
public:
	typedef Vector4 Vector4_Int;
	typedef Vector4 VCOLOR;

	static const Vector4 Zero;
	static const Vector4 One;
	static const Vector4 White;
	static const Vector4 Black;
	static const Vector4 Red;
	static const Vector4 Green;
	static const Vector4 Blue;
	static const Vector4 NONECOLOR;
	static Vector4 NonePos;


	// 현재는 직교투영이라 Z축이 없으므로 back, forward가 없음
	// 이동 방향
	static const Vector4 Left;
	static const Vector4 Right;
	static const Vector4 Up;
	static const Vector4 Down;

	// 회전 방향
	static const Vector4 AxisX;
	static const Vector4 AxisY;
	static const Vector4 AxisZ;


public:
	union
	{
		//DirectX::XMVECTOR m_XMVector;
		DirectX::XMFLOAT4 m_Vector;
		struct
		{
			float x;
			float y;
			float z;
			float w;		// 4차원을 나타내기 위한 값이 아니다.
		};

		struct
		{
			// 색상관련
			float r;
			float g;
			float b;
			float a;
		};

		float p_float[4];

		struct
		{
			int ix;
			int iy;
			int iz;
			int iw;		// 4차원 나타내기 위한거 아님
		};
		int p_int[4];

		struct
		{
			Vector2 Pos;
			Vector2 Size;
		};

		Vector2 vector2;
		Vector3 vector3;

	};

public:
	// 사이즈 절반
	float FSizeHalfX()
	{
		return Size.x * 0.5f;
	}

	float FSizeHalfY()
	{
		return Size.y * 0.5f;
	}

	// 각 요소들의 절반값 (x,y,z,w)
	float FHalfX()
	{
		return x * 0.5f;
	}

	float FHalfY()
	{
		return y * 0.5f;
	}

	float FHalfZ()
	{
		return z * 0.5f;
	}

	float FHalfW()
	{
		return w * 0.5f;
	}

	// 위치의 절반값
	float FLeft()
	{
		return Pos.x - FSizeHalfX();
	}

	float FRight()
	{
		return Pos.x + FSizeHalfX();
	}

	float FTop()
	{
		return Pos.y + FSizeHalfY();
	}

	float FBottom()
	{
		return Pos.y - FSizeHalfY();
	}

	// 각 모서리 부분 ( 꼭지점..? 여튼 그런 부분 )
	Vector2 FLeftTop()
	{
		return Vector2(FLeft(), FTop());
	}

	Vector2 FRightTop()
	{
		return Vector2(FRight(), FTop());
	}

	Vector2 FLeftBottom()
	{
		return Vector2(FLeft(), FBottom());
	}

	Vector2 FRightBottom()
	{
		return Vector2(FRight(), FBottom());
	}

public:
	void Convert_Int()
	{
		ix = (int)x;
		iy = (int)y;
		iz = (int)z;
		iw = (int)w;
	}

	Vector4_Int ReturnConvert_Int()
	{
		return Vector4((int)x, (int)y, (int)z, (int)w);
	}

	// 형변환 
	operator DirectX::FXMVECTOR() const
	{
		return DirectX::XMLoadFloat4(&m_Vector);
	}

	Vector4& operator +=(const Vector4& _Other)
	{
		x += _Other.x;
		y += _Other.y;
		z += _Other.z;
		//w += _Other.w;			//ㅣ필요없음

		return *this;
	}

	Vector4 operator +(const Vector4& _Other)
	{
		Vector4 ReturnValue = *this;
		ReturnValue.x += _Other.x;
		ReturnValue.y += _Other.y;
		ReturnValue.z += _Other.z;
		ReturnValue.w += _Other.w;
		return ReturnValue;
	}

	Vector4 operator -(const Vector4& _Other)
	{
		Vector4 ReturnValue = *this;
		ReturnValue.x -= _Other.x;
		ReturnValue.y -= _Other.y;
		ReturnValue.z -= _Other.z;
		//ReturnValue.w += _Other.w;
		return ReturnValue;
	}

	// 해당 벡터 음수로 바꾸기 ㅇㅅㅇ
	Vector4 operator -() const
	{
		Vector4 ReturnValue = *this;
		ReturnValue.x = -ReturnValue.x;
		ReturnValue.y = -ReturnValue.y;
		ReturnValue.z = -ReturnValue.z;
		ReturnValue.w = -ReturnValue.w;
		return ReturnValue;
	}

	Vector4& operator *=(const float& _Value)
	{
		x *= _Value;
		y *= _Value;
		z *= _Value;
		//w *= _Value;

		return *this;
	}

	Vector4 operator *(const float _Value) const
	{
		Vector4 ReturnValue = *this;
		ReturnValue *= _Value;
		return ReturnValue;
	}

	// 벡터와 벡터의 곱하기는 방향에도 영향을 준다.
	Vector4& operator *=(const Vector4& _Other)
	{
		x *= _Other.x;
		y *= _Other.y;
		z *= _Other.z;
		w *= _Other.w;

		return *this;
	}

	Vector4 operator *(const Vector4& _Value) const
	{
		Vector4 ReturnValue = *this;
		ReturnValue.x *= _Value.x;
		ReturnValue.y *= _Value.y;
		ReturnValue.z *= _Value.z;
		ReturnValue.w *= _Value.w;

		return ReturnValue;
	}



	Vector4& operator =(const Vector4& _Other)
	{
		m_Vector = _Other.m_Vector;

		return *this;
	}

	Vector4& operator =(const DirectX::XMVECTOR& _Other)
	{
		DirectX::XMStoreFloat4(&m_Vector, _Other);
		return *this;
	}


	Vector4 operator /(const float _Value) const
	{
		Vector4 ReturnValue = *this;
		ReturnValue.x /= _Value;
		ReturnValue.y /= _Value;
		ReturnValue.z /= _Value;
		ReturnValue.w /= _Value;
		return ReturnValue;
	}

	Vector4 operator /(const Vector4& _Value) const
	{
		Vector4 ReturnValue = *this;
		ReturnValue.x /= _Value.x;
		ReturnValue.y /= _Value.y;
		ReturnValue.z /= _Value.z;
		ReturnValue.w /= _Value.w;
		return ReturnValue;
	}

	Vector4& operator /=(const float _Value)
	{
		x /= _Value;
		y /= _Value;
		z /= _Value;
		w /= _Value;
		return *this;
	}

	bool operator !=(const Vector4& _Value)
	{
		if ((int)x == (int)_Value.x &&
			(int)y == (int)_Value.y &&
			(int)z == (int)_Value.z &&
			(int)a == (int)_Value.a)
		{
			return true;
		}
		return false;
	}



///////////////////////// 벡터연산
	// 놈 (길이) 구하기
	float Norm()
	{
		/*return sqrtf((x*x) + (y*y) + (z*z));*/
		return DirectX::XMVector3Length(*this).m128_f32[0];
	}


	// 정규화
	void Normalize()
	{
		/*return *this / Norm();*/
		*this = DirectX::XMVector4Normalize(*this);
	}

	// 정규화(리턴값존재)
	Vector4 RNormalize()
	{
		/*return *this / Norm();*/
		Vector4 NewNormalize = *this;
		NewNormalize.Normalize();
		return NewNormalize;
	}

	// 외적
	// 외적은 교환법칙이 성립하지 않는다. 그리고 외적은 2차원에서도 가능하다. 단지 Z값만 나올뿐
	Vector4 Cross3D(const Vector4& _Other)
	{
		Vector4 Left = *this;
		Vector4 Right = _Other;

		return DirectX::XMVector3Cross(Left, _Other);
	}

	void RadToDir2D(float _Angle)
	{
		// 이거 만드는중..
		x = cosf(_Angle);
		y = sinf(_Angle);

		if (x == 0)
		{

		}

		if (y == 0)
		{

		}
	}

	void DegToDir2D(float _Angle)
	{
		RadToDir2D(_Angle * DegreeToRadian);
	}

	// 외적의 결과로.. 정방향으로 나왔는지 반대방향인지 아니면 수평관계인지 알수 있다.
	int IsDir2D(const Vector4& _Other)
	{
		// 외적의 결과값이 0이라면 2차원의 경우에는 두 벡터가 수평관계에 있다.
		if (0 == Cross2D(_Other))
		{
			return 0;
		}

		// 부호가 양수인경우는 정방향으로 움직인다.
		if (0 < Cross2D(_Other))
		{
			return 1;
		}

		// 부호가 음수로 나올경우 -방향으로 움직인다.(반대방향)
		if (0 > Cross2D(_Other))
		{
			return -1;
		}

		return 3;
	}

	// 2D 외적 = Z값만 나온다. (방향을 가진 벡터가아닌 오직 Z값(-1, 0, 1)만 존재)
	float Cross2D(const Vector4& _Other)
	{
		Vector4 Left = *this;
		Vector4 Right = _Other;

		//DirectX::XMVECTOR CrossVector2D = DirectX::XMVector2Cross(Left, Right);

		// 왜 첫번째로 인자로 반환하는가?
		// 2차원의 두벡터를 외적하면 오직 Z값 하나만 결과로 나오게되는데
		// 이 Z값을 4개 원소 모두 다 넣어주기때문에 하나만 리턴해주면된다.
		// => 0~3번째든 하나만 넣어주면됨
		return DirectX::XMVector2Cross(Left, Right).m128_f32[0];
	}

	float Angle3DRad(const Vector4& _Other)
	{
#pragma region	XMVector3AngleBetweenVectors 과정

		//Vector4 Left = *this;
		//Vector4 Right = _Other;

		//// 정규화단계
		//Left.Normalize();
		//Right.Normalize();

		//// 세타값을 구하는 과정이기에
		//// 내적을 할경우 |a||b|cos세타에서
		//// |a|와 |b|의 경우 둘다 1이기때문에
		//// cos세타만 남는다.
		//// 세타값만 얻어오기 위해서 아크 코사인을 곱해준다.
		//acosf(DirectX::XMVector3Dot(Left, Right).m128_f32[0]);

#pragma endregion

		/*DirectX::XMVECTOR Temp = DirectX::XMVector3AngleBetweenVectors(*this, _Other);*/
		return  DirectX::XMVector3AngleBetweenVectors(*this, _Other).m128_f32[0];
	}

	float Angle3D(const Vector4& _Other)
	{
		return Angle3DRad(_Other) * RadianToDegree;
	}

	// 벡터 내적 (2D)
	float Dot2D(const Vector4& _Other)
	{
		return DirectX::XMVector3Dot(*this, _Other).m128_f32[0];
	}

	// 벡터 내적 (3D)
	float Dot3D(const Vector4& _Other)
	{
		// |a||b|cos세타
		return DirectX::XMVector3Dot(*this, _Other).m128_f32[0];
	}



public:
	Vector4() {}
	Vector4(DirectX::XMVECTOR& _Value) { DirectX::XMStoreFloat4(&m_Vector, _Value); }  // 16바이트로 사용되는 애를 
	Vector4(float _x, float _y) : x(_x), y(_y), z(0.0f), w(1.0f) {}
	Vector4(float _x, float _y, float _z) : x(_x), y(_y), z(_z), w(1.0f) {}
	Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

	Vector4(int _x, int _y, int _z) : ix(_x), iy(_y), iz(_z), iw(1) {}
	Vector4(int _x, int _y, int _z, int _w) : ix(_x), iy(_y), iz(_z), iw(_w) {}
	Vector4(const Vector4& _Other) : m_Vector(_Other.m_Vector) {}

};

typedef Vector4::Vector4_Int Vector4_Int;
typedef Vector4::VCOLOR VCOLOR;
typedef Vector4 Rect;

typedef Vector4 VINT;

typedef Vector2 Vector2_Int;
typedef Vector3 Vector3_Int;

// 행렬이 뭐고, 왜 사용하나
// 4*4 행렬에는 버텍스가 변환해야할 수치를 다 혼합할수가 있다.

// 크자이공부 ( 월드 최종행렬 )
// 크기 -> 자전 -> 이동 -> 공전 -> 부모

// 행렬을 사용하는 이유는
// 버텍스가 변환해야할 크기, 이동, 회전..

class Matrix
{
public:
	union
	{
		DirectX::XMFLOAT4X4 m_XFMatrix;		// 자료형이락 생각하면됨 float (4바이트) * 4개로 사용 ( 얜 16바이트로 정렬이 안되어있음 )
		//DirectX::XMMATRIX m_XMMatrix;   // XMMATRIX->16바이트로 정렬되어있음 

		float matrix[4][4];
		struct
		{
			Vector4 vec0;
			Vector4 vec1;
			Vector4 vec2;
			Vector4 vec3;

		};
	};

public:
	operator DirectX::FXMMATRIX()
	{
		return DirectX::XMLoadFloat4x4(&m_XFMatrix);
	}


	// 중심축 (기저벡터때문에..)
	// 방향 (forward, up, right)알려고 
	enum AXIS_TYPE
	{
		AXIS_X = 0,
		AXIS_Y = 1,
		AXIS_Z = 2,
	};

	// 월드포스
	Vector4 WorldPosition()
	{
		return Vector4{ matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]};
	}

	// 월드 크기
	Vector4 WorldScale()
	{
		return Vector4{ matrix[0][0], matrix[1][1], matrix[2][2] };
	}

	// 월드 회전
	Vector4 WorldRotation()
	{
		return Vector4{ matrix[0][0], matrix[1][1], matrix[2][2] };
	}

	// 열벡터뽑아내기( 해당 기저벡터를 알아내기 위해 행렬에서 열벡터를 뽑아내야한다. )
	Vector4 ColumnVector(AXIS_TYPE _Type)
	{
		switch (_Type)
		{
		case Matrix::AXIS_X:
		case Matrix::AXIS_Y:
		case Matrix::AXIS_Z:
			return Vector4{ matrix[0][_Type], matrix[1][_Type], matrix[2][_Type] };
		default:
			return Vector4::Zero;
		}

		return Vector4::Zero;
	}

public:
	// 모든 행렬값이 0 ..
	void Zero()
	{
		memset(matrix, 0, sizeof(Matrix));
	}

	// 항등행렬(단위행렬)
	void Identity()
	{
		//matrix[0][0] = 1.0f;
		//matrix[1][1] = 1.0f;
		//matrix[2][2] = 1.0f;
		//matrix[3][3] = 1.0f;

		//m_XMMatrix = DirectX::XMMatrixIdentity();
		// XMStoreFloat4x4 16바이트 사용안할거예요~ ( 16바이트로 정렬되어있는데 그거를 4바이트씩 나눠서 쓰는걸로 바꿔줌 'ㅅ' )
		DirectX::XMStoreFloat4x4(&m_XFMatrix, DirectX::XMMatrixIdentity());

	}

	void Scale(const Vector4& _Value)
	{
		//Zero();
		//matrix[0][0] = _Value.x;
		//matrix[1][1] = _Value.y;
		//matrix[2][2] = _Value.z;
		//matrix[3][3] = 1.0f;

		//m_XMMatrix = DirectX::XMMatrixScaling(_Value.x, _Value.y, _Value.z);
		DirectX::XMStoreFloat4x4(&m_XFMatrix, DirectX::XMMatrixScaling(_Value.x, _Value.y, _Value.z));
	}

	// X, Y, Z 축 회전
	// X축
	void Rotate_X_Degree(float _Degree)
	{
		//m_XMMatrix = DirectX::XMMatrixRotationX(_Degree * DegreeToRadian);
		DirectX::XMStoreFloat4x4(&m_XFMatrix, DirectX::XMMatrixRotationX(_Degree * DegreeToRadian));
	}

	void Rotate_X_Radian(float _Radian)
	{
		//m_XMMatrix = DirectX::XMMatrixRotationX(_Radian);
		DirectX::XMStoreFloat4x4(&m_XFMatrix, DirectX::XMMatrixRotationX(_Radian));
	}

	// Y축
	void Rotate_Y_Degree(float _Degree)
	{
		//m_XMMatrix = DirectX::XMMatrixRotationY(_Degree * DegreeToRadian);
		DirectX::XMStoreFloat4x4(&m_XFMatrix, DirectX::XMMatrixRotationY(_Degree * DegreeToRadian));
	}

	void Rotate_Y_Radian(float _Radian)
	{
		//m_XMMatrix = DirectX::XMMatrixRotationY(_Radian);
		DirectX::XMStoreFloat4x4(&m_XFMatrix, DirectX::XMMatrixRotationY(_Radian));
	}

	// Z축
	void Rotate_Z_Degree(float _Degree)
	{
		//m_XMMatrix = DirectX::XMMatrixRotationZ(_Degree * DegreeToRadian);
		DirectX::XMStoreFloat4x4(&m_XFMatrix, DirectX::XMMatrixRotationZ(_Degree * DegreeToRadian));
	}

	void Rotate_Z_Radian(float _Radian)
	{
		//m_XMMatrix = DirectX::XMMatrixRotationZ(_Radian);
		DirectX::XMStoreFloat4x4(&m_XFMatrix, DirectX::XMMatrixRotationZ(_Radian));
	}


	// Position
	void Position(const Vector4& _Value)
	{
		//Identity();
		//matrix[3][0] = _Value.x;
		//matrix[3][1] = _Value.y;
		//matrix[3][2] = _Value.z;
		//matrix[3][3] = 1.0f;
		//m_XMMatrix = DirectX::XMMatrixTranslation(_Value.x, _Value.y, _Value.z);
		DirectX::XMStoreFloat4x4( &m_XFMatrix, DirectX::XMMatrixTranslation(_Value.x, _Value.y, _Value.z));
	}

	// View 
	void ViewLookAtLH(const Vector4& _EyePosition, 
					  const Vector4& _FocusPosition, 
					  const Vector4& _UpDirection)
	{
		// LH -> 왼손 좌표계, RH -> 오른손 좌표계
		// 바라보는 위치,          바라보는  대상의 위치,    UP벡터(0,1,0)
		// ex ) 100, 100, 100   /    100, 100, 101       / 0, 1, 0
		//m_XMMatrix = DirectX::XMMatrixLookAtLH(_EyePosition.m_XMVector, _FocusPosition.m_XMVector, _UpDirection.m_XMVector);
		DirectX::XMStoreFloat4x4(&m_XFMatrix, DirectX::XMMatrixLookAtLH(_EyePosition, _FocusPosition, _UpDirection));
	}

	void ViewLookToLH(const Vector4& _EyePosition,
					  const Vector4& _EyeDirection,
					  const Vector4& _UpDirection)
	{
		// 바라보는 나의 위치 (카메라위치)    ,   바라보는 방향(정규화된벡터),    UP벡터
		//m_XMMatrix = DirectX::XMMatrixLookToLH(_EyePosition.m_XMVector, _EyeDirection.m_XMVector, _UpDirection.m_XMVector);
		DirectX::XMStoreFloat4x4(&m_XFMatrix, DirectX::XMMatrixLookToLH(_EyePosition, _EyeDirection, _UpDirection));
	}

	// Transpos
	void Transpos()
	{
		// 역행렬로.. (Camera Space)  ->  카메라 스페이스는 월드스페이스 행렬의 역행렬이 필요하다.
		// 내가 생각하는거는
		// 거울에 비친 나를 보면 내가 오른쪽으로 가면 거울 속의 나는 왼쪽으로 이동한다..
		// 이렇게 생각해서 역행렬로 표현하는것 같다고 생각함 ㅇㅅㅇ`
		//m_XMMatrix = DirectX::XMMatrixTranspose(m_XMMatrix);

		float Temp = 0.0f;
		for (size_t x = 0; x < 4; x++)
		{
			for (size_t y = x + 1; y < 4; y++)
			{
				Temp = matrix[x][y];
				matrix[x][y] = matrix[y][x];
				matrix[y][x] = Temp;
			}
		}


		//float Temp = 0.0f;
		//for (size_t x = 0; x < 4; ++x)
		//{
		//	for (size_t y = 0; y < 4; ++y)
		//	{
		//		if (x == y) {
		//			break;
		//		}

		//		Temp = matrix[x][y];
		//		matrix[x][y] = matrix[y][x];
		//		matrix[y][x] = Temp;
		//	}
		//}
	}

	Matrix Get_Transpos()
	{
		//return DirectX::XMMatrixTranspose(m_XMMatrix);
		Matrix NewMatirx = *this;
		NewMatirx.Transpos();
		return NewMatirx;
	}

	// Field of View (직교투영, 원근투영)

	// 원근
	void PerspectiveFov_Radian(float _Fov, float _Width, float _Height, float _Near, float _Far)
	{
		//m_XMMatrix = DirectX::XMMatrixPerspectiveFovLH(_Fov, _Width / _Height, _Near, _Far);
		DirectX::XMStoreFloat4x4(&m_XFMatrix, DirectX::XMMatrixPerspectiveFovLH(_Fov, _Width / _Height, _Near, _Far));
	}

	void PerspectiveFov_Degree(float _Fov, float _Width, float _Height, float _Near, float _Far)
	{
		//m_XMMatrix = DirectX::XMMatrixPerspectiveFovLH(_Fov * DegreeToRadian, _Width / _Height, _Near, _Far);
		DirectX::XMStoreFloat4x4(&m_XFMatrix, DirectX::XMMatrixPerspectiveFovLH(_Fov * DegreeToRadian, _Width / _Height, _Near, _Far));
	}

	void Perspective(float _Width, float _Height, float _Near, float _Far)
	{
		//m_XMMatrix = DirectX::XMMatrixPerspectiveLH(_Width, _Height, _Near, _Far);
		DirectX::XMStoreFloat4x4(&m_XFMatrix, DirectX::XMMatrixPerspectiveLH(_Width, _Height, _Near, _Far));
	}

	// 직교
	void Orthographic(float _Width, float _Height, float _Near, float _Far)
	{
		//m_XMMatrix = DirectX::XMMatrixOrthographicLH(_Width, _Height, _Near, _Far);
		DirectX::XMStoreFloat4x4(&m_XFMatrix, DirectX::XMMatrixOrthographicLH(_Width, _Height, _Near, _Far));
	}


	// 
	Vector4 Vector_Multiply_One(const Vector4& _Value)
	{
		//Vector4 ReturnVector;

		// 1.0f, 0.0f, 0.0f, 0.0f;
		// 0.0f, 1.0f, 0.0f, 0.0f;
		// 0.0f,   0.0f,1.0f, 0.0f;
		// 300.0f,   300.0f, 0.0f,   1.0f;

		// 			ReturnVec.x = 
		//    0.5f * 1.0f
		//	+ 0.5f * 0.0f
		//	+ 0.5f * 0.0f
		//	+ 0.0f * 300.0f;


		/*for (size_t i = 0; i < 4; i++)
		{
			ReturnVector.p_float[i] =
				_Value.p_float[0] * matrix[0][i]
				+ _Value.p_float[1] * matrix[1][i]
				+ _Value.p_float[2] * matrix[2][i]
				+ 1.0f * matrix[3][i];

		}

		return ReturnVector;*/

		return DirectX::XMVector3TransformCoord(_Value, *this);
	}

	Vector4 Vector_Multiply_Zero(const Vector4& _Value)
	{
		Vector4 ReturnVector;

		for (size_t i = 0; i < 4; i++)
		{
			ReturnVector.p_float[i] =
				_Value.p_float[0] * matrix[0][i]
				+ _Value.p_float[1] * matrix[1][i]
				+ _Value.p_float[2] * matrix[2][i]
				+ 0.0f * matrix[3][i];
		}

		return ReturnVector;
	}

	Vector4 Vector_Multiply(const Vector4& _Value)
	{
		Vector4 ReturnVector;

		for (size_t i = 0; i < 4; i++)
		{
			ReturnVector.p_float[i] =
				_Value.p_float[0] * matrix[0][i]
				+ _Value.p_float[1] * matrix[1][i]
				+ _Value.p_float[2] * matrix[2][i]
				+ _Value.p_float[3] * matrix[3][i];
		}

		return ReturnVector;
	}

	Matrix operator* (const Matrix& _Other) const
	{
		Matrix ReturnMatrix;

		for (size_t y = 0; y < 4; y++)
		{
			for (size_t x = 0; x < 4; x++)
			{
				ReturnMatrix.matrix[y][x] =
					matrix[y][0] * _Other.matrix[0][x]
					+ matrix[y][1] * _Other.matrix[1][x]
					+ matrix[y][2] * _Other.matrix[2][x]
					+ matrix[y][3] * _Other.matrix[3][x];
			}
		}

		return ReturnMatrix;
	}

	Matrix& operator= (const Matrix& _Other)
	{
		m_XFMatrix = _Other.m_XFMatrix;

		return *this;
	}

public:
	Matrix() : matrix{ 0 }
	{
		Identity();
	}

	Matrix(const Matrix& _Other) : m_XFMatrix(_Other.m_XFMatrix) {}
	//Matrix(const DirectX::XMMATRIX& _Other) : m_XMMatrix(_Other) {}
	Matrix(const DirectX::XMMATRIX& _Other) { DirectX::XMStoreFloat4x4(&m_XFMatrix, _Other); }
};


//class TestConstBuffer
//{
//public:
//	Vector3			ColorKey;
//	UINT			Check;
//
//
//	TestConstBuffer():ColorKey(Vector3(0.f,0.f,0.f)), Check(0){}
//};


class TestBuffer
{
public:
	Vector4 Position;
	Vector4 OutLineColor;
	Vector4 OutLineTF;

public:
	TestBuffer() : Position(Vector4(0.0f, 0.0f, 0.0f)), OutLineColor(Vector4(0.0f, 0.0f, 0.0f)), OutLineTF(Vector4(0.0f, 0.0f, 0.0f))
	{

	}
};

class SkillBuffer
{
public:
	Vector4 BlazeValue;
	Vector4 PoisonValue;
	Vector4 CataclysmValue;

public:
	SkillBuffer() : BlazeValue(0.0f, 0.0f, 0.0f, 0.0f), PoisonValue(0.0f, 0.0f, 0.0f, 1.0f), CataclysmValue(0.0f, 0.0f, 0.0f, 1.0f)
	{

	}
};
