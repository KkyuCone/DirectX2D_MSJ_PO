#pragma once
#include "DXHeader.h"

class GameMath
{
public:
	static bool FRectToPoint(Vector4 _Left, Vector4 _Right) { return false; }
	static bool FRectToRect(Vector4 _Left, Vector4 _Right) 
	{ 
		// 아래는 값을 확인하려고 잠깐 쓸려는거..
		float RL = _Right.FLeft();
		float RR = _Right.FRight();
		float RT = _Right.FTop();
		float RB = _Right.FBottom();

		float LL = _Left.FLeft();
		float LR = _Left.FRight();
		float LT = _Left.FTop();
		float LB = _Left.FBottom();

		// 사각형 충돌체크 하기
		if (_Left.FLeft() >= _Right.FRight())
		{
			return false;
		}

		if (_Left.FRight() <= _Right.FLeft())
		{
			return false;
		}

		if (_Left.FTop() <= _Right.FBottom())
		{
			return false;
		}

		if (_Left.FBottom() >= _Right.FTop())
		{
			return false;
		}

		return true;
	}

	static bool FRectToCircle(Vector4 _Left, Vector4 _Right) { return false; }

	static bool FPointToPoint(Vector2 _Left, Vector2 _Right) { return false; }
	//static bool FPointToRect(Vector2 _Left, Vector2 _Right) { return false; }
	//static bool FPointToCircle(Vector2 _Left, Vector2 _Right) { return false; }

	//static bool FCircleToPoint(Vector4 _Left, Vector4 _Right) { return false; }
	//static bool FCircleToRect(Vector4 _Left, Vector4 _Right) { return false; }
	static bool FCircleToCircle(Vector4 _Left, Vector4 _Right) { return false; }
private:
	GameMath();
	~GameMath();
};

