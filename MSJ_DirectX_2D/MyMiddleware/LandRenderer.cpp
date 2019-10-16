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
	// 삭제 
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


// 오버라이딩 함수

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
	// 자동 타일들 그려주기
	if (true == m_bAutoTileDrawCheck)
	{
		// 좌상
		RenderAutoTile(m_AutoLandMap_LU, _ViewProjection);

		// 우상
		RenderAutoTile(m_AutoLandMap_RU, _ViewProjection);

		// 좌하
		RenderAutoTile(m_AutoLandMap_LD, _ViewProjection);
		// 우하
		RenderAutoTile(m_AutoLandMap_RD, _ViewProjection);
	}


	// 여기서 타일들을 그려준다.
	std::map<__int64, TileInfo*>::iterator StartIter = m_LandMap.begin();
	std::map<__int64, TileInfo*>::iterator EndIter = m_LandMap.end();

	Vector2 Index = Vector2(0.0f, 0.0f);
	Vector4 Position = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

	m_Scale.Scale(m_TileSize);		// 타일 사이즈넣어주기 (가로, 세로)

	for (; StartIter != EndIter; ++StartIter)
	{
		TestOutLine.OutLineColor = VCOLOR::Blue;

		Index.i64 = StartIter->first;

		Position.x = m_BaseTilePos.x + Index.ix * m_TileSize.x;
		Position.y = m_BaseTilePos.y + Index.iy * m_TileSize.y;
		Position.z = GetTransform()->World_Position().z;

		m_Scale.vec3 = Position;				// vec3는 이동행렬 'ㅅ'

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


	// 자동 타일들 그려주기
	if (true == m_bAutoTileDrawCheck)
	{
		// 옆 경사 - 좌
		RenderAutoTile(m_AutoLandMap_SlopL, _ViewProjection);

		// 옆 경사 - 우
		RenderAutoTile(m_AutoLandMap_SlopR, _ViewProjection);

		// 하단 경사 - 좌
		RenderAutoTile(m_AutoLandMap_SlopDL, _ViewProjection);

		// 하단 경사 - 우
		RenderAutoTile(m_AutoLandMap_SlopDR, _ViewProjection);


		// 좌
		RenderAutoTile(m_AutoLandMap_L, _ViewProjection);

		// 우
		RenderAutoTile(m_AutoLandMap_R, _ViewProjection);

		// 하
		RenderAutoTile(m_AutoLandMap_D, _ViewProjection);

		// 상
		RenderAutoTile(m_AutoLandMap_U, _ViewProjection);


		// 작은 하
		RenderAutoTile(m_AutoLandMap_SD, _ViewProjection);

		// 얇은 좌
		RenderAutoTile(m_AutoLandMap_SlimL, _ViewProjection);

		// 얇은 좌하
		RenderAutoTile(m_AutoLandMap_SlimLD, _ViewProjection);

		// 얇은 우
		RenderAutoTile(m_AutoLandMap_SlimR, _ViewProjection);

		// 얇은 우하
		RenderAutoTile(m_AutoLandMap_SlimRD, _ViewProjection);

		// 얇은 하
		RenderAutoTile(m_AutoLandMap_SlimD, _ViewProjection);
	}

}

