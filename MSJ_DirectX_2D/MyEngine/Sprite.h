#pragma once
#include "Resource.h"
#include <vector>
#include "DXHeader.h"

// 멀티스프라이트 작업

class Texture;
class Sampler;
class Sprite : public Resource
{
//public:
//	friend class SpriteRenderer2D;

private:
	My_PTR<Texture> m_Texture;
	My_PTR<Sampler> m_Sampler;
	std::vector<Vector4> m_UvMap;

public:
	Texture* GetTexture()
	{
		return m_Texture;
	}

public:
	const Vector4& IndexToUv(const int _Index)
	{
		return m_UvMap[_Index];
	}

public:
	size_t CurSize()
	{
		return m_UvMap.size();
	}

public:
	DEFINITION_CREATOR(Sprite, std::wstring)
		DEFINITION_CREATOR_FUCTION(const wchar_t* _TextureName, unsigned int _CountX = 1, unsigned int _CountY = 1, const wchar_t* _SamplerName = L"DefaultSampler")
		DEFINITION_CREATOR_FUNCTION_PARAMETER(_TextureName, _CountX, _CountY)
	DEFINITION_CREATOR_END

public:
	void Update();

public:
	bool Create(const wchar_t* _TextureName, unsigned int _CountX = 1, unsigned int _CountY = 1, const wchar_t* _SamplerName = L"DefaultSampler");
	size_t UvPush(Vector4 _vUv);
	Vector4 UvCal(size_t _Index);

public:
	Sprite();
	~Sprite();
};

