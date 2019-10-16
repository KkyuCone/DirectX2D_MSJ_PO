#include "LandRenderer.h"
#include <ResourceManager.h>
#include <Device.h>
#include <Sprite.h>
#include <Transform.h>
#include <wincodec.h>
#include <atlstr.h>
#include <windows.storage.h>

LandRenderer::LandRenderer()
	: m_bFirstTileCheck(true)
	, BaseX(0), BaseY(0)
	, m_bAutoTileDrawCheck(false)
	, m_bSlim(false), m_bSlop(false), m_bDownSlop(false), m_bSmall(false)
	, m_bAutoUP(false), m_bAutoDOWN(false), m_bAutoLEFT(false), m_bAutoRIGHT(false)
	, m_bAutoDLEFT(false), m_bAutoDRIGHT(false), m_bAutoULEFT(false), m_bAutoURIGHT(false)
	, m_AutoTileSize(Vector4{ 0.0f, 0.0f, 1.0f, 1.0f })
{
	m_Scale.Identity();
	m_AutoTileScale.Identity();
}


LandRenderer::~LandRenderer()
{
	// ���� 
	std::list<TileInfo*>::iterator Start = m_TileInfoList.begin();
	std::list<TileInfo*>::iterator End = m_TileInfoList.end();

	for (; Start != End; ++Start)
	{
		if (nullptr != (*Start))
		{
			delete *Start;
		}
	}

	std::list<AutoTileInfo*>::iterator AutoStart = m_AutoTileInfoList.begin();
	std::list<AutoTileInfo*>::iterator AutoEnd = m_AutoTileInfoList.end();

	for (; AutoStart != AutoEnd; ++AutoStart)
	{
		if (nullptr != (*AutoStart))
		{
			delete *AutoStart;
		}
	}
}


// �������̵� �Լ�

bool LandRenderer::Init(int _RenderGroup)
{
	Renderer::Init(_RenderGroup);

	m_VertexShader = ResourceManager<VertexShader>::Find(L"TEXTUREVERTEXSHADER");
	m_PixelShader = ResourceManager<PixelShader>::Find(L"TEXTUREPIXELSHADER");
	m_Mesh = ResourceManager<Mesh>::Find(L"TextureMesh");
	m_Blend = ResourceManager<Blend>::Find(L"Default2DBlend");

	return true;
}

void LandRenderer::Render(const Matrix& _ViewProjection)
{
	// �ڵ� Ÿ�ϵ� �׷��ֱ�
	if (true == m_bAutoTileDrawCheck)
	{
		// �»�
		RenderAutoTile(m_AutoLandMap_LU, _ViewProjection);

		// ���
		RenderAutoTile(m_AutoLandMap_RU, _ViewProjection);

		// ����
		RenderAutoTile(m_AutoLandMap_LD, _ViewProjection);
		// ����
		RenderAutoTile(m_AutoLandMap_RD, _ViewProjection);
	}


	// ���⼭ Ÿ�ϵ��� �׷��ش�.
	std::map<__int64, TileInfo*>::iterator StartIter = m_LandMap.begin();
	std::map<__int64, TileInfo*>::iterator EndIter = m_LandMap.end();

	Vector2 Index = Vector2(0.0f, 0.0f);
	Vector4 Position = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

	m_Scale.Scale(m_TileSize);		// Ÿ�� ������־��ֱ� (����, ����)

	for (; StartIter != EndIter; ++StartIter)
	{
		TestOutLine.OutLineColor = VCOLOR::Blue;

		Index.i64 = StartIter->first;

		Position.x = m_BaseTilePos.x + Index.ix * m_TileSize.x;
		Position.y = m_BaseTilePos.y + Index.iy * m_TileSize.y;
		Position.z = GetTransform()->World_Position().z;

		m_Scale.vec3 = Position;				// vec3�� �̵���� '��'

		m_VertexShader->UpdateConstBuffer<Matrix>(L"WVP", (m_Scale * _ViewProjection).Get_Transpos());

		m_Sprite = ResourceManager<Sprite>::Find(StartIter->second->SpriteName);
		m_VertexShader->UpdateConstBuffer<Vector4>(L"CALUV", m_Sprite->UvCal(0));
		m_PixelShader->UpdateConstBuffer<TestBuffer>(L"MULCOLOR", TestOutLine);

		m_Mesh->Update();
		m_VertexShader->Update();
		m_PixelShader->Update();

		m_Sprite->Update();
		m_Blend->Update();
		m_Mesh->Render();
	}


	// �ڵ� Ÿ�ϵ� �׷��ֱ�
	if (true == m_bAutoTileDrawCheck)
	{
		// �� ��� - ��
		RenderAutoTile(m_AutoLandMap_SlopL, _ViewProjection);

		// �� ��� - ��
		RenderAutoTile(m_AutoLandMap_SlopR, _ViewProjection);

		// �ϴ� ��� - ��
		RenderAutoTile(m_AutoLandMap_SlopDL, _ViewProjection);

		// �ϴ� ��� - ��
		RenderAutoTile(m_AutoLandMap_SlopDR, _ViewProjection);


		// ��
		RenderAutoTile(m_AutoLandMap_L, _ViewProjection);

		// ��
		RenderAutoTile(m_AutoLandMap_R, _ViewProjection);

		// ��
		RenderAutoTile(m_AutoLandMap_D, _ViewProjection);

		// ��
		RenderAutoTile(m_AutoLandMap_U, _ViewProjection);


		// ���� ��
		RenderAutoTile(m_AutoLandMap_SD, _ViewProjection);

		// ���� ��
		RenderAutoTile(m_AutoLandMap_SlimL, _ViewProjection);

		// ���� ����
		RenderAutoTile(m_AutoLandMap_SlimLD, _ViewProjection);

		// ���� ��
		RenderAutoTile(m_AutoLandMap_SlimR, _ViewProjection);

		// ���� ����
		RenderAutoTile(m_AutoLandMap_SlimRD, _ViewProjection);

		// ���� ��
		RenderAutoTile(m_AutoLandMap_SlimD, _ViewProjection);
	}

}

