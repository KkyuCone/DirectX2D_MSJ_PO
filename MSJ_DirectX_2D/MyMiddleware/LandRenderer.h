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
	//  �̳༮�� ���� Ÿ�ϸ��̳ĸ�
	//  �������� �ٲ�� Ÿ�ϸ��̴�.

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

	// �ڵ�Ÿ�� Ÿ��
	enum AUTOTILETYPE
	{
		AUTOTILE_NONE = 0,
		AUTOTILE_UP,			// ��
		AUTOTILE_LUP,			// �»�
		AUTOTILE_RUP,			// ���
		AUTOTILE_LEFT,			// ��
		AUTOTILE_RIGHT,			// ��
		AUTOTILE_DOWN,			// ��
		AUTOTILE_LDOWN,			// ����
		AUTOTILE_RDOWN,			// ����
		AUTOTILE_SMALLDOWN,			// ���� ��
		AUTOTILE_SLIM_LEFT,		// ���� ��
		AUTOTIEL_SLIM_LDOWN,	// ���� ���� 
		AUTOTILE_SLIM_RIGHT,	// ���� ��
		AUTOTILE_SLIM_RDOWN,	// ���� ����
		AUTOTILE_SLIM_DOWN,		// ���� ��
		AUTOTILE_SLOPE_LEFT,	// �� ���
		AUTOTILE_SLOPE_RIGHT,	// �� ���
		AUTOTILE_SLOPE_DLEFT,	// �ϴ� �� ���
		AUTOTILE_SLOPE_DRIGHT,	// �ϴ� �� ���
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


	std::wstring SpriteFileName;			// Ÿ�� �̸�

public:
	bool Init(int _RenderGroup = 0) override;
	void Render(const Matrix& _ViewProjection) override;

	///////////////////////////////////////////////////////////////////

private:
	Vector2 m_BaseTilePos;		// ó�� Ÿ�� ������ (�ϳ��� ������� ù��°)
	Vector4 m_TileSize;
	Vector4 m_AutoTileSize;
	Matrix m_Scale;
	Matrix m_AutoTileScale;

	bool m_bFirstTileCheck;
	bool m_bAutoTileDrawCheck;			// �ڵ� Ÿ���� �׸� ����

	// �Ʒ� �ʱⰪ���� ���� false���߿� �ڵ�Ÿ���� �׸��� �ʿ���.
	bool m_bSlim;
	bool m_bSlop;
	bool m_bDownSlop;

	bool m_bSmall;		// ����Ÿ������.. (��� ���� Ÿ�� �׷��� �ƴ�..���״�� �׳� ��¥ ���� Ÿ����)

	// �ڵ� Ÿ�� �˻��� ����
	bool m_bAutoUP;
	bool m_bAutoDOWN;
	bool m_bAutoLEFT;
	bool m_bAutoRIGHT;

	// �ڵ� Ÿ�� - �Ϲ� ��� Ÿ�� �˻�
	bool m_bAutoDLEFT;		// �� �ϴ� -  �°��
	bool m_bAutoDRIGHT;		// ���ϴ�  - �� ��� ���

	// �ڵ� Ÿ�� - �ϴ� ��� Ÿ�� �˻�
	bool m_bAutoULEFT;
	bool m_bAutoURIGHT;

	// �������� ���� Ÿ���� �ε���
	int BaseX;
	int BaseY;

	//std::set<__int64> m_LandSet;
	std::map<__int64, TileInfo*> m_LandMap;				// �⺻Ÿ�ϵ�(��������Ÿ�ϵ�)

	std::map<__int64, AutoTileInfo*> m_AutoLandMap_U;		// �ڵ� - ��
	std::map<__int64, AutoTileInfo*> m_AutoLandMap_D;		// �ڵ� - ��
	std::map<__int64, AutoTileInfo*> m_AutoLandMap_L;		// �ڵ� - ��
	std::map<__int64, AutoTileInfo*> m_AutoLandMap_R;		// �ڵ� - ��

	std::map<__int64, AutoTileInfo*> m_AutoLandMap_LU;		// �ڵ� - �»�
	std::map<__int64, AutoTileInfo*> m_AutoLandMap_RU;		// �ڵ� - ���
	std::map<__int64, AutoTileInfo*> m_AutoLandMap_LD;		// �ڵ� - ����
	std::map<__int64, AutoTileInfo*> m_AutoLandMap_RD;		// �ڵ� - ����

	std::map<__int64, AutoTileInfo*> m_AutoLandMap_SD;		// �ڵ� - ���� ��

	// ���� ��, ����, �� , ����, ��
	std::map<__int64, AutoTileInfo*> m_AutoLandMap_SlimL;	// �ڵ� - ���� ��
	std::map<__int64, AutoTileInfo*> m_AutoLandMap_SlimLD;	// �ڵ� - ���� ����
	std::map<__int64, AutoTileInfo*> m_AutoLandMap_SlimR;	// �ڵ� - ���� ��
	std::map<__int64, AutoTileInfo*> m_AutoLandMap_SlimRD;	// �ڵ� - ���� ����
	std::map<__int64, AutoTileInfo*> m_AutoLandMap_SlimD;	// �ڵ� - ���� ��

	// ���Ÿ�ϵ�
	// �Ϲ� ���, �ϴ� ���
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
	void SetTileSize(Vector2 _Size);		// Ÿ�ϻ����� (����, ����) ����
	void SetTileImage(const wchar_t* _Name);
	void SetAutoTileDrawCheck(bool _Value);		// �ڵ� Ÿ�� �׸����� ����
	void SetAutoTileAllMap(std::unordered_map<std::wstring, std::vector<std::wstring>>& _CurAllAutoTile);		// �ڵ�Ÿ�� ��ü ����..
	void SetAutoSlim(bool _Value);
	void SetAutoSmall(bool _Value);
	void SetAutoSlop(bool _Value);
	void SetAutoDownSlop(bool _Value);


	// ���� ���� Ÿ�� �����
	void CreateTile(int _x, int _y);
	void CreateTile(Vector2 _Pos);

	// �ڵ� Ÿ�� �����
	void CreateRemainderTile(__int64 _Index, AUTOTILETYPE _AutoType);

	void Clear();							// �����


	//
	Sprite* GetSprite();

	//bool GetCom

	//void SavePNG(std::wstring _FileName);				// �����ϱ�


public:
	DirectX::Image* DrawSavePNG(DirectX::Image* _EmptyImage);

	//void SavePNG(std::wstring _FileName);




public:
	LandRenderer();
	~LandRenderer();
};

