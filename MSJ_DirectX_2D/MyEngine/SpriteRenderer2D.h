#pragma once
#include "Renderer.h"
#include "DXHeader.h"

class VertexShader;
class PixelShader;
class Mesh;
//class Texture;				// 텍스처, 샘플러 둘다 스프라이트에서 해줄거임
//class Sampler;
class Blend;
class Sprite;
class SpriteRenderer2D : public Renderer
{
public:
	friend GameObject;
private:
	int InputValue = 0;

private:

	My_PTR<VertexShader> m_VertexShader;			// 버텍스쉐이더
	My_PTR<PixelShader> m_PixelShader;				// 픽셀쉐이더
	My_PTR<Mesh> m_Mesh;							// 메쉬
	//My_PTR<Texture> m_Texture;					// 텍스처, 샘플러 -> 스프라이트에서 해줌
	//My_PTR<Sampler> m_Sampler;

	size_t m_SpriteIndex;			// 스프라이트 인덱스
	My_PTR<Sprite> m_Sprite;		// 얘 안에 텍스처, 샘플러있음


	My_PTR<Blend> m_Blend;
	//TestConstBuffer Temp;
	TestBuffer TestOutLine;
	SkillBuffer m_BlazeBuffer;
	//My_PTR<Sprite> m_Sprite;
	//int m_CurSpriteIndex;

	// 이게 TRUE가 되면 색깔이 나와야할 부분이 모두 단색이 되면 된다. ( 이거 숙제였음, )
	//bool IsAllColor;
	//VCOLOR AllColor;

public:
	Sprite* GetSprite()
	{
		return m_Sprite;
	}

	SkillBuffer* GetSkillBuffer()
	{
		return &m_BlazeBuffer;
	}


private:
	bool Init(int _Order = 0);

public:
	void SetSprite(const wchar_t* _SpriteName, size_t _Index = 0);
	void Index(size_t _Index);

private:
	void Render(const Matrix& _ViewProjection) override;

public:
	void ChangeMesh(const wchar_t* _MeshKey);

public:
	SpriteRenderer2D();
	~SpriteRenderer2D();
};