// �ڵ� Ÿ�� ����
void LandRenderer::RenderAutoTile(std::map<__int64, AutoTileInfo*>& _AutoLandMap, const Matrix& _ViewProjection)
{
	std::map<__int64, AutoTileInfo*>::iterator Auto_StartIter = _AutoLandMap.begin();
	std::map<__int64, AutoTileInfo*>::iterator Auto_EndIter = _AutoLandMap.end();

	Vector2 AutoIndex = Vector2(0.0f, 0.0f);
	AutoIndex.ix = 0;
	AutoIndex.iy = 0;
	Vector4 AutoPosition = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

	// ������ '��'
	for (; Auto_StartIter != Auto_EndIter; ++Auto_StartIter)
	{
		m_Sprite = ResourceManager<Sprite>::Find(Auto_StartIter->second->SpriteName);

		m_AutoTileSize = Vector4((float)(m_Sprite->GetTexture()->Width()), (float)(m_Sprite->GetTexture()->Height()), 1.0f);

		m_AutoTileScale.Scale(m_AutoTileSize);		// Ÿ�� ������־��ֱ� (����, ����)
		TestOutLine.OutLineColor = VCOLOR::Blue;

		AutoIndex.i64 = Auto_StartIter->first;


		// �˻��ϱ� � Ÿ������..
		AUTOTILETYPE AutoTileType = Auto_StartIter->second->TILETYPE;

		switch (AutoTileType)
		{
		case LandRenderer::AUTOTILE_UP:
			// ��
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x);
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) + ((m_TileSize.y / 2) + (m_AutoTileSize.y / 2));

			break;
		case LandRenderer::AUTOTILE_LUP:
			// �»�
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) - ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) + ((m_TileSize.y / 2) + (m_AutoTileSize.y / 2));
			break;
		case LandRenderer::AUTOTILE_RUP:
			//  ���
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) + ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) + ((m_TileSize.y / 2) + (m_AutoTileSize.y / 2));
			break;
		case LandRenderer::AUTOTILE_LEFT:
			// ��
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) - ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y);
			break;
		case LandRenderer::AUTOTILE_RIGHT:
			// ��
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) + ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y);
			break;
		case LandRenderer::AUTOTILE_DOWN:
			// ��
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x);
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) - ((m_TileSize.y / 2) + (m_AutoTileSize.y / 2));
			break;
		case LandRenderer::AUTOTILE_LDOWN:
			// ����
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) - ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) - ((m_TileSize.y / 2) + (m_AutoTileSize.y / 2));
			break;
		case LandRenderer::AUTOTILE_RDOWN:
			// ����
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) + ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) - ((m_TileSize.y / 2) + (m_AutoTileSize.y / 2));
			break;
		case LandRenderer::AUTOTILE_SMALLDOWN:
			// ���� �� 
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x);
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) - ((m_TileSize.y / 2) + (m_AutoTileSize.y / 2));
			break;
		case LandRenderer::AUTOTILE_SLIM_LEFT:
			// ���� ��
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) - ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y);
			break;
		case LandRenderer::AUTOTIEL_SLIM_LDOWN:
			// ���� ����
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) - ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) - ((m_TileSize.y / 2) + (m_AutoTileSize.y / 2));
			break;
		case LandRenderer::AUTOTILE_SLIM_RIGHT:
			// ���� ��
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) + ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y);
			break;
		case LandRenderer::AUTOTILE_SLIM_RDOWN:
			// ���� ����
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) + ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) - ((m_TileSize.y / 2) + (m_AutoTileSize.y / 2));
			break;
		case LandRenderer::AUTOTILE_SLIM_DOWN:
			// ���� ��
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x);
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) - ((m_TileSize.y / 2) + (m_AutoTileSize.y / 2));
			break;
		case LandRenderer::AUTOTILE_SLOPE_LEFT:
			// �� ���
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) - ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) - (m_TileSize.y / 2) + ((m_AutoTileSize.y) / 2);
			break;
		case LandRenderer::AUTOTILE_SLOPE_RIGHT:
			// �� ���
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) + ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) - (m_TileSize.y / 2) + ((m_AutoTileSize.y) / 2);
			break;
		case LandRenderer::AUTOTILE_SLOPE_DLEFT:
			// �ϴ� �� ���
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) - ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) + (m_TileSize.y / 2) - ((m_AutoTileSize.y) / 2);
			break;
		case LandRenderer::AUTOTILE_SLOPE_DRIGHT:
			// �ϴ� �� ���
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) + ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) + (m_TileSize.y / 2) - ((m_AutoTileSize.y) / 2);
			break;
		default:
			return;
		}
		AutoPosition.z = GetTransform()->World_Position().z;		// ����

		m_AutoTileScale.vec3 = AutoPosition;				// vec3�� �̵���� '��'

		m_VertexShader->UpdateConstBuffer<Matrix>(L"WVP", (m_AutoTileScale * _ViewProjection).Get_Transpos());

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


// �׿�

void LandRenderer::SetTileSize(Vector2 _Size)
{
	m_TileSize.x = _Size.x;
	m_TileSize.y = _Size.y;
	m_TileSize.z = 1.0f;
}