// 자동 타일 렌더
void LandRenderer::RenderAutoTile(std::map<__int64, AutoTileInfo*>& _AutoLandMap, const Matrix& _ViewProjection)
{
	std::map<__int64, AutoTileInfo*>::iterator Auto_StartIter = _AutoLandMap.begin();
	std::map<__int64, AutoTileInfo*>::iterator Auto_EndIter = _AutoLandMap.end();

	Vector2 AutoIndex = Vector2(0.0f, 0.0f);
	AutoIndex.ix = 0;
	AutoIndex.iy = 0;
	Vector4 AutoPosition = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

	// 돌리기 'ㅂ'
	for (; Auto_StartIter != Auto_EndIter; ++Auto_StartIter)
	{
		m_Sprite = ResourceManager<Sprite>::Find(Auto_StartIter->second->SpriteName);

		m_AutoTileSize = Vector4((float)(m_Sprite->GetTexture()->Width()), (float)(m_Sprite->GetTexture()->Height()), 1.0f);

		m_AutoTileScale.Scale(m_AutoTileSize);		// 타일 사이즈넣어주기 (가로, 세로)
		TestOutLine.OutLineColor = VCOLOR::Blue;

		AutoIndex.i64 = Auto_StartIter->first;


		// 검사하기 어떤 타일인지..
		AUTOTILETYPE AutoTileType = Auto_StartIter->second->TILETYPE;

		switch (AutoTileType)
		{
		case LandRenderer::AUTOTILE_UP:
			// 상
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x);
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) + ((m_TileSize.y / 2) + (m_AutoTileSize.y / 2));

			break;
		case LandRenderer::AUTOTILE_LUP:
			// 좌상
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) - ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) + ((m_TileSize.y / 2) + (m_AutoTileSize.y / 2));
			break;
		case LandRenderer::AUTOTILE_RUP:
			//  우상
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) + ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) + ((m_TileSize.y / 2) + (m_AutoTileSize.y / 2));
			break;
		case LandRenderer::AUTOTILE_LEFT:
			// 좌
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) - ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y);
			break;
		case LandRenderer::AUTOTILE_RIGHT:
			// 우
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) + ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y);
			break;
		case LandRenderer::AUTOTILE_DOWN:
			// 하
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x);
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) - ((m_TileSize.y / 2) + (m_AutoTileSize.y / 2));
			break;
		case LandRenderer::AUTOTILE_LDOWN:
			// 좌하
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) - ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) - ((m_TileSize.y / 2) + (m_AutoTileSize.y / 2));
			break;
		case LandRenderer::AUTOTILE_RDOWN:
			// 우하
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) + ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) - ((m_TileSize.y / 2) + (m_AutoTileSize.y / 2));
			break;
		case LandRenderer::AUTOTILE_SMALLDOWN:
			// 작은 하 
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x);
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) - ((m_TileSize.y / 2) + (m_AutoTileSize.y / 2));
			break;
		case LandRenderer::AUTOTILE_SLIM_LEFT:
			// 얇은 좌
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) - ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y);
			break;
		case LandRenderer::AUTOTIEL_SLIM_LDOWN:
			// 얇은 좌하
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) - ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) - ((m_TileSize.y / 2) + (m_AutoTileSize.y / 2));
			break;
		case LandRenderer::AUTOTILE_SLIM_RIGHT:
			// 얇은 우
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) + ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y);
			break;
		case LandRenderer::AUTOTILE_SLIM_RDOWN:
			// 얇은 우하
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) + ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) - ((m_TileSize.y / 2) + (m_AutoTileSize.y / 2));
			break;
		case LandRenderer::AUTOTILE_SLIM_DOWN:
			// 얇은 하
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x);
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) - ((m_TileSize.y / 2) + (m_AutoTileSize.y / 2));
			break;
		case LandRenderer::AUTOTILE_SLOPE_LEFT:
			// 좌 경사
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) - ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) - (m_TileSize.y / 2) + ((m_AutoTileSize.y) / 2);
			break;
		case LandRenderer::AUTOTILE_SLOPE_RIGHT:
			// 우 경사
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) + ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) - (m_TileSize.y / 2) + ((m_AutoTileSize.y) / 2);
			break;
		case LandRenderer::AUTOTILE_SLOPE_DLEFT:
			// 하단 좌 경사
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) - ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) + (m_TileSize.y / 2) - ((m_AutoTileSize.y) / 2);
			break;
		case LandRenderer::AUTOTILE_SLOPE_DRIGHT:
			// 하단 우 경사
			AutoPosition.x = (m_BaseTilePos.x + AutoIndex.ix * m_TileSize.x) + ((m_TileSize.x / 2) + (m_AutoTileSize.x / 2));
			AutoPosition.y = (m_BaseTilePos.y + AutoIndex.iy * m_TileSize.y) + (m_TileSize.y / 2) - ((m_AutoTileSize.y) / 2);
			break;
		default:
			return;
		}
		AutoPosition.z = GetTransform()->World_Position().z;		// 공통

		m_AutoTileScale.vec3 = AutoPosition;				// vec3는 이동행렬 'ㅅ'

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


// 그외

void LandRenderer::SetTileSize(Vector2 _Size)
{
	m_TileSize.x = _Size.x;
	m_TileSize.y = _Size.y;
	m_TileSize.z = 1.0f;
}

