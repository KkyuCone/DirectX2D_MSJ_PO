#include "SpriteRenderer2D.h"

#include "ResourceManager.h"
#include "Transform.h"

#include "VertexShader.h"
#include "PixelShader.h"
#include "Mesh.h"
//#include "Texture.h"
//#include "Sampler.h"
#include "Blend.h"
#include "Sprite.h"

#include "Input.h"			// �����Ϸ��� �߰�����.��..���ϸ� �����ϱ�


SpriteRenderer2D::SpriteRenderer2D() : m_SpriteIndex(0)
{
}


SpriteRenderer2D::~SpriteRenderer2D()
{
}

//

bool SpriteRenderer2D::Init(int _Order /*= 0*/)
{
	int a = 0;
	Renderer::Init(_Order);

	m_VertexShader = ResourceManager<VertexShader>::Find(L"TEXTUREVERTEXSHADER");
	m_PixelShader = ResourceManager<PixelShader>::Find(L"TEXTUREPIXELSHADER");
	m_Mesh = ResourceManager<Mesh>::Find(L"TextureMesh");

	//m_ColVertexShader = ResourceManager<VertexShader>::Find(L"COLORUVVERTEXSHADER");
	//m_ColPixelShader = ResourceManager<PixelShader>::Find(L"COLORUVPIXELSHADER");
	//m_ColMesh = ResourceManager<Mesh>::Find(L"COLORUVMESH");


	//m_Texture = ResourceManager<Texture>::Find(L"ladder.0.backHair.png");
	//m_Texture = ResourceManager<Texture>::Find(L"ladder.0.backHair.png");
	//m_Sampler = ResourceManager<Sampler>::Find(L"DefaultSampler");

	m_Blend = ResourceManager<Blend>::Find(L"Default2DBlend");

	return true;
}


void SpriteRenderer2D::SetSprite(const wchar_t* _SpriteName, size_t _Index)
{
	m_SpriteIndex = _Index;
	m_Sprite = ResourceManager<Sprite>::Find(_SpriteName);
}

void SpriteRenderer2D::Render(const Matrix& _ViewProjection)
{
	// ���� ��������Ʈ�� ���ٸ� ����
	if (nullptr == m_Sprite)
	{
		return;
	}

	SubTransformUpdate();


	// ����� OutPut Merger(��� ���ձ�) �ܰ��� �����̴�. ( ��� �ȼ����̴� ������ )
	
	VCOLOR Color = VCOLOR::Blue;
	//Temp.ColorKey = Vector3(1.0f, 1.0f, 0.0f);

	TestOutLine.OutLineColor = VCOLOR::Blue;
	//TestOutLine.OutLineTF.x = 0.0f;
	//TestOutLine.Position = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

	//if (Input::Down(L"Q"))
	//{
	//	TestOutLine.OutLineTF.x = 2.0f;
	//}


	//else if (Input::Down(L"T"))
	//{
	//	Temp.Check = 0;
	//}




	// ����
	//m_VertexShader->UpdateConstBuffer<Matrix>(L"WVP", _ViewProjection);
	/*m_VertexShader->UpdateConstBuffer<Matrix>(L"WVP", (GetTransform()->ConstRefWorldMatrix() * _ViewProjection).Get_Transpos());*/
	m_VertexShader->UpdateConstBuffer<Matrix>(L"WVP", (SubConstRefWorldMatrix() * _ViewProjection).Get_Transpos());
	m_VertexShader->UpdateConstBuffer<Vector4>(L"CALUV", m_Sprite->UvCal(m_SpriteIndex));			// ��������Ʈ �ε��� �־��ֱ�
	//m_PixelShader->UpdateConstBuffer<VCOLOR>(L"MULCOLOR",  Color );

	m_PixelShader->UpdateConstBuffer<TestBuffer>(L"MULCOLOR", TestOutLine);
	m_PixelShader->UpdateConstBuffer<SkillBuffer>(L"SKILLBF", m_BlazeBuffer);
	//OUTLINECOLOR
	//m_PixelShader->UpdateConstBuffer<VCOLOR>(L"MULCOLOR",  VCOLOR::White );

	m_Mesh->Update();
	m_VertexShader->Update();
	m_PixelShader->Update();


	m_Sprite->Update();

	//m_Texture->Update();
	//m_Sampler->Update();

	m_Blend->Update();		// ���������Ʈ

	m_Mesh->Render();





	// �ݶ��̴�
	//if (true == m_bDebugRender)
	//{
	//	int a = 0;
	//	m_ColVertexShader->UpdateConstBuffer<Matrix>(L"WVPC", (GetTransform()->ConstRefWorldMatrix() * _ViewProjection).Get_Transpos());
	//	m_ColPixelShader->UpdateConstBuffer<VCOLOR>(L"COLCOLOR", Color);
	//	m_ColMesh->Update();
	//	m_ColVertexShader->Update();
	//	m_ColPixelShader->Update();
	//	m_ColMesh->Render();
	//}
}
void SpriteRenderer2D::ChangeMesh(const wchar_t* _MeshKey)
{
	if (nullptr == ResourceManager<Mesh>::Find(_MeshKey))
		return;

	m_Mesh = ResourceManager<Mesh>::Find(_MeshKey);
}


void SpriteRenderer2D::Index(size_t _Index)
{
	if (m_Sprite->CurSize() <= _Index)
	{
		return;
	}

	m_SpriteIndex = _Index;
}