void LandRenderer::SetAutoTileDrawCheck(bool _Value)
{
	m_bAutoTileDrawCheck = _Value;

	// �ѹ��� �������ִ°�.. �������� true�� �ٲ��ִϱ�
	// ���⼭ m_AutoLandMap�� �������ش�.

	if (true == m_bAutoTileDrawCheck)
	{
		// ������ Ÿ�� ����� ��� �׻� �ϳ��� ���̽� Ÿ�ϵ��� �� ���� �Ŀ� ������Ѿ��Ѵ�.
		std::map<__int64, TileInfo*>::iterator LandStart = m_LandMap.begin();
		std::map<__int64, TileInfo*>::iterator LandEnd = m_LandMap.end();

		Vector2 CurIndex = Vector2{ 0.0f, 0.0f };
		CurIndex = Vector2_Int(0, 0);

		for (; LandStart != LandEnd; ++LandStart)
		{
			// ���⼭ �˻��� �׷��ֱ�
			// ����Ÿ������, �⺻���̽� Ÿ������, ���ʰ��Ÿ������, �ϴܰ��Ÿ������ ����Ͽ� �׷������
			//CreateRemainderTile(LandStart->first);


			// ����Ǽ� 16����  2^4   ( 2�� true, false  2������ ���, 4�� ��,��,��,��)
			// ��� ���� ����Ǽ��� 16������ ���´�. 
			// ���⼭ �Ǵ��ؼ� Ÿ�� Ÿ�Ե� �˷�����
			// Ÿ�� Ÿ�Ժ��� ������ָ��

			// �����¿� �Ǵ�
			CurIndex = Vector2{ 0.0f, 0.0f };
			CurIndex.i64 = LandStart->first;				// �ε��� �޾ƿ���

			// �����¿� �ڵ�Ÿ���� �׸��� �ִ��� Ž��
			m_bAutoUP = FindTile(CurIndex, FIND_UP);
			m_bAutoDOWN = FindTile(CurIndex, FIND_DOWN);
			m_bAutoLEFT = FindTile(CurIndex, FIND_LEFT);
			m_bAutoRIGHT = FindTile(CurIndex, FIND_RIGHT);

			// �Ϲ� �� ��� �˻�
			m_bAutoDLEFT = FindTile(CurIndex, FIND_DLEFT);
			m_bAutoDRIGHT = FindTile(CurIndex, FIND_DRIGHT);

			// �ϴ� Ÿ�� ��� �˻�
			m_bAutoULEFT = FindTile(CurIndex, FIND_ULEFT);
			m_bAutoURIGHT = FindTile(CurIndex, FIND_URIGHT);

			// �⺻, ���, �ϴ� ���      -   ����Ÿ��

			if (true == m_bSlim)
			{

				// ���� Ÿ���� ���  - �¿츸 �Ǵ����� 

				// �ϴ�Ÿ���� ������ ����
				CreateRemainderTile(CurIndex.i64, AUTOTILE_SLIM_DOWN);		// ��

				if (false == m_bAutoLEFT && true == m_bAutoRIGHT)
				{
					// �� X, �� O
					// ���� - ��, ����, �� �߰� 
					CreateRemainderTile(CurIndex.i64, AUTOTILE_SLIM_LEFT);		// ��
					CreateRemainderTile(CurIndex.i64, AUTOTIEL_SLIM_LDOWN);	// ����
				}
				else if (true == m_bAutoLEFT && false == m_bAutoRIGHT)
				{
					// �� O, �� X
					// ���� - ��, ����, �� �߰�
					CreateRemainderTile(CurIndex.i64, AUTOTILE_SLIM_RIGHT);	// ��
					CreateRemainderTile(CurIndex.i64, AUTOTILE_SLIM_RDOWN);	// ����

				}
				else if (false == m_bAutoLEFT && false == m_bAutoRIGHT)
				{
					// �� X, �� X
					// ���� - ��, ����, �� �߰�
					// ���� - ��, ����, �� �߰�
					CreateRemainderTile(CurIndex.i64, AUTOTILE_SLIM_LEFT);		// ��
					CreateRemainderTile(CurIndex.i64, AUTOTIEL_SLIM_LDOWN);	// ����

					CreateRemainderTile(CurIndex.i64, AUTOTILE_SLIM_RIGHT);	// ��
					CreateRemainderTile(CurIndex.i64, AUTOTILE_SLIM_RDOWN);	// ����

				}
			}
			else
			{
				// ����Ÿ������ �⺻ ���̽�Ÿ������ �˻��ϱ�

				// ���� Ÿ���� ���
				if (true == m_bSmall)
				{
					// ��� �ϴ�Ÿ�ϸ� �׸����
					CreateRemainderTile(CurIndex.i64, AUTOTILE_SMALLDOWN);		// ��
				}
				else
				{
					// �⺻, ���, �ϴ� ���
					// ���� Ÿ���� �ƴѰ��
					// �׸���ġ�Ǵ��� �׸���
					if (false == m_bAutoUP && true == m_bAutoDOWN)
					{
						// ���Ÿ�� X, �ϴ�Ÿ�� O
						// ���Ÿ�� ����� ( �⺻, ���, �ϴܰ�� �ش�) , ���� Ÿ���� �ش���� X
						int a = 0;
						CreateRemainderTile(CurIndex.i64, AUTOTILE_UP);		// ���Ÿ��
						if (false == m_bAutoLEFT && true == m_bAutoRIGHT)
						{
							// �� X, �� O
							// ��Ÿ��. �»�� �߰�
							if (true == m_bSlop && true == m_bAutoDLEFT)
							{
								// ����� �����
								CreateRemainderTile(CurIndex.i64, AUTOTILE_SLOPE_LEFT);		// ��

							}
							else
							{
								// ������ ������ ���� ���
								CreateRemainderTile(CurIndex.i64, AUTOTILE_LEFT);		// ��
								CreateRemainderTile(CurIndex.i64, AUTOTILE_LUP);		// �»��
							}
						}
						else if (true == m_bAutoLEFT && false == m_bAutoRIGHT)
						{
							// �� O, �� X
							// ��, ���� �߰�
							if (true == m_bSlop && true == m_bAutoDRIGHT)
							{
								// ����� �����
								CreateRemainderTile(CurIndex.i64, AUTOTILE_SLOPE_RIGHT);	// ��
							}
							else
							{
								CreateRemainderTile(CurIndex.i64, AUTOTILE_RIGHT);		// ��
								CreateRemainderTile(CurIndex.i64, AUTOTILE_RUP);			// ����
							}
						}
						else if (false == m_bAutoLEFT && false == m_bAutoRIGHT)
						{
							// �� X, �� X
							// ��, �»�, ��, ��� �߰�

							if (true == m_bSlop)
							{
								if (true == m_bAutoDLEFT)
								{
									CreateRemainderTile(CurIndex.i64, AUTOTILE_SLOPE_LEFT);		// ��
								}
								else
								{
									CreateRemainderTile(CurIndex.i64, AUTOTILE_LEFT);			// ��
									CreateRemainderTile(CurIndex.i64, AUTOTILE_LUP);				// �»��

								}

								if (true == m_bAutoDRIGHT)
								{
									// ����� �����
									CreateRemainderTile(CurIndex.i64, AUTOTILE_SLOPE_RIGHT);	// ��
								}
								else
								{
									CreateRemainderTile(CurIndex.i64, AUTOTILE_RIGHT);			// ��
									CreateRemainderTile(CurIndex.i64, AUTOTILE_RUP);				// ����
								}
							}
							else
							{
								CreateRemainderTile(CurIndex.i64, AUTOTILE_LEFT);			// ��
								CreateRemainderTile(CurIndex.i64, AUTOTILE_RIGHT);			// ��

								CreateRemainderTile(CurIndex.i64, AUTOTILE_LUP);				// �»��
								CreateRemainderTile(CurIndex.i64, AUTOTILE_RUP);				// ����
							}
						}
						// �� O, �� O
					}
					else if (true == m_bAutoUP && false == m_bAutoDOWN)
					{
						// ���Ÿ�� O, �ϴ�Ÿ�� X
						// �ϴ�Ÿ�� ����
						CreateRemainderTile(CurIndex.i64, AUTOTILE_DOWN);		// ����

						if (false == m_bAutoLEFT && true == m_bAutoRIGHT)
						{
							// �� X, �� O
							// ��, ���ϴ�
							if (true == m_bDownSlop && true == m_bAutoULEFT)
							{
								// �� �ϴ� ��� Ÿ��
								CreateRemainderTile(CurIndex.i64, AUTOTILE_SLOPE_DLEFT);			// ��
							}
							else
							{
								CreateRemainderTile(CurIndex.i64, AUTOTILE_LEFT);			// ��
								CreateRemainderTile(CurIndex.i64, AUTOTILE_LDOWN);		// ���ϴ�
							}
						}
						else if (true == m_bAutoLEFT && false == m_bAutoRIGHT)
						{
							// �� O, �� X
							// ��, ���ϴ� �߰�
							if (true == m_bDownSlop && true == m_bAutoURIGHT)
							{
								// �� �ϴ� ��� Ÿ��
								CreateRemainderTile(CurIndex.i64, AUTOTILE_SLOPE_DRIGHT);			// ��
							}
							else
							{
								CreateRemainderTile(CurIndex.i64, AUTOTILE_RIGHT);		// ��
								CreateRemainderTile(CurIndex.i64, AUTOTILE_RDOWN);			// ���ϴ�
							}
						}
						else if (false == m_bAutoLEFT && false == m_bAutoRIGHT)
						{
							// �� X, �� X
							// ��, ����, ��, ���� �߰�
							if (true == m_bDownSlop)
							{
								// �ϴ� ��� Ÿ�� - �¿�
								if (true == m_bAutoULEFT)
								{
									CreateRemainderTile(CurIndex.i64, AUTOTILE_SLOPE_DLEFT);			// ��
								}
								else
								{
									CreateRemainderTile(CurIndex.i64, AUTOTILE_LEFT);				// ��
									CreateRemainderTile(CurIndex.i64, AUTOTILE_LDOWN);				// ���ϴ�
								}

								if (true == m_bAutoURIGHT)
								{
									CreateRemainderTile(CurIndex.i64, AUTOTILE_SLOPE_DRIGHT);			// ��
								}
								else
								{
									CreateRemainderTile(CurIndex.i64, AUTOTILE_RIGHT);				// ��
									CreateRemainderTile(CurIndex.i64, AUTOTILE_RDOWN);				// ���ϴ�
								}
							}
							else
							{
								CreateRemainderTile(CurIndex.i64, AUTOTILE_LEFT);				// ��
								CreateRemainderTile(CurIndex.i64, AUTOTILE_RIGHT);				// ��

								CreateRemainderTile(CurIndex.i64, AUTOTILE_LDOWN);				// ���ϴ�
								CreateRemainderTile(CurIndex.i64, AUTOTILE_RDOWN);				// ���ϴ�
							}
						}
						// �� O, �� O
					}
					else if (true == m_bAutoUP && true == m_bAutoDOWN)
					{
						// ���Ÿ�� O, �ϴ�Ÿ�� O
						// �� �����ϱ� ���ϴ� Ÿ�� �ȸ���

						if (false == m_bAutoLEFT && true == m_bAutoRIGHT)
						{
							// �� X, �� O
							// ��Ÿ��
							CreateRemainderTile(CurIndex.i64, AUTOTILE_LEFT);		// ��

						}
						else if (true == m_bAutoLEFT && false == m_bAutoRIGHT)
						{
							// �� O, �� X
							CreateRemainderTile(CurIndex.i64, AUTOTILE_RIGHT);		// ��
						}
						else if (false == m_bAutoLEFT && false == m_bAutoRIGHT)
						{
							// �� X, �� X
							CreateRemainderTile(CurIndex.i64, AUTOTILE_LEFT);				// ��
							CreateRemainderTile(CurIndex.i64, AUTOTILE_RIGHT);			// ��
						}
						// �� O, �� O
					}
					else if (false == m_bAutoUP && false == m_bAutoDOWN)
					{
						// ���Ÿ�� X, �ϴ�Ÿ�� X
						// ���, �ϴ� ����
						CreateRemainderTile(CurIndex.i64, AUTOTILE_UP);			// ��
						CreateRemainderTile(CurIndex.i64, AUTOTILE_DOWN);		// ��

						if (false == m_bAutoLEFT && true == m_bAutoRIGHT)
						{
							// �� X, �� O
							// ��, �»��, ���ϴ�
							CreateRemainderTile(CurIndex.i64, AUTOTILE_LEFT);	// ��
							CreateRemainderTile(CurIndex.i64, AUTOTILE_LUP);	// �»��
							CreateRemainderTile(CurIndex.i64, AUTOTILE_LDOWN);	// ���ϴ�
						}
						else if (true == m_bAutoLEFT && false == m_bAutoRIGHT)
						{
							// �� O, �� X
							// ��, ����, ���ϴ�
							CreateRemainderTile(CurIndex.i64, AUTOTILE_RIGHT);	// ��
							CreateRemainderTile(CurIndex.i64, AUTOTILE_RUP);	// ����
							CreateRemainderTile(CurIndex.i64, AUTOTILE_RDOWN);	// ���ϴ�
						}
						else if (false == m_bAutoLEFT && false == m_bAutoRIGHT)
						{
							// �� X, �� X
							// ��, �»��, ���ϴ�, ��, ����, ���ϴ�
							CreateRemainderTile(CurIndex.i64, AUTOTILE_LEFT);	// ��
							CreateRemainderTile(CurIndex.i64, AUTOTILE_LUP);	// �»��
							CreateRemainderTile(CurIndex.i64, AUTOTILE_LDOWN);	// ���ϴ�

							CreateRemainderTile(CurIndex.i64, AUTOTILE_RIGHT);	// ��
							CreateRemainderTile(CurIndex.i64, AUTOTILE_RUP);	// ����
							CreateRemainderTile(CurIndex.i64, AUTOTILE_RDOWN);	// ���ϴ�
						}
						// �� O, �� O
					}

				}
			}
		}
	}
}

