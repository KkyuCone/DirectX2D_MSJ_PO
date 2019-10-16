#include "Device.h"
#include "Window.h"

// 레스터라이즈 스테이트
// unordered_map는 O(logN) 해시함수 -> 저장 원소들이 많을수록 map보다 유리
// map은 O(1) 레드블랙트리 사용
std::unordered_map<std::wstring, My_PTR<Device::RasterizeState>> Device::m_RasterizeStateMap;

Device::RasterizeState::~RasterizeState()
{
	SAFE_RELEASE(m_pState);		// 삭제[ (안하면 릭남음)
}


//

Device::DeviceDestroyer Device::DiviceDes;

// 얘 배경색임 ㅇㅅㅇ

VCOLOR Device::m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
/*VCOLOR Device::m_Color = { 0.0f, 0.94f, 0.70f, 1.0f };*/

ID3D11Device*					Device::m_pDevice = nullptr;			// 장치 인터페이스
ID3D11DeviceContext*			Device::m_pContext = nullptr;			// 장치 문맥 인터페이스

// 텍스처 생성할떄 두 파이프라인을 지정한 결속 플래그를 사용
// D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
ID3D11RenderTargetView*			Device::m_pTargetView = nullptr;		// 텍스처를 사용하기 위해.. (자원뷰중하나) , 텍스처를 렌더 대상으로 묶은것, 셰이더 자원으로서 묶는것
ID3D11DepthStencilView*			Device::m_pDepthStencilView = nullptr;

// 깊이 버퍼링, 깊이 버퍼(depth buffer)

// 관련페이지 p.107
// 깊이버퍼 : 이미지 자료를 담지 않는 텍스처의 한 예
// 깊이 버퍼는 각 픽셀의 깊이 정보를 담는다.
// 픽셀의 깊이 -> 0.0(관찰자와 최대한 가까운 픽셀) ~ 1.0 (최대한 먼 픽셀)
// Z값 같은 원근감 표시  ( 앞의 물체가 뒤의 물체를 가리는것 )
// 깊이 버퍼링에서 물체들을 그리는 순서는 중요하지 않음.
// 깊이 값이 이미 들어있던 값보다 작은 경우에만 후면 버퍼와 깊이 버퍼에 기록됨.

ID3D11Texture2D*				Device::m_pDepthStencilTexture = nullptr;

// 교환사슬 & 페이지 전환 (스왑체인, 버퍼교환(Buffer swapping)

// 그림 그리는중(그리기) - 후면 버퍼
// 다 그린그림을 화면에 띄우기 - 전면 버퍼
// 전면 버퍼가 화면에 표시되어 있는 동안 다음 그림을 후면 버퍼에 그린다.
// -> 다 그린 후에는 전면버퍼와 후면버퍼의 역할을 맞바꾼다.   그리기 -> 다그린그림띄우기 -> 그리기 (반복)
// 여기서 후면 버퍼르 ㄹ전면 버퍼와 교환해서 화면에 표시하게 되는 것을 제시 (presenting)이라고 한다.
// 버퍼들 자체를 맞바꾸는 것이아니라 포인터만 맞바꾸는 것임. ( 교환사슬(swap chain) 형성 )
//
// DX3에서 교환 사슬은 IDXGISwapChain이라는 인터페이스 사용, 전면버퍼텍스처와 후면 버퍼 텍스처를 담음.
// 버퍼 크기 변경을 위한 메소드 -> IDXGISwapChain::ResizeBuffers
// 버퍼 출력 메소드(화면출력) -> IDXGISwapChain::Present
//
// 두 개의 버퍼(전면,후면)을 사용하는 것을 이중버퍼링이라고 한다.( 더 많은 버퍼 사용 가능, 삼중 버퍼링, 하지만 보통 버퍼 2개만으로 충분 )

IDXGISwapChain*					Device::m_pSwapChain;				// 스왑체인, 백버퍼를 여기서 셋팅함.
UINT							Device::m_MultiSampleLevel = 0;						// MultiSampleLevel를 담당할 변수

Device::Device()
{
}


Device::~Device()
{
}

