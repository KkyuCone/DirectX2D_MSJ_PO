#pragma once
#include "Renderer.h"
#include <map>
#include <Device.h>
#include <Texture.h>


class VertexShader;
class PixelShader;
class Mesh;
class Blend;
class Sprite;
class LandRenderer : public Renderer
{
	//  이녀석은 무슨 타일맵이냐면
	//  포지션이 바뀌는 타일맵이다.

private:
	enum FINDTILE
	{
		FIND_UP = 0,
		FIND_DOWN,
		FIND_LEFT,
		FIND_RIGHT,
		FIND_DLEFT,
		FIND_DRIGHT,
		FIND_ULEFT,
		FIND_URIGHT,
	};

	// 자동타일 타입
	enum AUTOTILETYPE
	{
		AUTOTILE_NONE = 0,
		AUTOTILE_UP,			// 상
		AUTOTILE_LUP,			// 좌상
		AUTOTILE_RUP,			// 우상
		AUTOTILE_LEFT,			// 좌
		AUTOTILE_RIGHT,			// 우
		AUTOTILE_DOWN,			// 하
		AUTOTILE_LDOWN,			// 좌하
		AUTOTILE_RDOWN,			// 우하
		AUTOTILE_SMALLDOWN,			// 작은 하
		AUTOTILE_SLIM_LEFT,		// 얇은 좌
		AUTOTIEL_SLIM_LDOWN,	// 얇은 좌하 
		AUTOTILE_SLIM_RIGHT,	// 얇은 우
		AUTOTILE_SLIM_RDOWN,	// 얇은 우하
		AUTOTILE_SLIM_DOWN,		// 얇은 하
		AUTOTILE_SLOPE_LEFT,	// 좌 경사
		AUTOTILE_SLOPE_RIGHT,	// 우 경사
		AUTOTILE_SLOPE_DLEFT,	// 하단 좌 경사
		AUTOTILE_SLOPE_DRIGHT,	// 하단 우 경사
		AUTOTILETYPEMAX,
	};

public:
	friend GameObject;

public:
	class TileInfo
	{
	public:
		std::wstring SpriteName;
		//AUTOTILETYPE TILETYPE = AUTOTILE_NONE;
	};

	class AutoTileInfo
	{
	public:
		std::wstring SpriteName;
		AUTOTILETYPE TILETYPE = AUTOTILE_NONE;
	};

private:
	My_PTR<VertexShader> m_VertexShader;
	My_PTR<PixelShader> m_PixelShader;
	My_PTR<Mesh> m_Mesh;
	My_PTR<Sprite> m_Sprite;
	My_PTR<Blend> m_Blend;

	TestBuffer TestOutLine;


	std::wstring SpriteFileName;			// 타일 이름

public:
	bool Init(int _RenderGroup = 0) override;
	void Render(const Matrix& _ViewProjection) override;

	///////////////////////////////////////////////////////////////////

private:
	Vector2 m_BaseTilePos;		// 처음 타일 포지션 (하나의 땅덩어리중 첫번째)
	Vector4 m_TileSize;
	Vector4 m_AutoTileSize;
	Matrix m_Scale;
	Matrix m_AutoTileScale;

	bool m_bFirstTileCheck;
	bool m_bAutoTileDrawCheck;			// 자동 타일을 그릴 여부

	// 아래 초기값들은 전부 false나중에 자동타일을 그릴때 필요함.
	bool m_bSlim;
	bool m_bSlop;
	bool m_bDownSlop;

	bool m_bSmall;		// 작은타일인지.. (얘는 얇은 타일 그런게 아님..말그대로 그냥 진짜 작은 타일임)

	// 자동 타일 검사할 변수
	bool m_bAutoUP;
	bool m_bAutoDOWN;
	bool m_bAutoLEFT;
	bool m_bAutoRIGHT;

	// 자동 타일 - 일반 경사 타일 검사
	bool m_bAutoDLEFT;		// 좌 하단 -  좌경사
	bool m_bAutoDRIGHT;		// 우하단  - 우 경사 담당

	// 자동 타일 - 하단 경사 타일 검사
	bool m_bAutoULEFT;
	bool m_bAutoURIGHT;

	// 찍혔을때 최초 타일의 인덱스
	int BaseX;
	int BaseY;

	//std::set<__int64> m_LandSet;
	std::map<__int64, TileInfo*> m_LandMap;				// 기본타일들(내가찍은타일들)

