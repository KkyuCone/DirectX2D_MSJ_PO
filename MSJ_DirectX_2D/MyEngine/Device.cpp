#include "Device.h"
#include "Window.h"

// �����Ͷ����� ������Ʈ
// unordered_map�� O(logN) �ؽ��Լ� -> ���� ���ҵ��� �������� map���� ����
// map�� O(1) �����Ʈ�� ���
std::unordered_map<std::wstring, My_PTR<Device::RasterizeState>> Device::m_RasterizeStateMap;

Device::RasterizeState::~RasterizeState()
{
	SAFE_RELEASE(m_pState);		// ����[ (���ϸ� ������)
}


//

Device::DeviceDestroyer Device::DiviceDes;

// �� ������ ������

VCOLOR Device::m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
/*VCOLOR Device::m_Color = { 0.0f, 0.94f, 0.70f, 1.0f };*/

ID3D11Device*					Device::m_pDevice = nullptr;			// ��ġ �������̽�
ID3D11DeviceContext*			Device::m_pContext = nullptr;			// ��ġ ���� �������̽�

// �ؽ�ó �����ҋ� �� ������������ ������ ��� �÷��׸� ���
// D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
ID3D11RenderTargetView*			Device::m_pTargetView = nullptr;		// �ؽ�ó�� ����ϱ� ����.. (�ڿ������ϳ�) , �ؽ�ó�� ���� ������� ������, ���̴� �ڿ����μ� ���°�
ID3D11DepthStencilView*			Device::m_pDepthStencilView = nullptr;

// ���� ���۸�, ���� ����(depth buffer)

// ���������� p.107
// ���̹��� : �̹��� �ڷḦ ���� �ʴ� �ؽ�ó�� �� ��
// ���� ���۴� �� �ȼ��� ���� ������ ��´�.
// �ȼ��� ���� -> 0.0(�����ڿ� �ִ��� ����� �ȼ�) ~ 1.0 (�ִ��� �� �ȼ�)
// Z�� ���� ���ٰ� ǥ��  ( ���� ��ü�� ���� ��ü�� �����°� )
// ���� ���۸����� ��ü���� �׸��� ������ �߿����� ����.
// ���� ���� �̹� ����ִ� ������ ���� ��쿡�� �ĸ� ���ۿ� ���� ���ۿ� ��ϵ�.

ID3D11Texture2D*				Device::m_pDepthStencilTexture = nullptr;

// ��ȯ�罽 & ������ ��ȯ (����ü��, ���۱�ȯ(Buffer swapping)

// �׸� �׸�����(�׸���) - �ĸ� ����
// �� �׸��׸��� ȭ�鿡 ���� - ���� ����
// ���� ���۰� ȭ�鿡 ǥ�õǾ� �ִ� ���� ���� �׸��� �ĸ� ���ۿ� �׸���.
// -> �� �׸� �Ŀ��� ������ۿ� �ĸ������ ������ �¹ٲ۴�.   �׸��� -> �ٱ׸��׸����� -> �׸��� (�ݺ�)
// ���⼭ �ĸ� ���۸� ������ ���ۿ� ��ȯ�ؼ� ȭ�鿡 ǥ���ϰ� �Ǵ� ���� ���� (presenting)�̶�� �Ѵ�.
// ���۵� ��ü�� �¹ٲٴ� ���̾ƴ϶� �����͸� �¹ٲٴ� ����. ( ��ȯ�罽(swap chain) ���� )
//
// DX3���� ��ȯ �罽�� IDXGISwapChain�̶�� �������̽� ���, ��������ؽ�ó�� �ĸ� ���� �ؽ�ó�� ����.
// ���� ũ�� ������ ���� �޼ҵ� -> IDXGISwapChain::ResizeBuffers
// ���� ��� �޼ҵ�(ȭ�����) -> IDXGISwapChain::Present
//
// �� ���� ����(����,�ĸ�)�� ����ϴ� ���� ���߹��۸��̶�� �Ѵ�.( �� ���� ���� ��� ����, ���� ���۸�, ������ ���� ���� 2�������� ��� )

