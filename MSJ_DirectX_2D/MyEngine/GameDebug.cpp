#include "GameDebug.h"
#include "Device.h"
#include "ResourceManager.h"
#include "GameFont.h"



GameDebug::GameDebug()
{
}


GameDebug::~GameDebug()
{
}

void GameDebug::DrawStr(const wchar_t* _String, float _fSize, Vector2 _Position, Vector4 _Color, FW1_TEXT_FLAG _Flag /*= FW1_TEXT_FLAG::FW1_TOP*/)
{
	My_PTR<GameFont> StringPTR = ResourceManager<GameFont>::Find(L"메이플스토리");
	StringPTR->Draw(_String, _fSize, _Position, _Color, _Flag);

	Device::Get_Context()->VSSetShader(nullptr, nullptr, 0);
	Device::Get_Context()->HSSetShader(nullptr, nullptr, 0);
	Device::Get_Context()->GSSetShader(nullptr, nullptr, 0);
	Device::Get_Context()->CSSetShader(nullptr, nullptr, 0);
	Device::Get_Context()->DSSetShader(nullptr, nullptr, 0);
	Device::Get_Context()->PSSetShader(nullptr, nullptr, 0);
	Device::Get_Context()->OMSetDepthStencilState(nullptr, 1);
}