	std::map<__int64, AutoTileInfo*> m_AutoLandMap_U;		// 자동 - 상
	std::map<__int64, AutoTileInfo*> m_AutoLandMap_D;		// 자동 - 하
	std::map<__int64, AutoTileInfo*> m_AutoLandMap_L;		// 자동 - 좌
	std::map<__int64, AutoTileInfo*> m_AutoLandMap_R;		// 자동 - 우

	std::map<__int64, AutoTileInfo*> m_AutoLandMap_LU;		// 자동 - 좌상
	std::map<__int64, AutoTileInfo*> m_AutoLandMap_RU;		// 자동 - 우상
	std::map<__int64, AutoTileInfo*> m_AutoLandMap_LD;		// 자동 - 좌하
	std::map<__int64, AutoTileInfo*> m_AutoLandMap_RD;		// 자동 - 우하

	std::map<__int64, AutoTileInfo*> m_AutoLandMap_SD;		// 자동 - 작은 하

	// 얇은 좌, 좌하, 우 , 우하, 하
	std::map<__int64, AutoTileInfo*> m_AutoLandMap_SlimL;	// 자동 - 얇은 좌
	std::map<__int64, AutoTileInfo*> m_AutoLandMap_SlimLD;	// 자동 - 얇은 좌하
	std::map<__int64, AutoTileInfo*> m_AutoLandMap_SlimR;	// 자동 - 얇은 우
	std::map<__int64, AutoTileInfo*> m_AutoLandMap_SlimRD;	// 자동 - 얇은 우하
	std::map<__int64, AutoTileInfo*> m_AutoLandMap_SlimD;	// 자동 - 얇은 하

	// 경사타일들
	// 일반 경사, 하단 경사
	std::map<__int64, AutoTileInfo*> m_AutoLandMap_SlopL;
	std::map<__int64, AutoTileInfo*> m_AutoLandMap_SlopR;
	std::map<__int64, AutoTileInfo*> m_AutoLandMap_SlopDL;
	std::map<__int64, AutoTileInfo*> m_AutoLandMap_SlopDR;



	std::list<TileInfo*> m_TileInfoList;
	std::list<AutoTileInfo*> m_AutoTileInfoList;

	std::unordered_map<std::wstring, std::vector<std::wstring>> m_CurAllAutoTile;

	std::unordered_map<std::wstring, std::vector<std::wstring>>::iterator DrawTileMap;
	std::vector<std::wstring> DrawCurTiles;

private:
	bool FindTile(Vector2 _Index, FINDTILE _Dir);
	void RenderAutoTile(std::map<__int64, AutoTileInfo*>& _AutoLandMap, const Matrix& _ViewProjection);

	void SaveAutoTile(std::map<__int64, AutoTileInfo*>& _AutoLandMap, int* _Pixel, size_t _SceneSizeX, size_t _SceneSizeY);
	void SaveBaseTile(std::map<__int64, TileInfo*>& _AutoLandMap, int* _Pixel, size_t _SceneSizeX, size_t _SceneSizeY);

public:
	void DrawLandMap(int* _PixelValue, size_t _SceneSizeX, size_t _SceneSizeY);

public:
	void SetTileSize(Vector2 _Size);		// 타일사이즈 (가로, 세로) 세팅
	void SetTileImage(const wchar_t* _Name);
	void SetAutoTileDrawCheck(bool _Value);		// 자동 타일 그릴건지 여부
	void SetAutoTileAllMap(std::unordered_map<std::wstring, std::vector<std::wstring>>& _CurAllAutoTile);		// 자동타일 전체 묶음..
	void SetAutoSlim(bool _Value);
	void SetAutoSmall(bool _Value);
	void SetAutoSlop(bool _Value);
	void SetAutoDownSlop(bool _Value);


	// 내가 찍은 타일 만들기
	void CreateTile(int _x, int _y);
	void CreateTile(Vector2 _Pos);

	// 자동 타일 만들기
	void CreateRemainderTile(__int64 _Index, AUTOTILETYPE _AutoType);

	void Clear();							// 지우기


	//
	Sprite* GetSprite();

	//bool GetCom

	//void SavePNG(std::wstring _FileName);				// 저장하기


public:
	DirectX::Image* DrawSavePNG(DirectX::Image* _EmptyImage);

	//void SavePNG(std::wstring _FileName);




public:
	LandRenderer();
	~LandRenderer();
};