bool LandRenderer::FindTile(Vector2 _Index, FINDTILE _Dir)
{
	Vector2 CurIndex = Vector2{ 0.0f, 0.0f };
	CurIndex.i64 = _Index.i64;				// �ε��� �޾ƿ���

	switch (_Dir)
	{
	case LandRenderer::FIND_UP:
		CurIndex.iy += 1;
		break;
	case LandRenderer::FIND_DOWN:
		CurIndex.iy -= 1;
		break;
	case LandRenderer::FIND_LEFT:
		CurIndex.ix -= 1;
		break;
	case LandRenderer::FIND_RIGHT:
		CurIndex.ix += 1;
		break;
	case LandRenderer::FIND_DLEFT:
		CurIndex.ix -= 1;
		CurIndex.iy -= 1;
		break;
	case LandRenderer::FIND_DRIGHT:
		CurIndex.ix += 1;
		CurIndex.iy -= 1;
		break;
	case LandRenderer::FIND_ULEFT:
		CurIndex.ix -= 1;
		CurIndex.iy += 1;
		break;
	case LandRenderer::FIND_URIGHT:
		CurIndex.ix += 1;
		CurIndex.iy += 1;
		break;
	default:
		EAssert(true);
		return true;
	}

	__int64 IndexFirst = CurIndex.i64;

	std::map<__int64, TileInfo*>::iterator FindIndex = m_LandMap.find(IndexFirst);

	if (m_LandMap.end() == FindIndex)
	{
		// �ڵ�Ÿ���� ��ġ�Ҽ� �ִٸ�
		return false;
	}

	return true;
}

