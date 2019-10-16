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
	// 다이렉트 11 라이브러리는 GUID에 기반하는 UUID방식에 컴객체방식의 인터페이스를 제공한다.

private:
	// 마이크로소프트에서 지원하는 다이렉트11의 그래픽카드 리소스 관련 객체.
	// DX9까지는 디바이스를 얻어오면 그 디바이스로 모든걸 할 수 있었음. 11부터는 따로따로..

	// Direct3D의 초기화 과정
	//
	// 1. D3D11CreateDevice 함수를 이용해서, ID3D11Device (장치인터페이스)와 ID3D11DeviceContext (장치문맥인터페이스) 인터페이스를 생성
	// 2. ID3D11Device::CheckMultisampleQualityLevels  메소드를 이용해서 4X MSAA(멀티샘플링 안티 앨리어싱) 품질 수준 지원여부를 점검한다.
	// 3. 생성할 교환 사슬의 특성을 서술하는 DXGI_SWAP_CHAIN_DESC 구조체를 채운다.
	// 4. 장치를 생성하는데 사용했던 IDXGIFactory 인터페이스를 질의해서 IDXGISwapCHain 인스턴스를 생성한다.
	// 5. 교환 사슬의 후면 버퍼에 대한 렌더 대상 뷰를 생성한다.
	// 6. 깊이&스텐실버퍼와 그에 연결되는 깊이&스텐실 뷰를 생성한다.
	// 7. 렌더 대상 뷰와 깊이&스텐실 뷰를 Direct3D가 사용할 수 있도록 렌더링 파이프라인의 출력 병합기 단계에 묶는다.
	// 8. 뷰 포트를 설정한다.


	// ID3D11Device와 ID3D11DeviceContext는 Direct3D의 주된 인터페이스로 물리적인 그래픽 장치 하드웨어에 대한 소프트웨어 제어기라고 생각하면 됨.
	// 위 둘의 인터페이스들을 통해서 하드웨어에게 할일 (GPU 메모리에 자원할당, 후면 버퍼 지우기, 자원을 여러 파이프라인 단계에 묶기 기하구조 그리기 등)을 지시한다.


	static ID3D11Device* m_pDevice;						// 그래픽카드 리소스 관련 객체, 기능 지원 점검과 자원할당에 쓰인다.
	
	static ID3D11DeviceContext* m_pContext;				// 그래픽카드 렌더연산 관련 객체 /  렌더 대상 설정, 자원을 파이프라인에 묶고 GPU가 수행할 렌더링 명령들을 지시하는데 쓰임

	// 렌더 대상 뷰
	// ID3D11RenderTargetView
	// 어떤 자원을 파이프라인의 단계에 직접 묶는 것이 아님.
	// 반드시 자원에 대한 뷰를 생성하고 그 뷰를 파이프라인 단계에 묶어어야함.
	// -> 후면 버퍼를 파이프라인의 출력 병합기(output merger)단계에 묶으려면 
	// 먼저 렌더 대상 뷰를 생성해야 한다. ( 이렇게 해야 Direct3D가 버퍼에 뭔가를 그릴수있음..)
	static ID3D11RenderTargetView* m_pTargetView;		// 출력될 백버퍼 타겟의 뷰객체 (배경)

	static ID3D11DepthStencilView* m_pDepthStencilView;	// 렌더링시 깊이값에 대한 view객체 (z값을 저장,  뒤에있는 물체들은 가리는 등.. 안정해주면 렌더 순서대로 렌더링됨
	static ID3D11Texture2D* m_pDepthStencilTexture;		// 깊이값을 저장할 텍스쳐 객체 

	static IDXGISwapChain* m_pSwapChain;				// 스왑체인, 백버퍼를 여기서 셋팅함.

	static UINT m_MultiSampleLevel;						// MultiSampleLevel를 담당할 변수

	static VCOLOR m_Color;								// 색상


	// 레스터라이즈 스테이트 (Rasterize State) -> 레스터라이저에 관여함
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

	// 화면 지움
	static void ClearTarget();

	// 중간에 화면을 그림 ( DX는 화면을 지움 -> 화면을 그림 -> 다그렸으면 출력 이걸 반복함 )

	// 화면 출력
	static void SwapChain_Present();


private:
	static void DefaultResource_Init();

private:
	Device();
	virtual ~Device() = 0;
};

