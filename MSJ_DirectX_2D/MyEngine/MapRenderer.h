#pragma once
#include "Renderer.h"
#include <map>

class VertexShader;
class PixelShader;
class Mesh;
class Blend;
class Sprite;
class MapRenderer : public Renderer
{
public:
	friend GameObject;

private:
	My_PTR<VertexShader> m_VertexShader;
	My_PTR<PixelShader> m_PixelShader;
	My_PTR<Mesh> m_Mesh;
	My_PTR<Sprite> m_Sprite;
	My_PTR<Blend> m_Blend;

	TestBuffer TestOutLine;

	//////////////////////////////////////////////////////////////

	// 내가 추가한것
public:
	std::vector<std::wstring> ALLFolderPath;


private:
	int BaseX;
	int BaseY;

	int FirstTile;

	Vector2 m_BaseTilePos;

	/////////////////////////////////////////////////////////

public:
	// 타일 이미지 정보들 
	class MapTileData
	{
	//public:
	//	Vector2 m_TilePos;		// 타일 위치
	//	Vector2 m_Size;			// 타일 사이즈 (x,y)
	//	CString m_AllPath;
	//	CString m_FolderPath;
	//	CString m_FileName;
	};

	class OwnershipTileData
	{
	public:
		Vector2 m_Index;
		std::wstring m_FullFilePath;
	};

private:
	class TileInfo : public ReferenceBase
	{
	public:
		std::wstring m_FullFilePath;
		MapTileData* _Data;
	};

private:
	std::map<__int64, My_PTR<TileInfo>> m_TileMap;
	Vector4 m_TileSize;			// 타일 사이즈
	Matrix m_Scale;

public:
	std::list<MapRenderer::OwnershipTileData> AllList();

public:
	bool Init(int _RenderGroup) override;

	void SetTileSize(Vector2 _Size)		// 타일 사이즈
	{
		m_TileSize.x = _Size.x;
		m_TileSize.y = _Size.y;
		m_TileSize.z = 1.0f;
	}

	void Render(const Matrix& _ViewProjection) override;

	void CreateTile(int _x, int _y);
	void CreateTile(Vector2 _Position);

	void SetMapTileSprite(const wchar_t* _Name);		// 맵타일 스프라이트 지정

	size_t GetTileSize();

public:
	void Clear();


public:
	//public:
	//	My_PTR<GameObject> m_MapObject;
	//	My_PTR<SpriteRenderer2D> m_TileRender;
	//
	//	// 추가
	//
	//	std::map<Vector3, ImageInfo*> m_MapInfo;		// 맵타일 (위치, 정보) 
	//	std::vector<ImageInfo*> m_BaseTile;				// 기본 베이스 타일 (개수도 알고있음)
	//
	//private:
	//	My_PTR<GameObject> m_CameraObject;
	//	My_PTR<Camera> m_Camera;
	//
	//public:
	//	void MouseClick(Vector2 _Pos) {}		// 클릭한 위치
	//	void TileDragDraw() {}					// 클릭후 드래그..해서 찍기 (랜덤으로) 그리고 찍혔으면 넣기 ㅇㅅㅇ


public:
	MapRenderer();
	~MapRenderer();
};

