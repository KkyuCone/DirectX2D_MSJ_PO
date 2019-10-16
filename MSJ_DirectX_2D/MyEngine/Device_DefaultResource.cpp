#include "Device.h"
#include "DXHeader.h"
#include "VertexHeader.h"
#include "Mesh.h"
#include "Path.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "Blend.h"
#include "GameFont.h"


void Device::DefaultResource_Init()
{
	// �⺻ �����Ͷ����� ������Ʈ
	RasterizeState_Create(L"SOLID", D3D11_CULL_MODE::D3D11_CULL_NONE, D3D11_FILL_MODE::D3D11_FILL_SOLID);
	RasterizeState_Create(L"WIREFRAME", D3D11_CULL_MODE::D3D11_CULL_NONE, D3D11_FILL_MODE::D3D11_FILL_WIREFRAME);
	RasterizeState_Change(L"SOLID");


	// �⺻ ���÷��� ����
	ResourceManager<GameFont>::Create(L"�����ý��丮");
	ResourceManager<Sampler>::Create(L"DefaultSampler");
	ResourceManager<Blend>::Create(L"Default2DBlend");

	// �⺻�޽� '��'
	VertexColor m_ArrayVertex[4] = {};

	m_ArrayVertex[0].Position = Vector4(-0.5f, 0.5f, 0.0f, 1.0f);			// ������
	m_ArrayVertex[1].Position = Vector4(0.5f, 0.5f, 0.0f, 1.0f);			// ��������
	m_ArrayVertex[2].Position = Vector4(-0.5f, -0.5f, 0.0f, 1.0f);			// ���� �Ʒ�
	m_ArrayVertex[3].Position = Vector4(0.5f, -0.5f, 0.0f, 1.0f);			// ������ �Ʒ�


	m_ArrayVertex[0].Color = VCOLOR::White;
	m_ArrayVertex[1].Color = VCOLOR::White;
	m_ArrayVertex[2].Color = VCOLOR::White;
	m_ArrayVertex[3].Color = VCOLOR::White;

	// �׸��� �ɼ��� �ð�������� �׷���� �ﰢ���� ���´�. �ݽð�����̸� �ȵ� ������
	IDX16 m_ArrayIndex[2] = {};
	m_ArrayIndex[0] = IDX16(0, 3, 2);
	m_ArrayIndex[1] = IDX16(0, 1, 3);

	ResourceManager<Mesh>::Create(L"COLORMESH", 4, sizeof(VertexColor), D3D11_USAGE_DYNAMIC, m_ArrayVertex, 6, IDX16::MemberSize(), D3D11_USAGE_DYNAMIC, m_ArrayIndex, IDX16::Format());


	// UV ������
	// Uv�� �ؼ��� ������ ǥ���Ѱ��̴� �ؼ��̶� �ؽ�ó�� ���� �ȼ��̴�.
	// �ؽ�ó�� ������� ����.. �������¸� �׸��°���
	// �̹����� ������ �׸��� �ƴ϶� ������ ���ִ� ����

	UvVertexColor m_ArrayUvVertex[4] = {};

	m_ArrayUvVertex[0].Position = Vector4(-0.5f, 0.5f, 0.0f, 1.0f);			// ������
	m_ArrayUvVertex[1].Position = Vector4(0.5f, 0.5f, 0.0f, 1.0f);			// ��������
	m_ArrayUvVertex[2].Position = Vector4(-0.5f, -0.5f, 0.0f, 1.0f);			// ���� �Ʒ�
	m_ArrayUvVertex[3].Position = Vector4(0.5f, -0.5f, 0.0f, 1.0f);			// ������ �Ʒ�

	m_ArrayUvVertex[0].Uv = Vector2(0.0f, 0.0f);
	m_ArrayUvVertex[1].Uv = Vector2(1.0f, 0.0f);
	m_ArrayUvVertex[2].Uv = Vector2(0.0f, 1.0f);
	m_ArrayUvVertex[3].Uv = Vector2(1.0f, 1.0f);

	m_ArrayUvVertex[0].Color = VCOLOR::White;
	m_ArrayUvVertex[1].Color = VCOLOR::White;
	m_ArrayUvVertex[2].Color = VCOLOR::White;
	m_ArrayUvVertex[3].Color = VCOLOR::White;

	// �������~  UV

	// ���ؽ�
	//My_PTR<VertexShader> VTX = ResourceManager<VertexShader>::Load(L"COLORVERTEXSHADER", Path::FindPath(L"Shader") + L"ColorRect.fx", 5, 0, "VS_ColorRect");
	//// Layout_Add -> Input Layout �����
	//VTX->Layout_Add("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
	//VTX->Layout_End("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
	//VTX->ConstBuffer_Create<Matrix>(L"WVPC", 0, D3D11_USAGE::D3D11_USAGE_DYNAMIC);

	//ResourceManager<PixelShader>::Load(L"COLORPIXELSHADER", Path::FindPath(L"Shader") + L"ColorRect.fx", 5, 0, "PS_ColorRect");


	// �⺻ �ؽ�ó ������
	TextureVertex m_ArrayTextureVtx[4] = {};

	m_ArrayTextureVtx[0].Position = Vector4(-0.5f, 0.5f, 0.0f, 1.0f);			// ������
	m_ArrayTextureVtx[1].Position = Vector4(0.5f, 0.5f, 0.0f, 1.0f);			// ��������
	m_ArrayTextureVtx[2].Position = Vector4(-0.5f, -0.5f, 0.0f, 1.0f);			// ���� �Ʒ�
	m_ArrayTextureVtx[3].Position = Vector4(0.5f, -0.5f, 0.0f, 1.0f);			// ������ �Ʒ�


	m_ArrayTextureVtx[0].Uv = Vector2(0.0f, 0.0f);
	m_ArrayTextureVtx[1].Uv = Vector2(1.0f, 0.0f);
	m_ArrayTextureVtx[2].Uv = Vector2(0.0f, 1.0f);
	m_ArrayTextureVtx[3].Uv = Vector2(1.0f, 1.0f);


	int a = 0;

	ResourceManager<Mesh>::Create(L"TextureMesh", 4, sizeof(TextureVertex), D3D11_USAGE_DYNAMIC, m_ArrayTextureVtx, 6, IDX16::MemberSize(), D3D11_USAGE_DYNAMIC, m_ArrayIndex, IDX16::Format());

	m_ArrayTextureVtx[0].Position = Vector4(0.f, 0.5f, 0.0f, 1.0f);			// ������
	m_ArrayTextureVtx[1].Position = Vector4(1.0f, 0.5f, 0.0f, 1.0f);		// ��������
	m_ArrayTextureVtx[2].Position = Vector4(0.f, -0.5f, 0.0f, 1.0f);		// ���� �Ʒ�
	m_ArrayTextureVtx[3].Position = Vector4(1.f, -0.5f, 0.0f, 1.0f);		// ������ �Ʒ�
	
	ResourceManager<Mesh>::Create(L"TextureMesh2", 4, sizeof(TextureVertex), D3D11_USAGE_DYNAMIC, m_ArrayTextureVtx, 6, IDX16::MemberSize(), D3D11_USAGE_DYNAMIC, m_ArrayIndex, IDX16::Format());

	m_ArrayTextureVtx[0].Position = Vector4(-1.0f, 0.5f, 0.0f, 1.0f);			// ������
	m_ArrayTextureVtx[1].Position = Vector4(0.0f, 0.5f, 0.0f, 1.0f);		// ��������
	m_ArrayTextureVtx[2].Position = Vector4(-1.0f, -0.5f, 0.0f, 1.0f);		// ���� �Ʒ�
	m_ArrayTextureVtx[3].Position = Vector4(0.0f, -0.5f, 0.0f, 1.0f);		// ������ �Ʒ�

	ResourceManager<Mesh>::Create(L"TextureMesh3", 4, sizeof(TextureVertex), D3D11_USAGE_DYNAMIC, m_ArrayTextureVtx, 6, IDX16::MemberSize(), D3D11_USAGE_DYNAMIC, m_ArrayIndex, IDX16::Format());


	My_PTR<VertexShader> TEXVTX = ResourceManager<VertexShader>::Load(L"TEXTUREVERTEXSHADER", Path::FindPath(L"Shader") + L"Texture.fx", 5, 0, "VS_Texture");
	// Layout_Add -> Input Layout �����
	TEXVTX->Layout_Add("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
	TEXVTX->Layout_End("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 0);
	TEXVTX->ConstBuffer_Create<Matrix>(L"WVP", 0, D3D11_USAGE::D3D11_USAGE_DYNAMIC);
	TEXVTX->ConstBuffer_Create<Vector4>(L"CALUV", 1, D3D11_USAGE::D3D11_USAGE_DYNAMIC);

	My_PTR<PixelShader> TEXPIX = ResourceManager<PixelShader>::Load(L"TEXTUREPIXELSHADER", Path::FindPath(L"Shader") + L"Texture.fx", 5, 0, "PS_Texture");
	//TEXPIX->ConstBuffer_Create<VCOLOR>(L"MULCOLOR", 1, D3D11_USAGE_DYNAMIC);

	TEXPIX->ConstBuffer_Create<TestBuffer>(L"MULCOLOR", 1, D3D11_USAGE_DYNAMIC);
	TEXPIX->ConstBuffer_Create<SkillBuffer>(L"SKILLBF", 1, D3D11_USAGE_DYNAMIC);

	//TEXPIX->ConstBuffer_Create<TestConstBuffer>(L"OUTLINECOLOR", 0, D3D11_USAGE_DYNAMIC);



	// UV + �浹�����
	ResourceManager<Mesh>::Create(L"COLORUVMESH", 4, sizeof(UvVertexColor), D3D11_USAGE_DYNAMIC, m_ArrayUvVertex, 6, IDX16::MemberSize(), D3D11_USAGE_DYNAMIC, m_ArrayIndex, IDX16::Format());

	My_PTR<VertexShader> UVVTX = ResourceManager<VertexShader>::Load(L"COLORUVVERTEXSHADER", Path::FindPath(L"Shader") + L"UVRect.fx", 5, 0, "VS_UVRect");
	// Layout_Add -> Input Layout �����
	UVVTX->Layout_Add("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
	UVVTX->Layout_Add("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
	UVVTX->Layout_End("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 0);
	UVVTX->ConstBuffer_Create<Matrix>(L"ColDebugWVP", 0, D3D11_USAGE::D3D11_USAGE_DYNAMIC);

	My_PTR<PixelShader> UVPIXEL = ResourceManager<PixelShader>::Load(L"COLORUVPIXELSHADER", Path::FindPath(L"Shader") + L"UVRect.fx", 5, 0, "PS_UVRect");
	UVPIXEL->ConstBuffer_Create<VCOLOR>(L"COLCOLOR", 1, D3D11_USAGE_DYNAMIC);
}