void LandRenderer::SetAutoTileDrawCheck(bool _Value)
{
	m_bAutoTileDrawCheck = _Value;

	// 한번만 세팅해주는거.. 마지막에 true로 바꿔주니까
	// 여기서 m_AutoLandMap을 셋팅해준다.

	if (true == m_bAutoTileDrawCheck)
	{
		// 나머지 타일 만들기 얘는 항상 하나의 베이스 타일들을 다 찍은 후에 실행시켜야한다.
		std::map<__int64, TileInfo*>::iterator LandStart = m_LandMap.begin();
		std::map<__int64, TileInfo*>::iterator LandEnd = m_LandMap.end();

		Vector2 CurIndex = Vector2{ 0.0f, 0.0f };
		CurIndex = Vector2_Int(0, 0);

		for (; LandStart != LandEnd; ++LandStart)
		{
			// 여기서 검사후 그려주기
			// 얇은타일인지, 기본베이스 타일인지, 양쪽경사타일인지, 하단경사타일인지 고려하여 그려줘야함
			//CreateRemainderTile(LandStart->first);


			// 경우의수 16가지  2^4   ( 2는 true, false  2가지의 경우, 4는 상,하,좌,우)
			// 고로 최종 경우의수가 16가지가 나온다. 
			// 여기서 판단해서 타일 타입도 알려주자
			// 타일 타입별로 만들어주면됨

			// 상하좌우 판단
			CurIndex = Vector2{ 0.0f, 0.0f };
			CurIndex.i64 = LandStart->first;				// 인덱스 받아오기

			// 상하좌우 자동타일을 그릴수 있는지 탐색
			m_bAutoUP = FindTile(CurIndex, FIND_UP);
			m_bAutoDOWN = FindTile(CurIndex, FIND_DOWN);
			m_bAutoLEFT = FindTile(CurIndex, FIND_LEFT);
			m_bAutoRIGHT = FindTile(CurIndex, FIND_RIGHT);

			// 일반 옆 경사 검사
			m_bAutoDLEFT = FindTile(CurIndex, FIND_DLEFT);
			m_bAutoDRIGHT = FindTile(CurIndex, FIND_DRIGHT);

			// 하단 타일 경사 검사
			m_bAutoULEFT = FindTile(CurIndex, FIND_ULEFT);
			m_bAutoURIGHT = FindTile(CurIndex, FIND_URIGHT);

			// 기본, 경사, 하단 경사      -   얇은타일

			if (true == m_bSlim)
			{

				// 얇은 타일의 경우  - 좌우만 판단하자 

				// 하단타일은 무조건 생성
				CreateRemainderTile(CurIndex.i64, AUTOTILE_SLIM_DOWN);		// 하

				if (false == m_bAutoLEFT && true == m_bAutoRIGHT)
				{
					// 좌 X, 우 O
					// 얇은 - 좌, 좌하, 하 추가 
					CreateRemainderTile(CurIndex.i64, AUTOTILE_SLIM_LEFT);		// 좌
					CreateRemainderTile(CurIndex.i64, AUTOTIEL_SLIM_LDOWN);	// 좌하
				}
				else if (true == m_bAutoLEFT && false == m_bAutoRIGHT)
				{
					// 좌 O, 우 X
					// 얇은 - 우, 우하, 하 추가
					CreateRemainderTile(CurIndex.i64, AUTOTILE_SLIM_RIGHT);	// 우
					CreateRemainderTile(CurIndex.i64, AUTOTILE_SLIM_RDOWN);	// 우하

				}
				else if (false == m_bAutoLEFT && false == m_bAutoRIGHT)
				{
					// 좌 X, 우 X
					// 얇은 - 좌, 좌하, 하 추가
					// 얇은 - 우, 우하, 하 추가
					CreateRemainderTile(CurIndex.i64, AUTOTILE_SLIM_LEFT);		// 좌
					CreateRemainderTile(CurIndex.i64, AUTOTIEL_SLIM_LDOWN);	// 좌하

					CreateRemainderTile(CurIndex.i64, AUTOTILE_SLIM_RIGHT);	// 우
					CreateRemainderTile(CurIndex.i64, AUTOTILE_SLIM_RDOWN);	// 우하

				}
			}
			else
			{
				// 작은타일인지 기본 베이스타일인지 검사하기

				// 작은 타일의 경우
				if (true == m_bSmall)
				{
					// 얘는 하단타일만 그리면됨
					CreateRemainderTile(CurIndex.i64, AUTOTILE_SMALLDOWN);		// 하
				}
				else
				{
					// 기본, 경사, 하단 경사
					// 작은 타일이 아닌경우
					// 그릴위치판단후 그리기
					if (false == m_bAutoUP && true == m_bAutoDOWN)
					{
						// 상단타일 X, 하단타일 O
						// 상단타일 만들기 ( 기본, 경사, 하단경사 해당) , 얇은 타일은 해당사항 X
						int a = 0;
						CreateRemainderTile(CurIndex.i64, AUTOTILE_UP);		// 상단타일
						if (false == m_bAutoLEFT && true == m_bAutoRIGHT)
						{
							// 좌 X, 우 O
							// 좌타일. 좌상단 추가
							if (true == m_bSlop && true == m_bAutoDLEFT)
							{
								// 옆경사 만들기
								CreateRemainderTile(CurIndex.i64, AUTOTILE_SLOPE_LEFT);		// 좌

							}
							else
							{
								// 옆경사로 만들지 않을 경우
								CreateRemainderTile(CurIndex.i64, AUTOTILE_LEFT);		// 좌
								CreateRemainderTile(CurIndex.i64, AUTOTILE_LUP);		// 좌상단
							}
						}
						else if (true == m_bAutoLEFT && false == m_bAutoRIGHT)
						{
							// 좌 O, 우 X
							// 우, 우상단 추가
							if (true == m_bSlop && true == m_bAutoDRIGHT)
							{
								// 옆경사 만들기
								CreateRemainderTile(CurIndex.i64, AUTOTILE_SLOPE_RIGHT);	// 우
							}
							else
							{
								CreateRemainderTile(CurIndex.i64, AUTOTILE_RIGHT);		// 우
								CreateRemainderTile(CurIndex.i64, AUTOTILE_RUP);			// 우상단
							}
						}
						else if (false == m_bAutoLEFT && false == m_bAutoRIGHT)
						{
							// 좌 X, 우 X
							// 좌, 좌상, 우, 우상 추가

							if (true == m_bSlop)
							{
								if (true == m_bAutoDLEFT)
								{
									CreateRemainderTile(CurIndex.i64, AUTOTILE_SLOPE_LEFT);		// 좌
								}
								else
								{
									CreateRemainderTile(CurIndex.i64, AUTOTILE_LEFT);			// 좌
									CreateRemainderTile(CurIndex.i64, AUTOTILE_LUP);				// 좌상단

								}

								if (true == m_bAutoDRIGHT)
								{
									// 옆경사 만들기
									CreateRemainderTile(CurIndex.i64, AUTOTILE_SLOPE_RIGHT);	// 우
								}
								else
								{
									CreateRemainderTile(CurIndex.i64, AUTOTILE_RIGHT);			// 우
									CreateRemainderTile(CurIndex.i64, AUTOTILE_RUP);				// 우상단
								}
							}
							else
							{
								CreateRemainderTile(CurIndex.i64, AUTOTILE_LEFT);			// 좌
								CreateRemainderTile(CurIndex.i64, AUTOTILE_RIGHT);			// 우

								CreateRemainderTile(CurIndex.i64, AUTOTILE_LUP);				// 좌상단
								CreateRemainderTile(CurIndex.i64, AUTOTILE_RUP);				// 우상단
							}
						}
						// 좌 O, 우 O
					}
					else if (true == m_bAutoUP && false == m_bAutoDOWN)
					{
						// 상단타일 O, 하단타일 X
						// 하단타일 생성
						CreateRemainderTile(CurIndex.i64, AUTOTILE_DOWN);		// 생성

						if (false == m_bAutoLEFT && true == m_bAutoRIGHT)
						{
							// 좌 X, 우 O
							// 좌, 좌하단
							if (true == m_bDownSlop && true == m_bAutoULEFT)
							{
								// 좌 하단 경사 타일
								CreateRemainderTile(CurIndex.i64, AUTOTILE_SLOPE_DLEFT);			// 좌
							}
							else
							{
								CreateRemainderTile(CurIndex.i64, AUTOTILE_LEFT);			// 좌
								CreateRemainderTile(CurIndex.i64, AUTOTILE_LDOWN);		// 좌하단
							}
						}
						else if (true == m_bAutoLEFT && false == m_bAutoRIGHT)
						{
							// 좌 O, 우 X
							// 우, 우하단 추가
							if (true == m_bDownSlop && true == m_bAutoURIGHT)
							{
								// 우 하단 경사 타일
								CreateRemainderTile(CurIndex.i64, AUTOTILE_SLOPE_DRIGHT);			// 우
							}
							else
							{
								CreateRemainderTile(CurIndex.i64, AUTOTILE_RIGHT);		// 우
								CreateRemainderTile(CurIndex.i64, AUTOTILE_RDOWN);			// 우하단
							}
						}
						else if (false == m_bAutoLEFT && false == m_bAutoRIGHT)
						{
							// 좌 X, 우 X
							// 좌, 좌하, 우, 우하 추가
							if (true == m_bDownSlop)
							{
								// 하단 경사 타일 - 좌우
								if (true == m_bAutoULEFT)
								{
									CreateRemainderTile(CurIndex.i64, AUTOTILE_SLOPE_DLEFT);			// 좌
								}
								else
								{
									CreateRemainderTile(CurIndex.i64, AUTOTILE_LEFT);				// 좌
									CreateRemainderTile(CurIndex.i64, AUTOTILE_LDOWN);				// 좌하단
								}

								if (true == m_bAutoURIGHT)
								{
									CreateRemainderTile(CurIndex.i64, AUTOTILE_SLOPE_DRIGHT);			// 우
								}
								else
								{
									CreateRemainderTile(CurIndex.i64, AUTOTILE_RIGHT);				// 우
									CreateRemainderTile(CurIndex.i64, AUTOTILE_RDOWN);				// 우하단
								}
							}
							else
							{
								CreateRemainderTile(CurIndex.i64, AUTOTILE_LEFT);				// 좌
								CreateRemainderTile(CurIndex.i64, AUTOTILE_RIGHT);				// 우

								CreateRemainderTile(CurIndex.i64, AUTOTILE_LDOWN);				// 좌하단
								CreateRemainderTile(CurIndex.i64, AUTOTILE_RDOWN);				// 우하단
							}
						}
						// 좌 O, 우 O
					}
					else if (true == m_bAutoUP && true == m_bAutoDOWN)
					{
						// 상단타일 O, 하단타일 O
						// 다 있으니까 상하단 타일 안만듬

						if (false == m_bAutoLEFT && true == m_bAutoRIGHT)
						{
							// 좌 X, 우 O
							// 좌타일
							CreateRemainderTile(CurIndex.i64, AUTOTILE_LEFT);		// 좌

						}
						else if (true == m_bAutoLEFT && false == m_bAutoRIGHT)
						{
							// 좌 O, 우 X
							CreateRemainderTile(CurIndex.i64, AUTOTILE_RIGHT);		// 우
						}
						else if (false == m_bAutoLEFT && false == m_bAutoRIGHT)
						{
							// 좌 X, 우 X
							CreateRemainderTile(CurIndex.i64, AUTOTILE_LEFT);				// 좌
							CreateRemainderTile(CurIndex.i64, AUTOTILE_RIGHT);			// 우
						}
						// 좌 O, 우 O
					}
					else if (false == m_bAutoUP && false == m_bAutoDOWN)
					{
						// 상단타일 X, 하단타일 X
						// 상단, 하단 생성
						CreateRemainderTile(CurIndex.i64, AUTOTILE_UP);			// 상
						CreateRemainderTile(CurIndex.i64, AUTOTILE_DOWN);		// 하

						if (false == m_bAutoLEFT && true == m_bAutoRIGHT)
						{
							// 좌 X, 우 O
							// 좌, 좌상단, 좌하단
							CreateRemainderTile(CurIndex.i64, AUTOTILE_LEFT);	// 좌
							CreateRemainderTile(CurIndex.i64, AUTOTILE_LUP);	// 좌상단
							CreateRemainderTile(CurIndex.i64, AUTOTILE_LDOWN);	// 좌하단
						}
						else if (true == m_bAutoLEFT && false == m_bAutoRIGHT)
						{
							// 좌 O, 우 X
							// 우, 우상단, 우하단
							CreateRemainderTile(CurIndex.i64, AUTOTILE_RIGHT);	// 우
							CreateRemainderTile(CurIndex.i64, AUTOTILE_RUP);	// 우상단
							CreateRemainderTile(CurIndex.i64, AUTOTILE_RDOWN);	// 우하단
						}
						else if (false == m_bAutoLEFT && false == m_bAutoRIGHT)
						{
							// 좌 X, 우 X
							// 좌, 좌상단, 좌하단, 우, 우상단, 우하단
							CreateRemainderTile(CurIndex.i64, AUTOTILE_LEFT);	// 좌
							CreateRemainderTile(CurIndex.i64, AUTOTILE_LUP);	// 좌상단
							CreateRemainderTile(CurIndex.i64, AUTOTILE_LDOWN);	// 좌하단

							CreateRemainderTile(CurIndex.i64, AUTOTILE_RIGHT);	// 우
							CreateRemainderTile(CurIndex.i64, AUTOTILE_RUP);	// 우상단
							CreateRemainderTile(CurIndex.i64, AUTOTILE_RDOWN);	// 우하단
						}
						// 좌 O, 우 O
					}

				}
			}
		}
	}
}