IDXGISwapChain*					Device::m_pSwapChain;				// ����ü��, ����۸� ���⼭ ������.
UINT							Device::m_MultiSampleLevel = 0;						// MultiSampleLevel�� ����� ����

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
	// �÷��� 0 �� D3D11_CREATE_DEVICE_SINGLETHREADED : ���� �������..
	// �׿ܿ� �������� enum������ ������ ����. MSDN���� �˻�..! å���� �߸�������.
	int Int_Flag = 0;		// ���Ͻ������ �ҷ��� .. enum ���̶� ù��°���� 

#ifdef _DEBUG
	// ��������尡 Debug���� Release���� �����������.
	// D3D11_CREATE_DEVICE_DEBUG : ����� ��� ���忡�� ����� ������ Ȱ��ȭ��Ű���� �� �÷����� ����..
	// �� ����� �÷��׸� �����ϸ� Direct3D�� VC++ ���â�� ����� �޽����� ����
	Int_Flag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	// ����̽��� ���� ������ ���� �˾ƺ��� ���� ����
	// �׸��� ���� ���� �������� ������ ����̽��� ���ü� �ִ����� ���� ����
	// D3D_FEATURE_LEVEL_11_0 : Direct2D API�� �Բ� ����� �� �ִ� ��ġ�� ����� ���� ����.
	D3D_FEATURE_LEVEL DeviceVersion = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	// HRESULT ������ 0 ���н� 0�� �ƴѼ� (long, int�� ����)
	// ����̽��� ���ؽ�Ʈ�� ����.
	//HRESULT D3D11CreateDevice() �Լ� ���� p.116
	// ��ġ����( �ʱ�ȭ )
	if (S_OK != D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
		nullptr, Int_Flag, nullptr, 0, D3D11_SDK_VERSION, &m_pDevice, &DeviceVersion, &m_pContext))
	{
		Release();
		return false;
	}

	// ��Ƽ�ٸ���� ����� ( ��Ƽ���ø�, ���ۻ��ø� )  -> �ٸ���� ������ �ذ��ϱ� ���� ���
	
	// 1. ���ۻ��ø�(�ʰ�ǥ��ȭ)
	// �ĸ� & ���� ���۸� ���� �ػ󵵺��� 4�� ũ�� -> �ĸ� ���ۿ� ������ 
	// -> �̹����� ȭ�鿡 ����� �� �ĸ� ���۸� ����ũ��� ȯ���Ϸ��� ��� -> �޸� �Һ��� ��û ŭ.
	// ��� ��û �ε巴��. (�Ų��� ��)
	//
	// 2. ��Ƽ���ø�(����ǥ��ȭ)  -> DX���� ���
	// ���ۻ��ø� ���� �޸� �Һ��� ����. ( ���� �������� ��û ������ �ƴ� ������ �� ���̰� ŭ )
	// ȿ���� ���ۻ��ø����� ����... 
	// �Ϻΰ�� ����� �κ� �ȼ��� ���̿��� �����ϱ� ������ ( �޸� �Һ��� ���� ���� )
	// ���ۻ��ø��� ���� ȭ�� �ػ��� 4���� �ĸ�,���� ���۸� ���.
	//  �̹��� ������ �� �κ��ȼ����� ����ϴ� ���� �ƴ϶� �ѹ��� ����Ͽ� ��ü������ ���������� ����
	//
	// �Ʒ��� ������ ����
	// �����Ͷ��������� ū�̹���->�����̹����� ���϶� ������ �Ͼ�µ�
	// �̰��� ���ø��̶�� �Ѵ�.
	// ��Ƽ ���ø��� ( �̸� �������� ������ �̹����� ����� ���� �˸°� ����ϴ°� )
	// ���� ��� 2048 ũ���� �̹����� ���� �� 
	// �̰� ������ �ɰ� �������� �̹����� �̸� ����� ���� ��� �������ÿ� �ش� ������ �˸°� �����ͼ� ����Ѵ�.
	//
	// ��Ƽ���ø��� ���ؼ� DXGI_SAMPLE_DESC��� ����ü�� ä�����Ѵ�.

	// UNORM
	// UNSIGEND NORMAL ( �ִ� ������ �������� 1�� �� �� �ִ�. )
	// m_MultiSampleLevel 1�� ���� ����
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
	// ��ȯ�罽 ����(����ü��)
	DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};


	// BufferDesc-> �����ϰ��� �ϴ� �ĸ� ������ �Ӽ��� �����ϴ� �������� ����ü
	SwapChainDesc.BufferDesc.Width = Window::Size().ix;			// �ĸ� ������ �ʺ�
	SwapChainDesc.BufferDesc.Height = Window::Size().iy;		// �ĸ� ������ ����

	// ����� �ֻ����� ���� ���� ���� Ƚ���� �ǹ�
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;			// ����		
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;			// �и�   -> 60/1  -> �׷��ϱ� 1�ʿ� 60��..!

	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// �ĸ� ������ �ȼ� ����, FXGI_FORMAT �������� ��, ������ �ؽ�ó �ڿ����� ����� �� �ִ� ���ĵ��� ����

	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	// ���÷��� ��ĵ���� ���, �̹��� ������ ���� ������ ������� ����
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;					// ���÷��� ��� ���, ���۸� â�� Ŭ���̾�Ʈ ������ �����ϴ� ���

	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// ������� �뵵

	// ��������� ����� ����~ ��

	// ����ü�� ��Ƽ���� ī��Ʈ & ����Ƽ
	// SampleDesc-> ��Ƽ���ø��� ���� ������ ǥ�� ������ ǰ�� ������ �����ϴ� ����ü
	SwapChainDesc.SampleDesc.Count = 1;		// �ȼ��� ������ ǥ���� ����
	SwapChainDesc.SampleDesc.Quality = 0;	// ���ϴ� ǰ�� ���� 

	SwapChainDesc.OutputWindow = Window::WindowHWND();		// ������ ����� ǥ���� â�� �ڵ� ( ����ü���� ������ ���� â�� �ڵ�)
	SwapChainDesc.BufferCount = 1;			// ����ü�ο��� ����� �ĸ� ������ ���� �Ϲ������� 1���� ���(���߹��۸�), 2���� ���߹��۸�.

	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;		// ���� ü���� �����ϴ� ����ü, DXGI_SWAP_EFFECT_DISCARD-> ���÷��� �����Ⱑ ���� ȿ������ ���� ����� �����ϰ� ��.

	// Windowed�� flase�� ��üȭ��, true�� ��üȭ�� �ƴ�
	SwapChainDesc.Windowed = !Window::IsSceneFullSize();		// â��� ���ϸ� true, ��üȭ���� false

	// ������ ������
	// ������ ������ ������ ���̰� ����ü���� ������� �ƴ�.
	// ����ü���� ����� ���� Dx11���� ������ ����� 
	// �����, ����̽��� �̿��ؼ��� ���� �� ����
	// ����ü�ο� ���� ��ü��
	// �Ϲ����� �ؽ�ó�� �Ž�ó�� �ܼ��ϰ� ����̽����� ����� �޴´ٰ� ���� �� �ִ� ���� �ƴ�
	// ����ü���� ����� ���ؼ��� ������ ���� �ؾ���.

	IDXGIDevice* pIdxGIDevice = nullptr;
	IDXGIAdapter* pIdxGIAdapter = nullptr;

	// ��ü�� ������� ���丮��ü 
	// ����ü���� �����ϴ� ����ü�� ������ٸ� �������� IDXGIFactory �ν��Ͻ��� ����
	// IDXGIFactory::CreateSwapChain �޼ҵ带 ȣ���ؼ� ����ü�� �������̽� (IDXGISwapChain)�� �����Ѵ�.
	IDXGIFactory* pIdxGIFactory = nullptr;


	// �İ�ü ��� 
	// ���ǿ� �޼ҵ�  (UUID, �����͸� ����Ű�� ������)  -> �� ���� ��Ŀ������ �ַ� ID3D11Device �������̽��� �Բ� ����
	// UUID (universally unique identifier, ���� ���� �ĺ���)
	// �߰����� ��ġ �������̽��� �ʿ��� ��Ȳ���� �����..
	// ���� ��� Direct3D ��ġ�κ��� ��ġ�� ����� �������̽��� ��ų� DXGI �������̽��� ����� �Ҷ� ..!�̰��� �����
	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pIdxGIDevice);

	if (nullptr == pIdxGIDevice)
	{
		EAssert(true);
		return false;
	}

	// pIdxGIDevice�� �̿��ؼ� ����ü���� ������ ���丮�� ����� �ִ� �����
	pIdxGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pIdxGIAdapter);

	if (nullptr == pIdxGIAdapter)
	{
		EAssert(true);
		return false;
	}

	// ���� IDXGIFactory �������̽��� ��Ե� 
	pIdxGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pIdxGIFactory);

	if (nullptr == pIdxGIFactory || S_OK != pIdxGIFactory->CreateSwapChain(m_pDevice, &SwapChainDesc, &m_pSwapChain))	// ����ü�� ����
	{
		EAssert(true);
		return false;
	}

	// ȹ���� COM �������̽����� ���� ( �ٻ�������ϱ�! )
	pIdxGIFactory->Release();
	pIdxGIAdapter->Release();
	pIdxGIDevice->Release();

	return true;
}

