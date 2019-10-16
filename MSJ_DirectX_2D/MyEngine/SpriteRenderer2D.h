#pragma once
#include "Renderer.h"
#include "DXHeader.h"

class VertexShader;
class PixelShader;
class Mesh;
//class Texture;				// �ؽ�ó, ���÷� �Ѵ� ��������Ʈ���� ���ٰ���
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

	My_PTR<VertexShader> m_VertexShader;			// ���ؽ����̴�
	My_PTR<PixelShader> m_PixelShader;				// �ȼ����̴�
	My_PTR<Mesh> m_Mesh;							// �޽�
	//My_PTR<Texture> m_Texture;					// �ؽ�ó, ���÷� -> ��������Ʈ���� ����
	//My_PTR<Sampler> m_Sampler;

	size_t m_SpriteIndex;			// ��������Ʈ �ε���
	My_PTR<Sprite> m_Sprite;		// �� �ȿ� �ؽ�ó, ���÷�����


	My_PTR<Blend> m_Blend;
	//TestConstBuffer Temp;
	TestBuffer TestOutLine;
	SkillBuffer m_BlazeBuffer;
	//My_PTR<Sprite> m_Sprite;
	//int m_CurSpriteIndex;

	// �̰� TRUE�� �Ǹ� ������ ���;��� �κ��� ��� �ܻ��� �Ǹ� �ȴ�. ( �̰� ��������, )
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