void Device::Release()
{
	if (nullptr != m_pDepthStencilTexture)	
	{
		m_pDepthStencilTexture->Release();
		m_pDepthStencilTexture = nullptr;
	}

	if (nullptr != m_pDepthStencilView)		
	{ 
		m_pDepthStencilView->Release();
		m_pDepthStencilView = nullptr;
	}

	if (nullptr != m_pSwapChain)
	{
		m_pSwapChain->Release();



		m_pSwapChain = nullptr;
	}

	if (nullptr != m_pTargetView)			
	{ 
		m_pTargetView->Release();
		m_pTargetView = nullptr;
	}

	if (nullptr != m_pContext)				
	{ 
		m_pContext->Release();
		m_pContext = nullptr;
	
	}

	if (nullptr != m_pDevice)
	{
		m_pDevice->Release();
		m_pDevice = nullptr;
	}
}

bool Device::Init()
{
	// 플래그 0 은 D3D11_CREATE_DEVICE_SINGLETHREADED : 단일 스레드다..
	// 그외에 여러가지 enum값으로 가지고 있음. MSDN으로 검색..! 책에는 잘못나왔음.
	int Int_Flag = 0;		// 단일스레드로 할려고 .. enum 값이라서 첫번째꺼임 

#ifdef _DEBUG
	// 렌더링모드가 Debug인지 Release인지 세팅해줘야함.
	// D3D11_CREATE_DEVICE_DEBUG : 디버그 모드 빌드에서 디버그 계층을 활성화시키려면 이 플래스로 설정..
	// 이 디버그 플래그를 지정하면 Direct3D는 VC++ 출력창에 디버그 메시지를 보냄
	Int_Flag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	// 디바이스의 현재 버전에 대해 알아보기 위한 변수
	// 그리고 현재 내가 세팅해준 버전의 디바이스를 얻어올수 있는지에 대한 변수
	// D3D_FEATURE_LEVEL_11_0 : Direct2D API와 함꼐 사용할 수 있는 장치를 만들기 위한 것임.
	D3D_FEATURE_LEVEL DeviceVersion = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	// HRESULT 성공시 0 실패시 0이 아닌수 (long, int를 리턴)
	// 디바이스와 컨텍스트를 얻어옴.
	//HRESULT D3D11CreateDevice() 함수 관련 p.116
	// 장치생성( 초기화 )
	if (S_OK != D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
		nullptr, Int_Flag, nullptr, 0, D3D11_SDK_VERSION, &m_pDevice, &DeviceVersion, &m_pContext))
	{
		Release();
		return false;
	}

	// 안티앨리어싱 기법들 ( 멀티샘플링, 슈퍼샘플링 )  -> 앨리어싱 현상을 해결하기 위해 사용
	
	// 1. 슈퍼샘플링(초과표본화)
	// 후면 & 깊이 버퍼를 기존 해상도보다 4배 크게 -> 후면 버퍼에 렌더링 
	// -> 이미지를 화면에 출력할 때 후면 버퍼를 원래크기로 환원하려고 출력 -> 메모리 소비량이 엄청 큼.
	// 대신 엄청 부드럽다. (매끈한 선)
	//
	// 2. 멀티샘플링(다중표본화)  -> DX에서 사용
	// 슈퍼샘플링 보다 메모리 소비량이 적다. ( 보다 적은거지 엄청 적은게 아님 하지만 그 차이가 큼 )
	// 효과는 슈퍼샘플링보다 별로... 
	// 일부계산 결과를 부분 픽셀들 사이에서 공유하기 때문임 ( 메모리 소비량이 적은 이유 )
	// 슈퍼샘플링과 같이 화면 해상도의 4배인 후면,깊이 버퍼를 사용.
	//  이미지 색상을 각 부분픽셀마다 계산하는 것이 아니라 한번만 계산하여 전체적으로 최종색상을 결정
	//
	// 아래는 선생님 설명
	// 래스터라이저에서 큰이미지->작은이미지로 줄일때 보간이 일어나는데
	// 이것을 샘플링이라고 한다.
	// 멀티 샘플링은 ( 미리 여러가지 비율로 이미지를 만들어 놓고 알맞게 사용하는것 )
	// 예를 들어 2048 크기의 이미지가 있을 때 
	// 이걸 비율로 쪼개 여러가지 이미지를 미리 만들어 놓고 축소 렌더링시에 해당 비율에 알맞게 가져와서 사용한다.
	//
	// 멀티샘플링을 위해선 DXGI_SAMPLE_DESC라는 구조체를 채워야한다.

	// UNORM
	// UNSIGEND NORMAL ( 최대 임의의 방향으로 1이 될 수 있다. )
	// m_MultiSampleLevel 1이 뽑혀 나옴
	m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_MultiSampleLevel);

	if (false == Create_SwapChain())
	{
		Release();
		return false;
	}

	if (false == Create_DepthStancilView())
	{
		Release();
		return false;
	}

	if (false == Create_ViewPort())
	{
		Release();
		return false;
	}

	DefaultResource_Init();

	return true;
}