bool Device::Create_DepthStancilView()
{
	// ����ü���� ������µ� ����ü�ο� �ִ� ����� ���� �����ͼ� ���� �����Ҽ� �ְ� �ؾ� �Ѵ�.

	ID3D11Texture2D* pBackBufferTexture = nullptr;

	// m_pSwapChain->GetBuffer()�� �ĸ� ���۾��
	// GetBuffer �޼ҵ�� ����ü���� ����Ű�� �����͸� ��´�.
	// GetBuffer(0(�ĸ� ���۰� 1���� 0), ������ �������̽� ����, �ĸ���۸� ����Ű�� �����͸� ������ ��(��ȯ��)�� ������)
	if (S_OK != m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture))
	{
		EAssert(true);
		return false;
	}

	// ���� Ÿ�ٺ�� �ؽ�ó ���� ���� ���ϴ´�� ������ �� �ְ� ���ִ� ��ü.
	// ����Ÿ���̶� �׸������� �� ������ٰ� ����, �ű⿡ �� �׸��� ����.
	// �ĸ� ���۸� ������������ ��� ���ձ�(output merger) �ܰ迡 �������� �ĸ� ���ۿ� ���� ���� ��� �並 �����ؾ���.
	// ���� �並 �����ϴ� �޼ҵ� CreateRenderTargetView(���� ������� ����� �ڿ�, D3D11_RENDER_TARGET_VIEW_DESC ����ü�� ����Ű�� ������, ������ ������ ���� ����)
	// ������ �ĸ� ���ۿ� ���� ���� ��� �並 �����ϴ� ���̹Ƿ� ��� ���� �ĸ� ���۸� ��������.
	if (S_OK != m_pDevice->CreateRenderTargetView(pBackBufferTexture, 0, &m_pTargetView))
	{
		EAssert(true);
		return false;
	}

	// m_pSwapChain->GetBuffer()�� ȣ���ϸ� �ĸ� ���ۿ� ���� COM ���� Ƚ���� �����Ѵ�. �׷��� �� ����ϰ��� �����ؾ��Ѵ�.
	pBackBufferTexture->Release();

	// ����ü�κ䰡 ����� �ؽ�ó�� �����.
	// �Ʒ��� ���� ���� ������ ���ٽ� �ؽ���
	// 2���� �ؽ�ó�� ������ ������ ������ �ؽ�ó�� �����ϴ� D3D11_TEXTRUE2D_DESC ����ü�� ä���!
	// ID3D11Device::CreateTexture2D �޼ҵ带 ȣ���ؾ� �Ѵ�.
	D3D11_TEXTURE2D_DESC TextureDesc = {};

	TextureDesc.ArraySize = 1;							// �ؽ�ó �迭�� �ؽ�ó ����, ����&���ٽ� ������ ��쿡�� �ؽ�ó �ϳ��� �ʿ��ϴ�.
	TextureDesc.Width = (UINT)Window::Size().ix;		// �ؽ�ó�� �ʺ�(�ؼ� ����)
	TextureDesc.Height = (UINT)Window::Size().iy;		// �ؽ�ó�� ���� (�ؼ� ����)

	// ����Ʈ ũ�Ⱑ �߿�, ������ ó���ϴ� ���� ���� �߿����� ����
	// �Ʒ��� ���ٽǹ���.. 24��Ʈ�� ���̹��ۿ�.. 8��Ʈ�� ���ٽǹ��ۿ� ���.
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;		// ���� ���۸��� ���� �ؽ�ó ����, �ؼ��� ������ ���ϴ� �ʵ�, DXGI_FORMAT �������� ���� �� �ϳ��� �����Ѵ�.

	// SampleDesc : ��Ƽ���ø� ������ ǰ�� ������ �����ϴ� ����ü, ���� ��� ���� ������ ��ġ�ؾ���.
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.SampleDesc.Quality = 0;

	TextureDesc.MipLevels = 1;							// �Ӹ� ������ ����, ����&���ٽ� ������ ��쿡�� �ؽ�ó �ϳ��� �ʿ�

	// Usage : �ؽ�ó �뵵�� ���ϴ� �ʵ�,  D3D11_USAGE �������� ���� �� �ϳ��� ����.

	// 1. D3D11_USAGE_DEAFAULT   : �ڿ��� GPU�� �а� ��� �Ѵٸ� �̰ɷ� ����.  �̰ɷ� �����ϸ� GPU�� �ڿ��� �аų� �� �� ����.
	//							   ����&���ٽ� ���ۿ� ���� ��� �б� ����� GPU�� �����ϹǷ� ���� ���ٽ� ���۸� ���� �ؽ�ó�� ������ �� �̰� ���
	// 2. D3D11_USAGE_IMMUTABLE  : �ڿ��� ������ �Ŀ� �� ������ �ٲ��� ���� ��� �̰��� ���. �� �뵵�� �����ϸ� 
	//							   �ڿ��� GPU �б� ������ �ھ ��� ����ȭ�� ����������. 
	//							   CPU�� GPU�� �̷� �Һ� �ڿ��� �ڷḦ ����� �� ����. ( ���� ������ ���ܷ� �Ҷ� ), CPU�� �Ҹ� �ڿ��� �ڷḦ ���� �� ����.
	// 3. D3D11_USAGE_DYNAMIC    : ���� ���α׷�(CPU)�� �ڿ��� ������ ����ϰ� �����ؾ� �Ѵٸ� �̰��� ����.
	//							   �� �뵵�� ������ �ڿ��� GPU�� ���� �� �ְ� CPU�� ����� �� �ִ�. 
	//							   CPU�� �ڿ��� CPU���� ���� �����ϸ� ���ɻ� ������ ������ �� �ʿ��� ��찡 �ƴϸ� ����.
	// 4. D3D11_USAGE_STAGING    : ���� ���α׷�(CPU)�� �ڿ��� ���纻�� �о�� �Ѵٸ� �̰��� ���.
	//							   GPU���� CPU �޸𸮺���� ���� �����̱� ������ �̰� ���� �ʿ��� ��찡 �ƴϸ� ���ؾ��Ѵ�.
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;		

	// ���� ���Ľǿ� �ؽ���
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;   // 

	// CreateTexture2D ( ������ �ؽ�ó�� �����ϴ� ����ü, 
	//					�ؽ�ó�� ä�� �ʱ� �ڷḦ ����Ű�� �������� ���⼱ ���� ���ٽ� ���۷� ����ҰŴϱ� 0(null)���� �־���, 
	//					���� ���ٽ� ���۸� ����Ű�� �����͸� ���� �ؽ�ó )
	m_pDevice->CreateTexture2D(&TextureDesc, 0, &m_pDepthStencilTexture);

	if (nullptr == m_pDepthStencilTexture)
	{
		EAssert(true);
		return false;
	}


	// ���� ���Ľ�
	// CreateDepthStencilView( �並 �����ϰ��� �ϴ� �ڿ�,
	//						D3D11_DEPTH_STENCIL_VIEW_DESC ����ü�� ����Ű�� ��������(�ڿ��� ���� �ڷ� ����) ������ �����ؼ� �ڿ��� ���������� 0(null)���� �����ص���, 
	//						���� ���ٽ� �� )
	if (S_OK != m_pDevice->CreateDepthStencilView(m_pDepthStencilTexture, 0, &m_pDepthStencilView))
	{
		EAssert(true);
		return false;
	}

	// ���⿡ ������� �䰡 �����ܰ��� OutPutMerger���� ����� ���� ���� ����ؾ���.
	// ù��° ���� - �ѹ��� ��� ����Ÿ�� �並 ������ ���ΰ� (�츮�� 1��)
	// �׸��� ����Ÿ�� ����� �����͵��� �־��ֱ�
	// ���� ���۸� ���� ����� �ƴ϶� ���� ���۴� �ϳ��� �־��൵��.
	// ������������ ��� ���ձ� �ܰ迡 ���� ����.
	// OMSetRenderTargets(������ �ϴ� ���� ����� ����, 
	//						���������ο� ���� ���� ��� ����� ����Ű�� �����͵��� ���� �迭�� ù ���Ҹ� ����Ű�� ������,
	//						���������ο� ���� ���� ���ٽ� �並 ����Ű�� ������ ) 
	// ���� ��� ��� �������� �迭�� ��Ƽ� �ѹ��� ���� �� ������ ����&���ٽǺ�� �ϳ��� ���� �� ����.
	m_pContext->OMSetRenderTargets(1, &m_pTargetView, m_pDepthStencilView);

	return true;
}

