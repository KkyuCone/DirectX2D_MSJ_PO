#include "GameFont.h"
#include "Device.h"



GameFont::GameFont() : m_pFontFactory(nullptr), m_pFontWrapper(nullptr)
{
}


GameFont::~GameFont()
{
	if (nullptr != m_pFontFactory)
	{
		m_pFontFactory->Release();
	}

	if (nullptr != m_pFontWrapper)
	{
		m_pFontWrapper->Release();
	}
}

bool GameFont::Create()
{
	// 팩토리 생성 ㅇㅅㅇ ;;근데 무조건 생성해줌..;;ㅇㅅㅇ;;;
	if (S_OK != FW1CreateFactory(FW1_VERSION, &m_pFontFactory))
	{
		EAssert(true);
		return false;
	}

	// 얘도. .ㅇㅅㅇ;;;;
	if (S_OK != m_pFontFactory->CreateFontWrapper(Device::Get_Device(), GetPath().c_str(), &m_pFontWrapper))
	{
		EAssert(true);
		return false;
	}

	return true;
}

void GameFont::Draw(const wchar_t* _String, float _fSize, Vector2 _Position, Vector4 _Color, FW1_TEXT_FLAG _Flag /*= FW1_TEXT_FLAG::FW1_TOP*/)
{
	if (nullptr == m_pFontWrapper)
	{
		EAssert(true);
		return;
	}

	// 색상이...순서가 ABGR임 ㅇㅅㅇ;;
	UINT32 Color = 0xff00ff00;			// 알파 1, 블루 0, 그린 0, 레드 0

	m_pFontWrapper->DrawString(Device::Get_Context(), _String, _fSize, _Position.x, _Position.y, Color, _Flag);
}
