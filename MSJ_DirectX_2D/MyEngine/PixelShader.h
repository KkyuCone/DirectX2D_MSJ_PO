#pragma once
#include "Shader.h"

class PixelShader : public Shader
{
private:
	ID3D11PixelShader* m_pPixelShader;

	DEFINITION_LOADER(PixelShader, std::wstring)
		DEFINITION_KEY_LOAD_FUNCTION(UINT _VersionFirst, UINT _VersionSecond, const char* _FunctionName)
		DEFINITION_LOADER_FUNTION_PARAMETER(_VersionFirst, _VersionSecond, _FunctionName)
		DEFINITION_LOADER_END


public:
	bool Load(UINT _VersionFirst, UINT _VersionSecond, const char* _FunctionName);
	void Update() override;


	template<typename T>
	void UpdateConstBuffer(const wchar_t* _Name, const T& _Value)
	{
		My_PTR<ConstBuffer> Find_CB = Find_ConstBuffer(_Name);

		if (nullptr == Find_CB)
		{
			EAssert(true);
			return;
		}

		if (false == Find_CB->ClassType_Check<T>())
		{
			EAssert(true);
			return;
		}

		Find_CB->Update<T>(_Value);
		Find_CB->Pixel_Set();
		return;
	}

public:
	PixelShader();
	~PixelShader();
};