// ����Ʈ ���� 

bool Device::Create_ViewPort()
{
	D3D11_VIEWPORT ViewPort = {};

	ViewPort.TopLeftX = 0;
	ViewPort.TopLeftY = 0;

	ViewPort.Width = (float)Window::Size().ix;
	ViewPort.Height = (float)Window::Size().iy;


	// Direct3D�� ����ϴ� ���� ���� ���� ������ 0~1�̶� �ּҰ��� 0 �ִ밪�� 1�̶�� ����( Ư��ȿ���� ���Ÿ� �ٸ��� ���� )
	ViewPort.MinDepth = 0;
	ViewPort.MaxDepth = 1;


	// ����Ʈ�� ����ü�� �� ä�� �Ŀ��� Context�� ����Ʈ�� �˷��ش�. '��'
	m_pContext->RSSetViewports(1, &ViewPort);		// (����Ʈ�� ����, ����Ʈ�迭�� ����Ű�� ������)

	return true;
}

// ȭ�� �����
void Device::ClearTarget()
{
	m_pContext->ClearRenderTargetView(m_pTargetView, m_Color.p_float);
	m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

// ȭ��  ��� 
void Device::SwapChain_Present()
{
	m_pSwapChain->Present(0, 0);
}



// �����Ͷ����� ������Ʈ
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
	NewRasterize->m_Desc.CullMode = _CullMode;						// �׳� ������ �� �׸���.
	NewRasterize->m_Desc.FillMode = _FillMode;						// SOLID�� �� ä����ä�� �׸��� ���̾�� �ܰ�����..

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

	// RS��°��� �پ��ִ°� ��� �����Ͷ����� �ܰ�� ������ �ִ� ���̴�. (�����Ͷ����� �ܰ迡�� ������)
	m_pContext->RSSetState(Find_RasterizeState->m_pState);
}