void LandRenderer::SetAutoTileAllMap(std::unordered_map<std::wstring, std::vector<std::wstring>>& _CurAllAutoTile)
{
	m_CurAllAutoTile = _CurAllAutoTile;
}

void LandRenderer::SetTileImage(const wchar_t* _Name)
{
	// ����� �̹����̸��� �����ؼ� �ȵȴ�..
	// ó�����¾ַ� �����Ǿ�����.
	// ���߿� �ٲ��ֱ� ������
	m_Sprite = ResourceManager<Sprite>::Find(_Name);
	SpriteFileName = _Name;
}

void LandRenderer::SetAutoSlim(bool _Value)
{
	m_bSlim = _Value;
}

void LandRenderer::SetAutoSmall(bool _Value)
{
	m_bSmall = _Value;
}

void LandRenderer::SetAutoSlop(bool _Value)
{
	m_bSlop = _Value;
}

void LandRenderer::SetAutoDownSlop(bool _Value)
{
	m_bDownSlop = _Value;
}

Sprite* LandRenderer::GetSprite()
{
	return m_Sprite;
}

// ���� ���� Ÿ�� �׸���

void LandRenderer::CreateTile(int _x, int _y)
{
	// ��ó���� Ÿ������ üũ������� 
	if (true == m_bFirstTileCheck)
	{
		// ó���� Ÿ���̶�� (������)
		BaseX = _x;
		BaseY = _y;
	}

	int TileIndex_X = _x - BaseX;
	int TileIndex_Y = _y - BaseY;

	__int64 PosIndex = Vector2(TileIndex_X, TileIndex_Y).i64;

	// �̹� ���������� Ȯ��
	std::map<__int64, TileInfo*>::iterator StartIter = m_LandMap.begin();
	std::map<__int64, TileInfo*>::iterator EndIter = m_LandMap.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (PosIndex == StartIter->first)
		{
			return;
		}
	}

	// ���� ��ġ�� �ƴ϶�� ����
	TileInfo* NewTileInfo = new TileInfo();
	NewTileInfo->SpriteName = SpriteFileName;

	m_TileInfoList.push_back(NewTileInfo);
	m_LandMap.insert(std::map<__int64, TileInfo*>::value_type(PosIndex, NewTileInfo));

}

void LandRenderer::CreateTile(Vector2 _Pos)
{
	float TileIndex_X = 0.0f;
	float TileIndex_Y = 0.0f;

	if (true == m_bFirstTileCheck)
	{
		// ù��° Ÿ���̶�� (����Ÿ��)
		m_BaseTilePos = _Pos;

		// ù��°Ÿ���� ������ 0,0�̴�.
		TileIndex_X = 0.0f;
		TileIndex_Y = 0.0f;
	}
	else
	{
		// �ι�°Ÿ�Ϻ���..
		TileIndex_X = (_Pos.x - m_BaseTilePos.x) / m_TileSize.x;
		TileIndex_Y = (_Pos.y - m_BaseTilePos.y) / m_TileSize.y;
	}


	if (0 < TileIndex_X)
	{
		TileIndex_X += 0.5f;
	}

	if (0 < TileIndex_Y)
	{
		TileIndex_Y += 0.5f;
	}

	if (0 > TileIndex_X)
	{
		TileIndex_X -= 0.5f;
	}

	if (0 > TileIndex_Y)
	{
		TileIndex_Y -= 0.5f;
	}


	// �ε��� �Ҽ� -> ������ �ٲ���Ѵ�.
	int iXIndex = (int)TileIndex_X;
	int iYIndex = (int)TileIndex_Y;

	CreateTile(iXIndex, iYIndex);

	if (true == m_bFirstTileCheck)
	{
		m_bFirstTileCheck = false;		// ó��Ÿ���� ����ٸ� false
	}
}