bool LandRenderer::FindTile(Vector2 _Index, FINDTILE _Dir)
{
	Vector2 CurIndex = Vector2{ 0.0f, 0.0f };
	CurIndex.i64 = _Index.i64;				// 인덱스 받아오기

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
		// 자동타일을 설치할수 있다면
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
	// 현재는 이미지이름이 동일해선 안된다..
	// 처음들어온애로 지정되어있음.
	// 나중에 바꿔주기 ㅇㅅㅇ
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

// 내가 찍은 타일 그리기

void LandRenderer::CreateTile(int _x, int _y)
{
	// 맨처음의 타일인지 체크해줘야함 
	if (true == m_bFirstTileCheck)
	{
		// 처음의 타일이라면 (기준점)
		BaseX = _x;
		BaseY = _y;
	}

	int TileIndex_X = _x - BaseX;
	int TileIndex_Y = _y - BaseY;

	__int64 PosIndex = Vector2(TileIndex_X, TileIndex_Y).i64;

	// 이미 찍힌곳인지 확인
	std::map<__int64, TileInfo*>::iterator StartIter = m_LandMap.begin();
	std::map<__int64, TileInfo*>::iterator EndIter = m_LandMap.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (PosIndex == StartIter->first)
		{
			return;
		}
	}

	// 찍힌 위치가 아니라면 생성
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
		// 첫번째 타일이라면 (기준타일)
		m_BaseTilePos = _Pos;

		// 첫번째타일은 무조건 0,0이다.
		TileIndex_X = 0.0f;
		TileIndex_Y = 0.0f;
	}
	else
	{
		// 두번째타일부터..
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


	// 인덱스 소수 -> 정수로 바꿔야한다.
	int iXIndex = (int)TileIndex_X;
	int iYIndex = (int)TileIndex_Y;

	CreateTile(iXIndex, iYIndex);

	if (true == m_bFirstTileCheck)
	{
		m_bFirstTileCheck = false;		// 처음타일을 찍었다면 false
	}
}


