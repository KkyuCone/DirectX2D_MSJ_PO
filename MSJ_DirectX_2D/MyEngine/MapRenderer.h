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

	// ���� �߰��Ѱ�
public:
	std::vector<std::wstring> ALLFolderPath;


private:
	int BaseX;
	int BaseY;

	int FirstTile;

	Vector2 m_BaseTilePos;

	/////////////////////////////////////////////////////////

public:
	// Ÿ�� �̹��� ������ 
	class MapTileData
	{
	//public:
	//	Vector2 m_TilePos;		// Ÿ�� ��ġ
	//	Vector2 m_Size;			// Ÿ�� ������ (x,y)
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
	Vector4 m_TileSize;			// Ÿ�� ������
	Matrix m_Scale;

public:
	std::list<MapRenderer::OwnershipTileData> AllList();

public:
	bool Init(int _RenderGroup) override;

	void SetTileSize(Vector2 _Size)		// Ÿ�� ������
	{
		m_TileSize.x = _Size.x;
		m_TileSize.y = _Size.y;
		m_TileSize.z = 1.0f;
	}

	void Render(const Matrix& _ViewProjection) override;

	void CreateTile(int _x, int _y);
	void CreateTile(Vector2 _Position);

	void SetMapTileSprite(const wchar_t* _Name);		// ��Ÿ�� ��������Ʈ ����

	size_t GetTileSize();

public:
	void Clear();


public:
	//public:
	//	My_PTR<GameObject> m_MapObject;
	//	My_PTR<SpriteRenderer2D> m_TileRender;
	//
	//	// �߰�
	//
	//	std::map<Vector3, ImageInfo*> m_MapInfo;		// ��Ÿ�� (��ġ, ����) 
	//	std::vector<ImageInfo*> m_BaseTile;				// �⺻ ���̽� Ÿ�� (������ �˰�����)
	//
	//private:
	//	My_PTR<GameObject> m_CameraObject;
	//	My_PTR<Camera> m_Camera;
	//
	//public:
	//	void MouseClick(Vector2 _Pos) {}		// Ŭ���� ��ġ
	//	void TileDragDraw() {}					// Ŭ���� �巡��..�ؼ� ��� (��������) �׸��� �������� �ֱ� ������


public:
	MapRenderer();
	~MapRenderer();
};