// �ڵ� Ÿ�� �����
void LandRenderer::CreateRemainderTile(__int64 _Index, AUTOTILETYPE _AutoType)
{
	// �޾ƿ��� �ε�������
	// ���� ��ġ�ؾ��ϴ� ����Ÿ���� �ε����� '��'

	Vector2 GenTileIndex = Vector2{ 0.0f, 0.0f };     // �ε��� ����(���̽�)
	GenTileIndex.i64 = _Index;

	AutoTileInfo* NewAutoTileInfo = new AutoTileInfo();
	std::wstring TileImageName = L"";
	int RandomValue = -1;

	// �׸� Ÿ�ϵ� Ž��
	switch (_AutoType)
	{
	case LandRenderer::AUTOTILE_UP:
		// ��
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_UpTile");
		break;
	case LandRenderer::AUTOTILE_LUP:
		// �»�
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_SUpLHalfTile");
		break;
	case LandRenderer::AUTOTILE_RUP:
		// ���
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_SUpRHalfTile");
		break;
	case LandRenderer::AUTOTILE_LEFT:
		// ��
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_LeftTile");
		break;
	case LandRenderer::AUTOTILE_RIGHT:
		// ��
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_RightTile");
		break;
	case LandRenderer::AUTOTILE_DOWN:
		// ��
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_DownTile");
		break;
	case LandRenderer::AUTOTILE_LDOWN:
		// ����
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_SDownLHalfTile");
		break;
	case LandRenderer::AUTOTILE_RDOWN:
		// ����
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_SDownRHalfTile");
		break;
	case LandRenderer::AUTOTILE_SMALLDOWN:
		// ����Ÿ��
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_SDownTile");
		break;
	case LandRenderer::AUTOTILE_SLIM_LEFT:
		// ���� ��
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_SUpLHalfTile");
		break;
	case LandRenderer::AUTOTIEL_SLIM_LDOWN:
		// ���� ���� 
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_SDownLHalfTile");
		break;
	case LandRenderer::AUTOTILE_SLIM_RIGHT:
		// ���� ��
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_SUpRHalfTile");
		break;
	case LandRenderer::AUTOTILE_SLIM_RDOWN:
		// ���� ����
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_SDownRHalfTile");
		break;
	case LandRenderer::AUTOTILE_SLIM_DOWN:
		// ���� ��
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_DownTile");
		break;
	case LandRenderer::AUTOTILE_SLOPE_LEFT:
		// �� ���
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_LSlopTile");
		break;
	case LandRenderer::AUTOTILE_SLOPE_RIGHT:
		// �� ���
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_RSlopTile");
		break;
	case LandRenderer::AUTOTILE_SLOPE_DLEFT:
		// �ϴ� �°��
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_LDownTile");
		break;
	case LandRenderer::AUTOTILE_SLOPE_DRIGHT:
		// �ϴ� ����
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_RDownTile");
		break;
	default:
		// ������ (�ȸ��������Ѵ�.)
		delete NewAutoTileInfo;
		return;
	}


	// ����ó�� -> ��ã�Ҵٸ�..
	if (m_CurAllAutoTile.end() == DrawTileMap)
	{
		delete NewAutoTileInfo;
		return;
	}

	DrawCurTiles = DrawTileMap->second;							// �׸����ִ� Ÿ�ϵ�(vector����) ��������
	RandomValue = rand() % DrawCurTiles.size();					// ����
	NewAutoTileInfo->SpriteName = DrawCurTiles[RandomValue];	// �׸� Ÿ���̸� ����
	NewAutoTileInfo->TILETYPE = _AutoType;						// Ÿ�� Ÿ��


	m_AutoTileInfoList.push_back(NewAutoTileInfo);				// �׸�Ÿ������ �־��ֱ� 

	// �ش� �ڵ�Ÿ�ϵ��� ��������� ���� �־��ֱ� 

	switch (_AutoType)
	{
	case LandRenderer::AUTOTILE_UP:	// ��
		m_AutoLandMap_U.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_LUP:	// �»�
		m_AutoLandMap_LU.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_RUP:	// ���
		m_AutoLandMap_RU.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_LEFT:	// ��
		m_AutoLandMap_L.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_RIGHT:	// ��
		m_AutoLandMap_R.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_DOWN:	// ��
		m_AutoLandMap_D.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_LDOWN:	// ����
		m_AutoLandMap_LD.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_RDOWN:	// ����
		m_AutoLandMap_RD.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_SMALLDOWN:	// ������ 
		m_AutoLandMap_SD.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_SLIM_LEFT:	// ���� ��
		m_AutoLandMap_SlimL.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTIEL_SLIM_LDOWN:	// ���� ����
		m_AutoLandMap_SlimLD.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_SLIM_RIGHT:	// ���� ��
		m_AutoLandMap_SlimR.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_SLIM_RDOWN:	// ���� ����
		m_AutoLandMap_SlimRD.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_SLIM_DOWN:		// ���� ��
		m_AutoLandMap_SlimD.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_SLOPE_LEFT:		// �� ���
		m_AutoLandMap_SlopL.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_SLOPE_RIGHT:	// �� ���
		m_AutoLandMap_SlopR.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_SLOPE_DLEFT:	// �ϴ� �� ���
		m_AutoLandMap_SlopDL.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_SLOPE_DRIGHT:	// �ϴ� �� ���
		m_AutoLandMap_SlopDR.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	}
}

// �����
void LandRenderer::Clear()
{
	m_LandMap.clear();
}


DirectX::Image* LandRenderer::DrawSavePNG(DirectX::Image* _EmptyImage)
{

	DirectX::ScratchImage pImage;

	HRESULT hr = DirectX::CaptureTexture(Device::Get_Device(), Device::Get_Context(), (ID3D11Resource*)(m_Sprite->GetTexture()->GetTexture2D()), pImage);

	const DirectX::Image* SaveImage = pImage.GetImages();

	return nullptr;
}


/////////////////////////////////////////////////////////////////////////////////////////////