// 자동 타일 만들기
void LandRenderer::CreateRemainderTile(__int64 _Index, AUTOTILETYPE _AutoType)
{
	// 받아오는 인덱스값은
	// 내가 설치해야하는 기준타일의 인덱스임 'ㅅ'

	Vector2 GenTileIndex = Vector2{ 0.0f, 0.0f };     // 인덱스 정보(베이스)
	GenTileIndex.i64 = _Index;

	AutoTileInfo* NewAutoTileInfo = new AutoTileInfo();
	std::wstring TileImageName = L"";
	int RandomValue = -1;

	// 그릴 타일들 탐색
	switch (_AutoType)
	{
	case LandRenderer::AUTOTILE_UP:
		// 상
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_UpTile");
		break;
	case LandRenderer::AUTOTILE_LUP:
		// 좌상
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_SUpLHalfTile");
		break;
	case LandRenderer::AUTOTILE_RUP:
		// 우상
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_SUpRHalfTile");
		break;
	case LandRenderer::AUTOTILE_LEFT:
		// 좌
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_LeftTile");
		break;
	case LandRenderer::AUTOTILE_RIGHT:
		// 우
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_RightTile");
		break;
	case LandRenderer::AUTOTILE_DOWN:
		// 하
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_DownTile");
		break;
	case LandRenderer::AUTOTILE_LDOWN:
		// 좌하
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_SDownLHalfTile");
		break;
	case LandRenderer::AUTOTILE_RDOWN:
		// 우하
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_SDownRHalfTile");
		break;
	case LandRenderer::AUTOTILE_SMALLDOWN:
		// 작은타일
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_SDownTile");
		break;
	case LandRenderer::AUTOTILE_SLIM_LEFT:
		// 얇은 좌
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_SUpLHalfTile");
		break;
	case LandRenderer::AUTOTIEL_SLIM_LDOWN:
		// 얇은 좌하 
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_SDownLHalfTile");
		break;
	case LandRenderer::AUTOTILE_SLIM_RIGHT:
		// 얇은 우
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_SUpRHalfTile");
		break;
	case LandRenderer::AUTOTILE_SLIM_RDOWN:
		// 얇은 우하
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_SDownRHalfTile");
		break;
	case LandRenderer::AUTOTILE_SLIM_DOWN:
		// 얇은 하
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_DownTile");
		break;
	case LandRenderer::AUTOTILE_SLOPE_LEFT:
		// 좌 경사
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_LSlopTile");
		break;
	case LandRenderer::AUTOTILE_SLOPE_RIGHT:
		// 우 경사
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_RSlopTile");
		break;
	case LandRenderer::AUTOTILE_SLOPE_DLEFT:
		// 하단 좌경사
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_LDownTile");
		break;
	case LandRenderer::AUTOTILE_SLOPE_DRIGHT:
		// 하단 우경사
		DrawTileMap = m_CurAllAutoTile.find(L"Auto_RDownTile");
		break;
	default:
		// 나머지 (안만들어줘야한다.)
		delete NewAutoTileInfo;
		return;
	}


	// 예외처리 -> 못찾았다면..
	if (m_CurAllAutoTile.end() == DrawTileMap)
	{
		delete NewAutoTileInfo;
		return;
	}

	DrawCurTiles = DrawTileMap->second;							// 그릴수있는 타일들(vector관리) 가져오기
	RandomValue = rand() % DrawCurTiles.size();					// 랜덤
	NewAutoTileInfo->SpriteName = DrawCurTiles[RandomValue];	// 그릴 타일이름 설정
	NewAutoTileInfo->TILETYPE = _AutoType;						// 타일 타입


	m_AutoTileInfoList.push_back(NewAutoTileInfo);				// 그릴타일정보 넣어주기 

	// 해당 자동타일들이 모아진곳에 정보 넣어주기 

	switch (_AutoType)
	{
	case LandRenderer::AUTOTILE_UP:	// 상
		m_AutoLandMap_U.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_LUP:	// 좌상
		m_AutoLandMap_LU.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_RUP:	// 우상
		m_AutoLandMap_RU.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_LEFT:	// 좌
		m_AutoLandMap_L.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_RIGHT:	// 우
		m_AutoLandMap_R.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_DOWN:	// 하
		m_AutoLandMap_D.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_LDOWN:	// 좌하
		m_AutoLandMap_LD.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_RDOWN:	// 우하
		m_AutoLandMap_RD.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_SMALLDOWN:	// 작은하 
		m_AutoLandMap_SD.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_SLIM_LEFT:	// 얇은 좌
		m_AutoLandMap_SlimL.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTIEL_SLIM_LDOWN:	// 얇은 좌하
		m_AutoLandMap_SlimLD.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_SLIM_RIGHT:	// 얇은 우
		m_AutoLandMap_SlimR.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_SLIM_RDOWN:	// 얇은 우하
		m_AutoLandMap_SlimRD.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_SLIM_DOWN:		// 얇은 하
		m_AutoLandMap_SlimD.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_SLOPE_LEFT:		// 좌 경사
		m_AutoLandMap_SlopL.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_SLOPE_RIGHT:	// 우 경사
		m_AutoLandMap_SlopR.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_SLOPE_DLEFT:	// 하단 좌 경사
		m_AutoLandMap_SlopDL.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	case LandRenderer::AUTOTILE_SLOPE_DRIGHT:	// 하단 우 경사
		m_AutoLandMap_SlopDR.insert(std::map<__int64, AutoTileInfo*>::value_type(GenTileIndex.i64, NewAutoTileInfo));
		break;
	}
}

