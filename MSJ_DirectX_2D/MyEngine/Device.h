#pragma once
#include "DXHeader.h"
#include "ConstBuffer.h"
#include "ReferenceBase.h"
#include <unordered_map>

//class Core;
//class Window;
class Device
{
private:
	class DeviceDestroyer
	{
	public:
		~DeviceDestroyer()
		{
			Device::Release();
		}
	};

	static DeviceDestroyer DiviceDes;

public:
	friend class Window;
	friend class Scene;
	friend class Core;
	friend class DeviceDestroyer;
	// ���̷�Ʈ 11 ���̺귯���� GUID�� ����ϴ� UUID��Ŀ� �İ�ü����� �������̽��� �����Ѵ�.

private:
	// ����ũ�μ���Ʈ���� �����ϴ� ���̷�Ʈ11�� �׷���ī�� ���ҽ� ���� ��ü.
	// DX9������ ����̽��� ������ �� ����̽��� ���� �� �� �־���. 11���ʹ� ���ε���..

	// Direct3D�� �ʱ�ȭ ����
	//
	// 1. D3D11CreateDevice �Լ��� �̿��ؼ�, ID3D11Device (��ġ�������̽�)�� ID3D11DeviceContext (��ġ�����������̽�) �������̽��� ����
	// 2. ID3D11Device::CheckMultisampleQualityLevels  �޼ҵ带 �̿��ؼ� 4X MSAA(��Ƽ���ø� ��Ƽ �ٸ����) ǰ�� ���� �������θ� �����Ѵ�.
	// 3. ������ ��ȯ �罽�� Ư���� �����ϴ� DXGI_SWAP_CHAIN_DESC ����ü�� ä���.
	// 4. ��ġ�� �����ϴµ� ����ߴ� IDXGIFactory �������̽��� �����ؼ� IDXGISwapCHain �ν��Ͻ��� �����Ѵ�.
	// 5. ��ȯ �罽�� �ĸ� ���ۿ� ���� ���� ��� �並 �����Ѵ�.
	// 6. ����&���ٽǹ��ۿ� �׿� ����Ǵ� ����&���ٽ� �並 �����Ѵ�.
	// 7. ���� ��� ��� ����&���ٽ� �並 Direct3D�� ����� �� �ֵ��� ������ ������������ ��� ���ձ� �ܰ迡 ���´�.
	// 8. �� ��Ʈ�� �����Ѵ�.


	// ID3D11Device�� ID3D11DeviceContext�� Direct3D�� �ֵ� �������̽��� �������� �׷��� ��ġ �ϵ��� ���� ����Ʈ���� ������� �����ϸ� ��.
	// �� ���� �������̽����� ���ؼ� �ϵ����� ���� (GPU �޸𸮿� �ڿ��Ҵ�, �ĸ� ���� �����, �ڿ��� ���� ���������� �ܰ迡 ���� ���ϱ��� �׸��� ��)�� �����Ѵ�.


	static ID3D11Device* m_pDevice;						// �׷���ī�� ���ҽ� ���� ��ü, ��� ���� ���˰� �ڿ��Ҵ翡 ���δ�.
	
	static ID3D11DeviceContext* m_pContext;				// �׷���ī�� �������� ���� ��ü /  ���� ��� ����, �ڿ��� ���������ο� ���� GPU�� ������ ������ ��ɵ��� �����ϴµ� ����

	// ���� ��� ��
	// ID3D11RenderTargetView
	// � �ڿ��� ������������ �ܰ迡 ���� ���� ���� �ƴ�.
	// �ݵ�� �ڿ��� ���� �並 �����ϰ� �� �並 ���������� �ܰ迡 ��������.
	// -> �ĸ� ���۸� ������������ ��� ���ձ�(output merger)�ܰ迡 �������� 
	// ���� ���� ��� �並 �����ؾ� �Ѵ�. ( �̷��� �ؾ� Direct3D�� ���ۿ� ������ �׸�������..)
	static ID3D11RenderTargetView* m_pTargetView;		// ��µ� ����� Ÿ���� �䰴ü (���)

	static ID3D11DepthStencilView* m_pDepthStencilView;	// �������� ���̰��� ���� view��ü (z���� ����,  �ڿ��ִ� ��ü���� ������ ��.. �������ָ� ���� ������� ��������
	static ID3D11Texture2D* m_pDepthStencilTexture;		// ���̰��� ������ �ؽ��� ��ü 

	static IDXGISwapChain* m_pSwapChain;				// ����ü��, ����۸� ���⼭ ������.

	static UINT m_MultiSampleLevel;						// MultiSampleLevel�� ����� ����

	static VCOLOR m_Color;								// ����


	// �����Ͷ����� ������Ʈ (Rasterize State) -> �����Ͷ������� ������
private:
	class RasterizeState : public ReferenceBase
	{
	public:
		D3D11_RASTERIZER_DESC m_Desc;
		ID3D11RasterizerState* m_pState;

	public:
		~RasterizeState();
	};

private:
	static std::unordered_map<std::wstring, My_PTR<RasterizeState>> m_RasterizeStateMap;

public:
	static void RasterizeState_Create(const wchar_t* _Name, D3D11_CULL_MODE _CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE, D3D11_FILL_MODE _FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID);
	
	static void RasterizeState_Change(const wchar_t* _Name);

	//
public:
	static ID3D11Device* Get_Device() { return m_pDevice; }
	static ID3D11DeviceContext* Get_Context() { return m_pContext; }
	static IDXGISwapChain* Get_SwapChain() { return m_pSwapChain; }

private:
	static bool Create_SwapChain();
	static bool Create_DepthStancilView();
	static bool Create_ViewPort();

private:
	static bool Init();
	static void Release();

	// ȭ�� ����
	static void ClearTarget();

	// �߰��� ȭ���� �׸� ( DX�� ȭ���� ���� -> ȭ���� �׸� -> �ٱ׷����� ��� �̰� �ݺ��� )

	// ȭ�� ���
	static void SwapChain_Present();


private:
	static void DefaultResource_Init();

private:
	Device();
	virtual ~Device() = 0;
};