void LandRenderer::DrawLandMap(int* _Pixel, size_t _SceneSizeX, size_t _SceneSizeY)
{
	// �ڵ�Ÿ��
	SaveAutoTile(m_AutoLandMap_LU, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_RU, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_LD, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_RD, _Pixel, _SceneSizeX, _SceneSizeY);


	// �⺻Ÿ��
	SaveBaseTile(m_LandMap, _Pixel, _SceneSizeX, _SceneSizeY);

	 // �ڵ�Ÿ��
	// ���
	SaveAutoTile(m_AutoLandMap_SlopL, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_SlopR, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_SlopDL, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_SlopDR, _Pixel, _SceneSizeX, _SceneSizeY);

	// �����¿�
	SaveAutoTile(m_AutoLandMap_L, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_R, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_D, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_U, _Pixel, _SceneSizeX, _SceneSizeY);

	// ��Ÿ
	SaveAutoTile(m_AutoLandMap_SD, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_SlimL, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_SlimLD, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_SlimR, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_SlimRD, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_SlimD, _Pixel, _SceneSizeX, _SceneSizeY);


}


// �⺻���̽� Ÿ��
void LandRenderer::SaveBaseTile(std::map<__int64, TileInfo*>& _AutoLandMap, int* _Pixel, size_t _SceneSizeX, size_t _SceneSizeY)
{
	// �⺻Ÿ��
	std::map<__int64, TileInfo*>::iterator BaseLandStart = m_LandMap.begin();
	std::map<__int64, TileInfo*>::iterator BaseLandEnd = m_LandMap.end();

	for (; BaseLandStart != BaseLandEnd; ++BaseLandStart)
	{
		m_Sprite = ResourceManager<Sprite>::Find(BaseLandStart->second->SpriteName);


		if (nullptr == m_Sprite)
		{
			EAssert(true);
			return;
		}
		DXGI_FORMAT SpriteFormat = m_Sprite->GetTexture()->Format();
		// ���������� �̹��� �׸��� (�ȼ�)
		// ��ġ ���
		// �ϴ� �⺻ Ÿ�ϸ� ���� 


		// �ش� ��������Ʈ ������ (����,����)
		size_t ImageWidth = m_Sprite->GetTexture()->Width();
		size_t ImageHeight = m_Sprite->GetTexture()->Height();


		// �ε��� ��
		Vector2 Index = Vector2{ 0.0f, 0.0f };
		Index.i64 = BaseLandStart->first;


		// �̹��� ���� �߾��� ������
		int ImageCenterPosX = ((int)m_BaseTilePos.x + (Index.ix * (int)ImageWidth));
		int ImageCenterPosY = ((int)m_BaseTilePos.y + (Index.iy * (int)ImageHeight));

		// �߾� �������� �ȼ�
		int CenterPixelX = ((int)_SceneSizeX / 2) + ImageCenterPosX;
		int CenterPixelY = ((int)_SceneSizeY / 2) - ImageCenterPosY;

		// ������� ���� �ȼ����� ù�ȼ��� ������
		int FirstPixelX = CenterPixelX - (int)(ImageWidth / 2);
		int FirstPixelY = CenterPixelY - (int)(ImageHeight / 2);




		for (size_t y = 0; y < ImageHeight; ++y)
		{
			for (size_t x = 0; x < ImageWidth; ++x)
			{
				// ���ȼ��� �����ͼ� ������ġ ��ǥ�� ������ �����ϱ�
				unsigned int XValue = (unsigned int)x;
				unsigned int YValue = (unsigned int)y;

				uint8_t* PixelRGBA = m_Sprite->GetTexture()->ImageOriginGetPixel(XValue, YValue);		// �ش� �̹����� �ȼ��� �޾ƿ���(ARGB)


				uint8_t r = PixelRGBA[0];		// ���
				uint8_t g = PixelRGBA[1];		// �׸�
				uint8_t b = PixelRGBA[2];		// ����
				uint8_t a = PixelRGBA[3];		// ����

				if (a <= 0)
				{
					continue;
				}

				// ���� ����Ŀ� �ȼ��� �־��ֱ� (������ ������)
				if (FirstPixelX + x > _SceneSizeX || FirstPixelY + y > 768
					|| FirstPixelX + x < 0 || FirstPixelY + y < 0)
				{
					continue;
				}

				int tempChangePixel = _Pixel[(_SceneSizeX*(FirstPixelY + y)) + (FirstPixelX + x)];

				if (tempChangePixel != 0x00ff0000)
				{
					continue;
				}



				std::wstring CheckSpriteName = BaseLandStart->second->SpriteName;

				if (std::wstring::npos != CheckSpriteName.find(L"_edU."))
				{
					_Pixel[(_SceneSizeX*(FirstPixelY + y)) + (FirstPixelX + x)] = *(int*)PixelRGBA;
				}
				else
				{
					PixelRGBA[0] = r;			   // ���
					PixelRGBA[1] = g;				// �׸� 
					PixelRGBA[2] = b;				// ����6
					PixelRGBA[3] = a;				// ����

					int PA = (int)a;
					int PR = (int)r;
					int pG = (int)g;
					int pB = (int)b;

					int CurPixel = 0x00000000;

					CurPixel |= PA;
					CurPixel = CurPixel << 8;
					CurPixel |= PR;
					CurPixel = CurPixel << 8;
					CurPixel |= pG;
					CurPixel = CurPixel << 8;
					CurPixel |= pB;

					_Pixel[(_SceneSizeX*(FirstPixelY + y)) + (FirstPixelX + x)] = CurPixel;
				}
				//_Pixel[(_SceneSizeX*(FirstPixelY + y)) + (FirstPixelX + x)] = *(int*)PixelRGBA;
			}
		}
	}
}