// 지우기
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
	// 자동타일
	SaveAutoTile(m_AutoLandMap_LU, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_RU, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_LD, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_RD, _Pixel, _SceneSizeX, _SceneSizeY);


	// 기본타일
	SaveBaseTile(m_LandMap, _Pixel, _SceneSizeX, _SceneSizeY);

	 // 자동타일
	// 경사
	SaveAutoTile(m_AutoLandMap_SlopL, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_SlopR, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_SlopDL, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_SlopDR, _Pixel, _SceneSizeX, _SceneSizeY);

	// 상하좌우
	SaveAutoTile(m_AutoLandMap_L, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_R, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_D, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_U, _Pixel, _SceneSizeX, _SceneSizeY);

	// 기타
	SaveAutoTile(m_AutoLandMap_SD, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_SlimL, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_SlimLD, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_SlimR, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_SlimRD, _Pixel, _SceneSizeX, _SceneSizeY);
	SaveAutoTile(m_AutoLandMap_SlimD, _Pixel, _SceneSizeX, _SceneSizeY);


}


// 기본베이스 타일
void LandRenderer::SaveBaseTile(std::map<__int64, TileInfo*>& _AutoLandMap, int* _Pixel, size_t _SceneSizeX, size_t _SceneSizeY)
{
	// 기본타일
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
		// 가져왔으면 이미지 그리기 (픽셀)
		// 위치 계산
		// 일단 기본 타일만 찍어보자 


		// 해당 스프라이트 사이즈 (가로,세로)
		size_t ImageWidth = m_Sprite->GetTexture()->Width();
		size_t ImageHeight = m_Sprite->GetTexture()->Height();


		// 인덱스 값
		Vector2 Index = Vector2{ 0.0f, 0.0f };
		Index.i64 = BaseLandStart->first;


		// 이미지 실제 중앙의 포지션
		int ImageCenterPosX = ((int)m_BaseTilePos.x + (Index.ix * (int)ImageWidth));
		int ImageCenterPosY = ((int)m_BaseTilePos.y + (Index.iy * (int)ImageHeight));

		// 중앙 포지션의 픽셀
		int CenterPixelX = ((int)_SceneSizeX / 2) + ImageCenterPosX;
		int CenterPixelY = ((int)_SceneSizeY / 2) - ImageCenterPosY;

		// 순서대로 찍힐 픽셀들의 첫픽셀의 포지션
		int FirstPixelX = CenterPixelX - (int)(ImageWidth / 2);
		int FirstPixelY = CenterPixelY - (int)(ImageHeight / 2);




		for (size_t y = 0; y < ImageHeight; ++y)
		{
			for (size_t x = 0; x < ImageWidth; ++x)
			{
				// 한픽셀씩 가져와서 월드위치 좌표로 찍은후 저장하기
				unsigned int XValue = (unsigned int)x;
				unsigned int YValue = (unsigned int)y;

				uint8_t* PixelRGBA = m_Sprite->GetTexture()->ImageOriginGetPixel(XValue, YValue);		// 해당 이미지의 픽셀값 받아오기(ARGB)


				uint8_t r = PixelRGBA[0];		// 블루
				uint8_t g = PixelRGBA[1];		// 그린
				uint8_t b = PixelRGBA[2];		// 레드
				uint8_t a = PixelRGBA[3];		// 알파

				if (a <= 0)
				{
					continue;
				}

				// 월드 계산후에 픽셀값 넣어주기 (위에서 다했음)
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
					PixelRGBA[0] = r;			   // 블루
					PixelRGBA[1] = g;				// 그린 
					PixelRGBA[2] = b;				// 레드6
					PixelRGBA[3] = a;				// 알파

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





// 자동타일
void LandRenderer::SaveAutoTile(std::map<__int64, AutoTileInfo*>& _AutoLandMap, int* _Pixel, size_t _SceneSizeX, size_t _SceneSizeY)
{
	// 자동 타일
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
		// 가져왔으면 이미지 그리기 (픽셀)
		// 위치 계산
		// 일단 기본 타일만 찍어보자 

		DXGI_FORMAT SpriteFormat = m_Sprite->GetTexture()->Format();


		// 해당 스프라이트 사이즈 (가로,세로)
		size_t ImageWidth = m_Sprite->GetTexture()->Width();
		size_t ImageHeight = m_Sprite->GetTexture()->Height();


		// 인덱스 값
		Vector2 Index = Vector2{ 0.0f, 0.0f };
		Index.i64 = AutoStart->first;


		// 이미지 실제 중앙의 포지션
		int ImageCenterPosX = 0;
		int ImageCenterPosY = 0;


		// 검사하기 어떤 타일인지..
		AUTOTILETYPE AutoTileType = AutoStart->second->TILETYPE;

		switch (AutoTileType)
		{
		case LandRenderer::AUTOTILE_UP:
			// 상
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x);
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) + (((int)m_TileSize.y / 2) + ((int)ImageHeight / 2));

			break;
		case LandRenderer::AUTOTILE_LUP:
			// 좌상
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) - (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) + (((int)m_TileSize.y / 2) + ((int)ImageHeight / 2));
			break;
		case LandRenderer::AUTOTILE_RUP:
			//  우상
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) + (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) + (((int)m_TileSize.y / 2) + ((int)ImageHeight / 2));
			break;
		case LandRenderer::AUTOTILE_LEFT:
			// 좌
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) - (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y);
			break;
		case LandRenderer::AUTOTILE_RIGHT:
			// 우
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) + (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y);
			break;
		case LandRenderer::AUTOTILE_DOWN:
			// 하
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x);
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) - (((int)m_TileSize.y / 2) + ((int)ImageHeight / 2));
			break;
		case LandRenderer::AUTOTILE_LDOWN:
			// 좌하
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) - (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) - (((int)m_TileSize.y / 2) + ((int)ImageHeight / 2));
			break;
		case LandRenderer::AUTOTILE_RDOWN:
			// 우하
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) + (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) - (((int)m_TileSize.y / 2) + ((int)ImageHeight / 2));
			break;
		case LandRenderer::AUTOTILE_SMALLDOWN:
			// 작은 하 
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x);
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) - (((int)m_TileSize.y / 2) + ((int)ImageHeight / 2));
			break;
		case LandRenderer::AUTOTILE_SLIM_LEFT:
			// 얇은 좌
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) - (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y);
			break;
		case LandRenderer::AUTOTIEL_SLIM_LDOWN:
			// 얇은 좌하
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) - (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) - (((int)m_TileSize.y / 2) + ((int)ImageHeight / 2));
			break;
		case LandRenderer::AUTOTILE_SLIM_RIGHT:
			// 얇은 우
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) + (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y);
			break;
		case LandRenderer::AUTOTILE_SLIM_RDOWN:
			// 얇은 우하
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) + (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) - (((int)m_TileSize.y / 2) + ((int)ImageHeight / 2));
			break;
		case LandRenderer::AUTOTILE_SLIM_DOWN:
			// 얇은 하
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x);
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) - (((int)m_TileSize.y / 2) + ((int)ImageHeight / 2));
			break;
		case LandRenderer::AUTOTILE_SLOPE_LEFT:
			// 좌 경사
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) - (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) - ((int)m_TileSize.y / 2) + (((int)ImageHeight) / 2);
			break;
		case LandRenderer::AUTOTILE_SLOPE_RIGHT:
			// 우 경사
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) + (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) - ((int)m_TileSize.y / 2) + (((int)ImageHeight) / 2);
			break;
		case LandRenderer::AUTOTILE_SLOPE_DLEFT:
			// 하단 좌 경사
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) - (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) + ((int)m_TileSize.y / 2) - (((int)ImageHeight) / 2);
			break;
		case LandRenderer::AUTOTILE_SLOPE_DRIGHT:
			// 하단 우 경사
			ImageCenterPosX = ((int)m_BaseTilePos.x + Index.ix * (int)m_TileSize.x) + (((int)m_TileSize.x / 2) + ((int)ImageWidth / 2));
			ImageCenterPosY = ((int)m_BaseTilePos.y + Index.iy * (int)m_TileSize.y) + ((int)m_TileSize.y / 2) - (((int)ImageHeight) / 2);
			break;
		default:
			return;
		}



		// 중앙 포지션의 픽셀
		int CenterPixelX = ((int)_SceneSizeX / 2) + ImageCenterPosX;
		int CenterPixelY = ((int)_SceneSizeY / 2) - ImageCenterPosY;

		// 순서대로 찍힐 픽셀들의 첫픽셀의 포지션
		int FirstPixelX = CenterPixelX - (int)(ImageWidth / 2);
		int FirstPixelY = CenterPixelY - (int)(ImageHeight / 2);




		for (size_t y = 0; y < ImageHeight; ++y)
		{
			for (size_t x = 0; x < ImageWidth; ++x)
			{
				int CurPixel = 0x00000000;

				// 한픽셀씩 가져와서 월드위치 좌표로 찍은후 저장하기
				uint8_t* PixelRGBA = m_Sprite->GetTexture()->ImageOriginGetPixel((unsigned int)x, (unsigned int)y);		// 해당 이미지의 픽셀값 받아오기(ARGB)


				uint8_t r = PixelRGBA[0];		
				uint8_t g = PixelRGBA[1];		
				uint8_t b = PixelRGBA[2];		
				uint8_t a = PixelRGBA[3];		

				if (a <= 0)
				{
					continue;
				}


						// ARGB
				PixelRGBA[0] = r;			   // 블루
				PixelRGBA[1] = g;				// 그린 
				PixelRGBA[2] = b;				// 레드6
				PixelRGBA[3] = a;				// 알파

				// 월드 계산후에 픽셀값 넣어주기 (위에서 다했음)
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