bool Device::Create_SwapChain()
{
	// 교환사슬 설정(스왑체인)
	DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};


	// BufferDesc-> 생성하고자 하는 후면 버퍼의 속성들 서술하는 개별적인 구조체
	SwapChainDesc.BufferDesc.Width = Window::Size().ix;			// 후면 버퍼의 너비
	SwapChainDesc.BufferDesc.Height = Window::Size().iy;		// 후면 버퍼의 높이

	// 모니터 주사율에 따른 버퍼 갱신 횟수를 의미
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;			// 분자		
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;			// 분모   -> 60/1  -> 그러니까 1초에 60번..!

	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 후면 버퍼의 픽셀 형식, FXGI_FORMAT 열거형의 값, 버퍼의 텍스처 자원으로 사용할 수 있는 형식들을 제공

	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	// 디스플레이 스캔라인 모드, 이미지 생성을 위한 래스터 기법들을 정의
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;					// 디스플레이 비례 모드, 버퍼를 창의 클라이언트 영역에 적용하는 방식

	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// 백버퍼의 용도

	// 여기까지가 백버퍼 세팅~ 끝

	// 스왑체인 멀티샘플 카운트 & 퀄리티
	// SampleDesc-> 멀티샘플링을 위해 추출할 표본 개수와 품질 수준을 서술하는 구조체
	SwapChainDesc.SampleDesc.Count = 1;		// 픽셀당 추출할 표본의 개수
	SwapChainDesc.SampleDesc.Quality = 0;	// 원하는 품질 수준 

	SwapChainDesc.OutputWindow = Window::WindowHWND();		// 렌더링 결과를 표기할 창의 핸들 ( 스왑체인의 내용을 받을 창의 핸들)
	SwapChainDesc.BufferCount = 1;			// 스왑체인에서 사용할 후면 버퍼의 개수 일반적으로 1개만 사용(이중버퍼링), 2개는 삼중버퍼링.

	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;		// 스왑 체인을 서술하는 구조체, DXGI_SWAP_EFFECT_DISCARD-> 디스플레이 구동기가 가장 효율적인 제시 방법을 선택하게 됨.

	// Windowed가 flase면 전체화면, true면 전체화면 아님
	SwapChainDesc.Windowed = !Window::IsSceneFullSize();		// 창모드 원하면 true, 전체화면은 false

	// 정보는 세팅함
	// 하지만 정보는 정보일 뿐이고 스왑체인을 만든것은 아님.
	// 스왑체인을 만들기 위해 Dx11에서 지정한 방식의 
	// 어댑터, 디바이스를 이용해서만 만들 수 있음
	// 스왑체인에 대한 객체는
	// 일반적인 텍스처나 매쉬처럼 단순하게 디바이스에게 허락을 받는다고 만들 수 있는 것이 아님
	// 스왑체인을 만들기 위해서는 다음과 같이 해야함.

	IDXGIDevice* pIdxGIDevice = nullptr;
	IDXGIAdapter* pIdxGIAdapter = nullptr;

	// 객체를 만들어줄 팩토리객체 
	// 스왑체인을 서술하는 구조체를 만들었다면 다음으로 IDXGIFactory 인스턴스를 통해
	// IDXGIFactory::CreateSwapChain 메소드를 호출해서 스왑체인 인터페이스 (IDXGISwapChain)를 생성한다.
	IDXGIFactory* pIdxGIFactory = nullptr;


	// 컴객체 방식 
	// 질의용 메소드  (UUID, 포인터를 가리키는 포인터)  -> 이 질의 메커니즘은 주로 ID3D11Device 인터페이스와 함께 쓰임
	// UUID (universally unique identifier, 보편 고유 식별자)
	// 추가적인 장치 인터페이스가 필요한 상황에서 사용함..
	// 예를 들어 Direct3D 장치로부터 장치의 디버그 인터페이스를 얻거나 DXGI 인터페이스를 얻고자 할때 ..!이것을 사용함
	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pIdxGIDevice);

	if (nullptr == pIdxGIDevice)
	{
		EAssert(true);
		return false;
	}

	// pIdxGIDevice를 이용해서 스왑체인을 생성할 팩토리를 만들수 있는 어댑터
	pIdxGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pIdxGIAdapter);

	if (nullptr == pIdxGIAdapter)
	{
		EAssert(true);
		return false;
	}

	// 드디어 IDXGIFactory 인터페이스를 얻게됨 
	pIdxGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pIdxGIFactory);

	if (nullptr == pIdxGIFactory || S_OK != pIdxGIFactory->CreateSwapChain(m_pDevice, &SwapChainDesc, &m_pSwapChain))	// 스왑체인 생성
	{
		EAssert(true);
		return false;
	}

	// 획득한 COM 인터페이스들을 해제 ( 다사용했으니까! )
	pIdxGIFactory->Release();
	pIdxGIAdapter->Release();
	pIdxGIDevice->Release();

	return true;
}

