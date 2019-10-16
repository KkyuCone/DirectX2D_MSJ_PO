#pragma once
#include "Resource.h"
#include "DXHeader.h"
#include <FW1CompileSettings.h>
#include <FW1FontWrapper.h>

class GameFont : public Resource
{
private:
	IFW1Factory* m_pFontFactory;
	IFW1FontWrapper* m_pFontWrapper;

public:
	DEFINITION_CREATOR(GameFont, std::wstring)
		DEFINITION_CREATOR_FUCTION()
		DEFINITION_CREATOR_FUNCTION_PARAMETER()
	DEFINITION_CREATOR_END

public:
	bool Create();
	void Draw(const wchar_t* _String, float _fSize, Vector2 _Position, Vector4 _Color, FW1_TEXT_FLAG _Flag = FW1_TEXT_FLAG::FW1_TOP);
public:
	GameFont();
	~GameFont();
};

