#include "VertexHeader.h"

const Vector4 Vector4::Zero = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
const Vector4 Vector4::One = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
const Vector4 Vector4::White = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
const Vector4 Vector4::Black = Vector4(.0f, .0f, .0f, 1.0f);
const Vector4 Vector4::Red = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
const Vector4 Vector4::Green = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
const Vector4 Vector4::Blue = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
const Vector4 Vector4::NONECOLOR = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
// �߰�
Vector4 Vector4::NonePos = Vector4(-1.0f, -1.0f, -1.0f, -1.0f);


// �̵�����
const Vector4 Vector4::Left = Vector4(-1.0f, 0.0f, 0.0f, 1.0f);
const Vector4 Vector4::Right = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
const Vector4 Vector4::Up = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
const Vector4 Vector4::Down = Vector4(0.0f, -1.0f, 0.0f, 1.0f);

// ȸ������
const Vector4 Vector4::AxisX = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
const Vector4 Vector4::AxisY = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
const Vector4 Vector4::AxisZ = Vector4(0.0f, 0.0f, 1.0f, 1.0f);