bool Device::Create_DepthStancilView()
{
	// 스왑체인을 만들었는데 스왑체인에 있는 백버퍼 등을 가져와서 내가 제어할수 있게 해야 한다.

	ID3D11Texture2D* pBackBufferTexture = nullptr;

	// m_pSwapChain->GetBuffer()로 후면 버퍼얻기
	// GetBuffer 메소드로 스왑체인을 가리키는 포인터를 얻는다.
	// GetBuffer(0(후면 버퍼가 1개면 0), 버퍼의 인터페이스 형식, 후면버퍼를 가리키는 포인터를 돌려줄 애(반환할)즉 받을애)
	if (S_OK != m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture))
	{
		EAssert(true);
		return false;
	}

	// 렌더 타겟뷰는 텍스처 등을 내가 원하는대로 제어할 수 있게 해주는 객체.
	// 렌더타겟이랑 그림한장을 더 만들었다고 생각, 거기에 또 그릴수 있음.
	// 후면 버퍼를 파이프라인의 출력 병합기(output merger) 단계에 묶으려면 후면 버퍼에 대한 렌더 대상 뷰를 생성해야함.
	// 렌더 뷰를 생성하는 메소드 CreateRenderTargetView(렌더 대상으로 사용할 자원, D3D11_RENDER_TARGET_VIEW_DESC 구조체를 가리키는 포인터, 돌려줄 생성된 렌더 대상뷰)
	// 지금은 후면 버퍼에 대한 렌더 대상 뷰를 생성하는 것이므로 방금 얻은 후면 버퍼를 지정했음.
	if (S_OK != m_pDevice->CreateRenderTargetView(pBackBufferTexture, 0, &m_pTargetView))
	{
		EAssert(true);
		return false;
	}

	// m_pSwapChain->GetBuffer()를 호출하면 후면 버퍼에 대한 COM 참조 횟수가 증가한다. 그래서 다 사용하고나서 해제해야한다.
	pBackBufferTexture->Release();

	// 스왑체인뷰가 담당할 텍스처를 만든다.
	// 아래는 깊이 값을 저장할 스텐실 텍스쳐
	// 2차원 텍스처를 생성할 때에는 생성할 텍스처를 서술하는 D3D11_TEXTRUE2D_DESC 구조체를 채우고!
	// ID3D11Device::CreateTexture2D 메소드를 호출해야 한다.
	D3D11_TEXTURE2D_DESC TextureDesc = {};

	TextureDesc.ArraySize = 1;							// 텍스처 배열의 텍스처 개수, 깊이&스텐실 버퍼의 경우에는 텍스처 하나만 필요하다.
	TextureDesc.Width = (UINT)Window::Size().ix;		// 텍스처의 너비(텍셀 단위)
	TextureDesc.Height = (UINT)Window::Size().iy;		// 텍스처의 높이 (텍셀 단위)

	// 바이트 크기가 중요, 색상값을 처리하는 것은 별로 중요하지 않음
	// 아래는 스텐실버퍼.. 24비트는 깊이버퍼에.. 8비트를 스텐실버퍼에 사용.
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;		// 깊이 버퍼링을 위한 텍스처 형식, 텍셀의 형식을 뜻하는 필드, DXGI_FORMAT 열거형의 값들 중 하나를 지정한다.

	// SampleDesc : 멀티샘플링 개수와 품질 수준을 서술하는 구조체, 렌더 대상에 쓰인 설정과 일치해야함.
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.SampleDesc.Quality = 0;

	TextureDesc.MipLevels = 1;							// 밉맵 수준의 개수, 깊이&스텐실 버퍼의 경우에는 텍스처 하나만 필요

	// Usage : 텍스처 용도를 뜻하는 필드,  D3D11_USAGE 열거형의 값들 중 하나를 지정.

	// 1. D3D11_USAGE_DEAFAULT   : 자원을 GPU가 읽고 써야 한다면 이걸로 설정.  이걸로 설정하면 GPU는 자원을 읽거나 쓸 수 없다.
	//							   깊이&스텐실 버퍼에 대한 모든 읽기 쓰기는 GPU가 수행하므로 깊이 스텐실 버퍼를 위한 텍스처를 생성할 때 이걸 사용
	// 2. D3D11_USAGE_IMMUTABLE  : 자원을 생성한 후에 그 내용을 바꾸지 않을 경우 이것을 사용. 이 용도를 지정하면 
	//							   자원이 GPU 읽기 전용이 뒤어서 몇가지 최적화가 가능해진다. 
	//							   CPU와 GPU는 이런 불변 자원에 자료를 기록할 수 없다. ( 생성 시점을 예외로 할때 ), CPU는 불면 자원의 자료를 읽을 수 없다.
	// 3. D3D11_USAGE_DYNAMIC    : 응용 프로그램(CPU)이 자원의 내용을 빈번하게 갱신해야 한다면 이것을 지정.
	//							   이 용도로 생성된 자원은 GPU가 읽을 수 있고 CPU가 기록할 수 있다. 
	//							   CPU의 자원을 CPU에서 동적 갱신하면 성능상 안좋기 때문에 꼭 필요한 경우가 아니면 자제.
	// 4. D3D11_USAGE_STAGING    : 응용 프로그램(CPU)이 자원의 복사본을 읽어야 한다면 이것을 사용.
	//							   GPU에서 CPU 메모리복사는 느린 연산이기 때문에 이것 또한 필요한 경우가 아니면 피해야한다.
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;		

	// 뎁스 스탠실용 텍스쳐
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;   // 

	// CreateTexture2D ( 생성할 텍스처를 서술하는 구조체, 
	//					텍스처에 채울 초기 자료를 가리키는 포인터임 여기선 깊이 스텐실 버퍼로 사용할거니까 0(null)값을 넣어줌, 
	//					깊이 스텐실 버퍼를 가리키는 포인터를 받을 텍스처 )
	m_pDevice->CreateTexture2D(&TextureDesc, 0, &m_pDepthStencilTexture);

	if (nullptr == m_pDepthStencilTexture)
	{
		EAssert(true);
		return false;
	}


	// 뎁스 스탠실
	// CreateDepthStencilView( 뷰를 생성하고자 하는 자원,
	//						D3D11_DEPTH_STENCIL_VIEW_DESC 구조체를 가리키는 포인터임(자원의 원소 자료 형식) 형식을 지정해서 자원을 생성했으면 0(null)값을 지정해도됨, 
	//						깊이 스텐실 뷰 )
	if (S_OK != m_pDevice->CreateDepthStencilView(m_pDepthStencilTexture, 0, &m_pDepthStencilView))
	{
		EAssert(true);
		return false;
	}

	// 여기에 만들어진 뷰가 최종단계인 OutPutMerger에서 사용할 뷰라는 것을 명시해야함.
	// 첫번째 인자 - 한번에 몇개의 렌더타겟 뷰를 세팅할 것인가 (우리는 1개)
	// 그리고 렌더타겟 뷰들의 포인터들을 넣어주기
	// 깊이 버퍼를 쓰는 방식이 아니라서 깊이 버퍼는 하나만 넣어줘도됨.
	// 파이프라인의 출력 병합기 단계에 묶는 것임.
	// OMSetRenderTargets(묶고자 하는 렌더 대상의 개수, 
	//						파이프라인에 묶을 렌더 대상 뷰들을 가리키는 포인터들을 담은 배열의 첫 원소를 가리키는 포인터,
	//						파이프라인에 묶을 깊이 스텐실 뷰를 가리키는 포인터 ) 
	// 렌더 대상 뷰는 여러개를 배열에 담아서 한번에 묶을 수 있지만 깊이&스텐실뷰는 하나만 묶을 수 있음.
	m_pContext->OMSetRenderTargets(1, &m_pTargetView, m_pDepthStencilView);

	return true;
}

