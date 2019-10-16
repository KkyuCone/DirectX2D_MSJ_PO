#pragma once
#include <math.h>

#include <d3d11.h>					// ���̷�Ʈ11 ���ҽ� �� �ڷ��� ���
#include <d3dcompiler.h>			// ���̴� �����Ͽ� ���
#include <DirectXPackedVector.h>	// ���̷�Ʈ�� simd ����� ����ϴ� ������ ���� ���Ϳ� ���


// �Ϲ����� �ڵ� ������ �ƴ϶� 16����Ʈ ������ �̿��ؼ� ��� Ŭ������ 16����Ʈ ������ �������.
// -> 16����Ʈ ���� �������͸� �̿��ؼ� ���ͳ� ����� ������ �̿��ؼ�..

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
	// ���밪���� �ٲٱ�
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

// ���Ϳ� ���� ����
// 1. ���ʹ� ��ġ�� ǥ���ϱ� ���� ���� ����� �ƴ�
// 2. ������ �������� ��

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


	// ����� ���������̶� Z���� �����Ƿ� back, forward�� ����
	// �̵� ����
	static const Vector4 Left;
	static const Vector4 Right;
	static const Vector4 Up;
	static const Vector4 Down;

	// ȸ�� ����
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
			float w;		// 4������ ��Ÿ���� ���� ���� �ƴϴ�.
		};

		struct
		{
			// �������
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
			int iw;		// 4���� ��Ÿ���� ���Ѱ� �ƴ�
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
	// ������ ����
	float FSizeHalfX()
	{
		return Size.x * 0.5f;
	}

	float FSizeHalfY()
	{
		return Size.y * 0.5f;
	}

	// �� ��ҵ��� ���ݰ� (x,y,z,w)
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

	// ��ġ�� ���ݰ�
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

	// �� �𼭸� �κ� ( ������..? ��ư �׷� �κ� )
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

	// ����ȯ 
	operator DirectX::FXMVECTOR() const
	{
		return DirectX::XMLoadFloat4(&m_Vector);
	}

	Vector4& operator +=(const Vector4& _Other)
	{
		x += _Other.x;
		y += _Other.y;
		z += _Other.z;
		//w += _Other.w;			//���ʿ����

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

	// �ش� ���� ������ �ٲٱ� ������
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

	// ���Ϳ� ������ ���ϱ�� ���⿡�� ������ �ش�.
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



///////////////////////// ���Ϳ���
	// �� (����) ���ϱ�
	float Norm()
	{
		/*return sqrtf((x*x) + (y*y) + (z*z));*/
		return DirectX::XMVector3Length(*this).m128_f32[0];
	}


	// ����ȭ
	void Normalize()
	{
		/*return *this / Norm();*/
		*this = DirectX::XMVector4Normalize(*this);
	}

	// ����ȭ(���ϰ�����)
	Vector4 RNormalize()
	{
		/*return *this / Norm();*/
		Vector4 NewNormalize = *this;
		NewNormalize.Normalize();
		return NewNormalize;
	}

	// ����
	// ������ ��ȯ��Ģ�� �������� �ʴ´�. �׸��� ������ 2���������� �����ϴ�. ���� Z���� ���û�
	Vector4 Cross3D(const Vector4& _Other)
	{
		Vector4 Left = *this;
		Vector4 Right = _Other;

		return DirectX::XMVector3Cross(Left, _Other);
	}

	void RadToDir2D(float _Angle)
	{
		// �̰� �������..
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

	// ������ �����.. ���������� ���Դ��� �ݴ�������� �ƴϸ� ����������� �˼� �ִ�.
	int IsDir2D(const Vector4& _Other)
	{
		// ������ ������� 0�̶�� 2������ ��쿡�� �� ���Ͱ� ������迡 �ִ�.
		if (0 == Cross2D(_Other))
		{
			return 0;
		}

		// ��ȣ�� ����ΰ��� ���������� �����δ�.
		if (0 < Cross2D(_Other))
		{
			return 1;
		}

		// ��ȣ�� ������ ���ð�� -�������� �����δ�.(�ݴ����)
		if (0 > Cross2D(_Other))
		{
			return -1;
		}

		return 3;
	}

	// 2D ���� = Z���� ���´�. (������ ���� ���Ͱ��ƴ� ���� Z��(-1, 0, 1)�� ����)
	float Cross2D(const Vector4& _Other)
	{
		Vector4 Left = *this;
		Vector4 Right = _Other;

		//DirectX::XMVECTOR CrossVector2D = DirectX::XMVector2Cross(Left, Right);

		// �� ù��°�� ���ڷ� ��ȯ�ϴ°�?
		// 2������ �κ��͸� �����ϸ� ���� Z�� �ϳ��� ����� �����ԵǴµ�
		// �� Z���� 4�� ���� ��� �� �־��ֱ⶧���� �ϳ��� �������ָ�ȴ�.
		// => 0~3��°�� �ϳ��� �־��ָ��
		return DirectX::XMVector2Cross(Left, Right).m128_f32[0];
	}

	float Angle3DRad(const Vector4& _Other)
	{
#pragma region	XMVector3AngleBetweenVectors ����

		//Vector4 Left = *this;
		//Vector4 Right = _Other;

		//// ����ȭ�ܰ�
		//Left.Normalize();
		//Right.Normalize();

		//// ��Ÿ���� ���ϴ� �����̱⿡
		//// ������ �Ұ�� |a||b|cos��Ÿ����
		//// |a|�� |b|�� ��� �Ѵ� 1�̱⶧����
		//// cos��Ÿ�� ���´�.
		//// ��Ÿ���� ������ ���ؼ� ��ũ �ڻ����� �����ش�.
		//acosf(DirectX::XMVector3Dot(Left, Right).m128_f32[0]);

#pragma endregion

		/*DirectX::XMVECTOR Temp = DirectX::XMVector3AngleBetweenVectors(*this, _Other);*/
		return  DirectX::XMVector3AngleBetweenVectors(*this, _Other).m128_f32[0];
	}

	float Angle3D(const Vector4& _Other)
	{
		return Angle3DRad(_Other) * RadianToDegree;
	}

	// ���� ���� (2D)
	float Dot2D(const Vector4& _Other)
	{
		return DirectX::XMVector3Dot(*this, _Other).m128_f32[0];
	}

	// ���� ���� (3D)
	float Dot3D(const Vector4& _Other)
	{
		// |a||b|cos��Ÿ
		return DirectX::XMVector3Dot(*this, _Other).m128_f32[0];
	}



public:
	Vector4() {}
	Vector4(DirectX::XMVECTOR& _Value) { DirectX::XMStoreFloat4(&m_Vector, _Value); }  // 16����Ʈ�� ���Ǵ� �ָ� 
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

// ����� ����, �� ����ϳ�
// 4*4 ��Ŀ��� ���ؽ��� ��ȯ�ؾ��� ��ġ�� �� ȥ���Ҽ��� �ִ�.

// ũ���̰��� ( ���� ������� )
// ũ�� -> ���� -> �̵� -> ���� -> �θ�

// ����� ����ϴ� ������
// ���ؽ��� ��ȯ�ؾ��� ũ��, �̵�, ȸ��..

class Matrix
{
public:
	union
	{
		DirectX::XMFLOAT4X4 m_XFMatrix;		// �ڷ����̶� �����ϸ�� float (4����Ʈ) * 4���� ��� ( �� 16����Ʈ�� ������ �ȵǾ����� )
		//DirectX::XMMATRIX m_XMMatrix;   // XMMATRIX->16����Ʈ�� ���ĵǾ����� 

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


	// �߽��� (�������Ͷ�����..)
	// ���� (forward, up, right)�˷��� 
	enum AXIS_TYPE
	{
		AXIS_X = 0,
		AXIS_Y = 1,
		AXIS_Z = 2,
	};

	// ��������
	Vector4 WorldPosition()
	{
		return Vector4{ matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]};
	}

	// ���� ũ��
	Vector4 WorldScale()
	{
		return Vector4{ matrix[0][0], matrix[1][1], matrix[2][2] };
	}

	// ���� ȸ��
	Vector4 WorldRotation()
	{
		return Vector4{ matrix[0][0], matrix[1][1], matrix[2][2] };
	}

	// �����ͻ̾Ƴ���( �ش� �������͸� �˾Ƴ��� ���� ��Ŀ��� �����͸� �̾Ƴ����Ѵ�. )
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
	// ��� ��İ��� 0 ..
	void Zero()
	{
		memset(matrix, 0, sizeof(Matrix));
	}

	// �׵����(�������)
	void Identity()
	{
		//matrix[0][0] = 1.0f;
		//matrix[1][1] = 1.0f;
		//matrix[2][2] = 1.0f;
		//matrix[3][3] = 1.0f;

		//m_XMMatrix = DirectX::XMMatrixIdentity();
		// XMStoreFloat4x4 16����Ʈ �����Ұſ���~ ( 16����Ʈ�� ���ĵǾ��ִµ� �װŸ� 4����Ʈ�� ������ ���°ɷ� �ٲ��� '��' )
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

	// X, Y, Z �� ȸ��
	// X��
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

	// Y��
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

	// Z��
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
		// LH -> �޼� ��ǥ��, RH -> ������ ��ǥ��
		// �ٶ󺸴� ��ġ,          �ٶ󺸴�  ����� ��ġ,    UP����(0,1,0)
		// ex ) 100, 100, 100   /    100, 100, 101       / 0, 1, 0
		//m_XMMatrix = DirectX::XMMatrixLookAtLH(_EyePosition.m_XMVector, _FocusPosition.m_XMVector, _UpDirection.m_XMVector);
		DirectX::XMStoreFloat4x4(&m_XFMatrix, DirectX::XMMatrixLookAtLH(_EyePosition, _FocusPosition, _UpDirection));
	}

	void ViewLookToLH(const Vector4& _EyePosition,
					  const Vector4& _EyeDirection,
					  const Vector4& _UpDirection)
	{
		// �ٶ󺸴� ���� ��ġ (ī�޶���ġ)    ,   �ٶ󺸴� ����(����ȭ�Ⱥ���),    UP����
		//m_XMMatrix = DirectX::XMMatrixLookToLH(_EyePosition.m_XMVector, _EyeDirection.m_XMVector, _UpDirection.m_XMVector);
		DirectX::XMStoreFloat4x4(&m_XFMatrix, DirectX::XMMatrixLookToLH(_EyePosition, _EyeDirection, _UpDirection));
	}

	// Transpos
	void Transpos()
	{
		// ����ķ�.. (Camera Space)  ->  ī�޶� �����̽��� ���彺���̽� ����� ������� �ʿ��ϴ�.
		// ���� �����ϴ°Ŵ�
		// �ſ￡ ��ģ ���� ���� ���� ���������� ���� �ſ� ���� ���� �������� �̵��Ѵ�..
		// �̷��� �����ؼ� ����ķ� ǥ���ϴ°� ���ٰ� ������ ������`
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

	// Field of View (��������, ��������)

	// ����
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

	// ����
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