// �ڵ�Ÿ��
void LandRenderer::SaveAutoTile(std::map<__int64, AutoTileInfo*>& _AutoLandMap, int* _Pixel, size_t _SceneSizeX, size_t _SceneSizeY)
{
	// �ڵ� Ÿ��
	std::map<__int64, AutoTileInfo*>::iterator AutoStart = _AutoLandMap.begin();
	std::map<__int64, AutoTileInfo*>::iterator AutoEnd = _AutoLandMap.end();

	for (; AutoStart != AutoEnd; ++AutoStart)
	{
		m_Sprite = ResourceManager<Sprite>::Find(AutoStart->second->SpriteName);

		if (nullptr == m_Sprite)
		{
			EAssert(true);
			return;
		}
		// ���������� �̹��� �׸��� (�ȼ�)
		// ��ġ ���
		// �ϴ� �⺻ Ÿ�ϸ� ���� 

		DXGI_FORMAT SpriteFormat = m_Sprite->GetTexture()->Format();


		// �ش� ��������Ʈ ������ (����,����)
		size_t ImageWidth = m_Sprite->GetTexture()->Width();
		size_t ImageHeight = m_Sprite->GetTexture()->Height();


		// �ε��� ��
		Vector2 Index = Vector2{ 0.0f, 0.0f };
		Index.i64 = AutoStart->first;


		// �̹��� ���� �߾��� ������
		int ImageCenterPosX = 0;
		int ImageCenterPosY = 0;


		// �˻��ϱ� � Ÿ������..
		AUTOTILETYPE AutoTileType = AutoStart->second->TILETYPE;

		switch (AutoTileType)
		{
		case LandRenderer::AUTOTILE_UP:
			// ��
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x);
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) + (((int)m_TileSize.y / 2) + ((int)ImageHeight / 2));

			break;
		case LandRenderer::AUTOTILE_LUP:
			// �»�
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) - (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) + (((int)m_TileSize.y / 2) + ((int)ImageHeight / 2));
			break;
		case LandRenderer::AUTOTILE_RUP:
			//  ���
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) + (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) + (((int)m_TileSize.y / 2) + ((int)ImageHeight / 2));
			break;
		case LandRenderer::AUTOTILE_LEFT:
			// ��
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) - (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y);
			break;
		case LandRenderer::AUTOTILE_RIGHT:
			// ��
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) + (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y);
			break;
		case LandRenderer::AUTOTILE_DOWN:
			// ��
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x);
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) - (((int)m_TileSize.y / 2) + ((int)ImageHeight / 2));
			break;
		case LandRenderer::AUTOTILE_LDOWN:
			// ����
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) - (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) - (((int)m_TileSize.y / 2) + ((int)ImageHeight / 2));
			break;
		case LandRenderer::AUTOTILE_RDOWN:
			// ����
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) + (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) - (((int)m_TileSize.y / 2) + ((int)ImageHeight / 2));
			break;
		case LandRenderer::AUTOTILE_SMALLDOWN:
			// ���� �� 
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x);
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) - (((int)m_TileSize.y / 2) + ((int)ImageHeight / 2));
			break;
		case LandRenderer::AUTOTILE_SLIM_LEFT:
			// ���� ��
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) - (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y);
			break;
		case LandRenderer::AUTOTIEL_SLIM_LDOWN:
			// ���� ����
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) - (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) - (((int)m_TileSize.y / 2) + ((int)ImageHeight / 2));
			break;
		case LandRenderer::AUTOTILE_SLIM_RIGHT:
			// ���� ��
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) + (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y);
			break;
		case LandRenderer::AUTOTILE_SLIM_RDOWN:
			// ���� ����
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) + (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) - (((int)m_TileSize.y / 2) + ((int)ImageHeight / 2));
			break;
		case LandRenderer::AUTOTILE_SLIM_DOWN:
			// ���� ��
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x);
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) - (((int)m_TileSize.y / 2) + ((int)ImageHeight / 2));
			break;
		case LandRenderer::AUTOTILE_SLOPE_LEFT:
			// �� ���
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) - (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) - ((int)m_TileSize.y / 2) + (((int)ImageHeight) / 2);
			break;
		case LandRenderer::AUTOTILE_SLOPE_RIGHT:
			// �� ���
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) + (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) - ((int)m_TileSize.y / 2) + (((int)ImageHeight) / 2);
			break;
		case LandRenderer::AUTOTILE_SLOPE_DLEFT:
			// �ϴ� �� ���
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) - (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) + ((int)m_TileSize.y / 2) - (((int)ImageHeight) / 2);
			break;
		case LandRenderer::AUTOTILE_SLOPE_DRIGHT:
			// �ϴ� �� ���
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) + (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) + ((int)m_TileSize.y / 2) - (((int)ImageHeight) / 2);
			break;
		default:
			return;
		}



		// �߾� �������� �ȼ�
		int CenterPixelX = ((int)_SceneSizeX / 2) + ImageCenterPosX;
		int CenterPixelY = ((int)_SceneSizeY / 2) - ImageCenterPosY;

		// ������� ���� �ȼ����� ù�ȼ��� ������
		int FirstPixelX = CenterPixelX - (int)(ImageWidth / 2);
		int FirstPixelY = CenterPixelY - (int)(ImageHeight / 2);




		for (size_t y = 0; y < ImageHeight; ++y)
		{
			for (size_t x = 0; x < ImageWidth; ++x)
			{
				int CurPixel = 0x00000000;

				// ���ȼ��� �����ͼ� ������ġ ��ǥ�� ������ �����ϱ�
				uint8_t* PixelRGBA = m_Sprite->GetTexture()->ImageOriginGetPixel((unsigned int)x, (unsigned int)y);		// �ش� �̹����� �ȼ��� �޾ƿ���(ARGB)


				uint8_t r = PixelRGBA[0];		
				uint8_t g = PixelRGBA[1];		
				uint8_t b = PixelRGBA[2];		
				uint8_t a = PixelRGBA[3];		

				if (a <= 0)
				{
					continue;
				}


						// ARGB
				PixelRGBA[0] = r;			   // ���
				PixelRGBA[1] = g;				// �׸� 
				PixelRGBA[2] = b;				// ����6
				PixelRGBA[3] = a;				// ����

				// ���� ����Ŀ� �ȼ��� �־��ֱ� (������ ������)
				if (FirstPixelX + x > _SceneSizeX || FirstPixelY + y > 768
					|| FirstPixelX + x < 0 || FirstPixelY + y < 0)
				{
					continue;
				}

				int tempChangePixel = _Pixel[(_SceneSizeX*(FirstPixelY + y)) + (FirstPixelX + x)];

				if (tempChangePixel != 0x00ff0000)
				{
					continue;
				}

				_Pixel[(_SceneSizeX*(FirstPixelY + y)) + (FirstPixelX + x)] = *(int*)PixelRGBA;

			}
		}
	}
}