// 뷰포트 설정 

bool Device::Create_ViewPort()
{
	D3D11_VIEWPORT ViewPort = {};

	ViewPort.TopLeftX = 0;
	ViewPort.TopLeftY = 0;

	ViewPort.Width = (float)Window::Size().ix;
	ViewPort.Height = (float)Window::Size().iy;


	// Direct3D가 사용하는 깊이 버퍼 값의 범위가 0~1이라서 최소값을 0 최대값을 1이라고 설정( 특수효과를 낼거면 다르게 설정 )
	ViewPort.MinDepth = 0;
	ViewPort.MaxDepth = 1;


	// 뷰포트의 구조체를 다 채운 후에는 Context에 뷰포트를 알려준다. 'ㅅ'
	m_pContext->RSSetViewports(1, &ViewPort);		// (뷰포트의 개수, 뷰포트배열을 가리키는 포인터)

	return true;
}

// 화면 지우기
void Device::ClearTarget()
{
	m_pContext->ClearRenderTargetView(m_pTargetView, m_Color.p_float);
	m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

// 화면  출력 
void Device::SwapChain_Present()
{
	m_pSwapChain->Present(0, 0);
}



// 레이터라이즈 스테이트
void Device::RasterizeState_Create(const wchar_t* _Name,
									D3D11_CULL_MODE _CullMode/* = D3D11_CULL_MODE::D3D11_CULL_NONE*/,
									D3D11_FILL_MODE _FillMode/* = D3D11_FILL_MODE::D3D11_FILL_SOLID*/)
{
	if (nullptr != Map_Find_PTR<My_PTR<RasterizeState>>(m_RasterizeStateMap, _Name))
	{
		EAssert(true);
		return;
	}

	My_PTR<RasterizeState> NewRasterize = new RasterizeState();
	NewRasterize->m_Desc.CullMode = _CullMode;						// 그냥 무조건 다 그린다.
	NewRasterize->m_Desc.FillMode = _FillMode;						// SOLID는 다 채워진채로 그리고 와이어는 외곽선만..

	if (S_OK != m_pDevice->CreateRasterizerState(&(NewRasterize->m_Desc), &(NewRasterize->m_pState)))
	{
		EAssert(true);
		return;
	}

	m_RasterizeStateMap.insert(std::unordered_map<std::wstring, My_PTR<RasterizeState>>::value_type(_Name, NewRasterize));
}

void Device::RasterizeState_Change(const wchar_t* _Name)
{
	My_PTR<RasterizeState> Find_RasterizeState = Map_Find_PTR<My_PTR<RasterizeState>>(m_RasterizeStateMap, _Name);

	if (nullptr == Find_RasterizeState)
	{
		EAssert(true);
		return;
	}

	// RS라는것이 붙어있는건 모두 레스터라이저 단계와 관련이 있는 것이다. (레스터라이저 단계에서 간섭함)
	m_pContext->RSSetState(Find_RasterizeState->m_pState);
}