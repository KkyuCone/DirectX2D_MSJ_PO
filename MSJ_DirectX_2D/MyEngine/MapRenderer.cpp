#include "MapRenderer.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "Transform.h"


MapRenderer::MapRenderer() 
	: m_VertexShader(nullptr), m_PixelShader(nullptr), m_Mesh(nullptr), m_Sprite(nullptr), m_Blend(nullptr)
	, FirstTile(0), BaseX(0), BaseY(0)
{
}


MapRenderer::~MapRenderer()
{
}

//

bool MapRenderer::Init(int _RenderGroup)
{
	Renderer::Init(_RenderGroup);

	m_VertexShader = ResourceManager<VertexShader>::Find(L"TEXTUREVERTEXSHADER");
	m_PixelShader = ResourceManager<PixelShader>::Find(L"TEXTUREPIXELSHADER");
	m_Mesh = ResourceManager<Mesh>::Find(L"TextureMesh");
	m_Blend = ResourceManager<Blend>::Find(L"Default2DBlend");

	return true;
}

void MapRenderer::Render(const Matrix& _ViewProjection)
{
	std::map<__int64, My_PTR<TileInfo>>::iterator StartIter = m_TileMap.begin();
	std::map<__int64, My_PTR<TileInfo>>::iterator EndIter = m_TileMap.end();

	Vector2 Index;
	Vector4 Position;
	Position.w = 1.0f;

	m_Scale.Scale(m_TileSize);

	for (; StartIter != EndIter; ++StartIter)
	{
		TestOutLine.OutLineColor = VCOLOR::Blue;

		Index.i64 = StartIter->first;
		// �̵��� ������� 
		Position.x = m_BaseTilePos.x + Index.ix * m_TileSize.x;
		Position.y = m_BaseTilePos.y + Index.iy * m_TileSize.y;
		Position.z = GetTransform()->World_Position().z;

		m_Scale.vec3 = Position;

		m_VertexShader->UpdateConstBuffer<Matrix>(L"WVP", (m_Scale * _ViewProjection).Get_Transpos());
		m_VertexShader->UpdateConstBuffer<Vector4>(L"CALUV", m_Sprite->UvCal(0));

		m_PixelShader->UpdateConstBuffer<TestBuffer>(L"MULCOLOR", TestOutLine);

		m_Mesh->Update();
		m_VertexShader->Update();
		m_PixelShader->Update();

		m_Sprite->Update();
		m_Blend->Update();
		m_Mesh->Render();
	}
}


void MapRenderer::CreateTile(int _x, int _y)
{
	if (0 == FirstTile)
	{
		BaseX = _x;
		BaseY = _y;
	}

	int TileIndexX = _x - BaseX;
	int TileIndexY = _y - BaseY;



	m_TileMap.insert(std::map<__int64, My_PTR<TileInfo>>::value_type(Vector2(TileIndexX, TileIndexY).i64, new TileInfo()));

}

void MapRenderer::CreateTile(Vector2 _Position)
{
	float XIndex;
	float YIndex;

	 //���ʷ� �������� �������� (0,0)
	if (0 == FirstTile)
	{
		m_BaseTilePos = _Position;
		XIndex = 0;		// ��ġ�����ͼ� Ÿ�Ͽ���� '��'
		YIndex = 0;
	}
	else 
	{
		XIndex = (_Position.x - m_BaseTilePos.x) / m_TileSize.x;		// ��ġ�����ͼ� Ÿ�Ͽ���� '��'
		YIndex = (_Position.y - m_BaseTilePos.y) / m_TileSize.y;
	}

	if (0 < XIndex)
	{
		XIndex += 0.5f;
	}

	if (0 < YIndex)
	{
		YIndex += 0.5f;
	}

	if (0 > XIndex)
	{
		XIndex -= 0.5f;
	}

	if (0 > YIndex)
	{
		YIndex -= 0.5f;
	}

	//�ε��� ����
	int iX = (int)XIndex;
	int iY = (int)YIndex;

	CreateTile(iX, iY);		// Ÿ�� ���

	if (0 == FirstTile)
	{
		FirstTile = 1;
	}

}

void MapRenderer::SetMapTileSprite(const wchar_t * _Name)
{
	m_Sprite = ResourceManager<Sprite>::Find(_Name);		// ��������Ʈ ����
}

size_t MapRenderer::GetTileSize()
{
	return m_Sprite->CurSize();
}

void MapRenderer::Clear()
{
	m_TileMap.clear();
}

//
std::list<MapRenderer::OwnershipTileData> MapRenderer::AllList()
{
	std::map<__int64, My_PTR<TileInfo>>::iterator StartIter = m_TileMap.begin();
	std::map<__int64, My_PTR<TileInfo>>::iterator EndIter = m_TileMap.end();

	std::list<MapRenderer::OwnershipTileData> List;

	for (; StartIter != EndIter;++StartIter)
	{
		MapRenderer::OwnershipTileData Info;

		Info.m_Index.i64 = StartIter->first;		// �ε��� ����
		Info.m_FullFilePath = StartIter->second->m_FullFilePath;		// ���� ��ü���
		List.push_back(Info);
	}

	return List;
}