#include "pch_dx_11.h"

#include "DX11Renderer.h"

#include "Math\Transform.h"
#include "GameObjects\Camera.h"

DX11Renderer* DX11Renderer::instance = nullptr;

DX11Renderer::DX11Renderer()
    :device(nullptr), dc(nullptr), hWnd(0), windowInfo{}, swapChain(nullptr), rtv{}, depthStencilBuffer{},
    depthStencilView{}, rasterState{}, width(0), height(0)
{
}

DX11Renderer::~DX11Renderer()
{
    for (int i = 0; i < 3; i++)
    {
        SAFE_DELETE( rtts[i]);
    }

    rtv.Return();
    depthStencilBuffer.Return();
    depthStencilState.Return();
    rasterState.Return();
    blendState.Return();
    depthStencilView.Return();

    delete resources;

    TestDestructor();
    device->Release();
    dc->Release();
    swapChain->Release();
}

HRESULT DX11Renderer::Init()
{
    hWnd = GetActiveWindow();
    GetWindowInfo(hWnd, &windowInfo);

    OnResize();

    HRESULT hr = S_OK;

    hr = CreateDeviceAndSwapChain();
    if (hr != S_OK) return hr;

    resources = new Resources();

    hr = CreateRtv();
    if (hr != S_OK) return hr;

    hr = CreateAndSetDepthStencilView();
    if (hr != S_OK) return hr;

    hr = CreateAndSetRasterizerState();
    if (hr != S_OK) return hr;

    hr = CreateBlendState();
    if (hr != S_OK) return hr;

    SetViewPort();


    TestInit(); 

    return hr;
}

HRESULT DX11Renderer::CreateDeviceAndSwapChain()
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;//창모드 변경



    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = 0;

    D3D_FEATURE_LEVEL FeatureLevels = D3D_FEATURE_LEVEL_11_0;

    HRESULT hr = S_OK;
    D3D_FEATURE_LEVEL FeatureLevel;

    if (FAILED(hr = D3D11CreateDeviceAndSwapChain(NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        0,
        &FeatureLevels,
        1,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &swapChain,
        &device,
        &FeatureLevel,
        &dc)))
    {
        return hr;
    }

    return hr;
}

HRESULT DX11Renderer::CreateRtv()
{
    HRESULT hr = S_OK;

    rtv.Return();

    ID3D11Texture2D* backBuffer = nullptr;
    swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    if (FAILED(hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer)))
    {
        return hr;
    }

    resources->rtvs->CreateDefault(rtv, backBuffer);

    backBuffer->Release();
    backBuffer = nullptr;

    return hr;
}

HRESULT DX11Renderer::CreateAndSetDepthStencilView()
{
    HRESULT hr = S_OK;
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

    depthBufferDesc.Width = width;
    depthBufferDesc.Height = height;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    resources->texture2Ds->Create(depthStencilBuffer, depthBufferDesc);

    resources->depthStencilStates->GetDefault(depthStencilState);

    dc->OMSetDepthStencilState(depthStencilState, 1);

    resources->depthStencilViews->CreateDefault(depthStencilView, depthStencilBuffer);

    dc->OMSetRenderTargets(1, rtv, depthStencilView);//todo : 여기
    return hr;
}

HRESULT DX11Renderer::CreateAndSetRasterizerState()
{
    HRESULT hr = S_OK;

    resources->rasterStates->GetDefault(rasterState);

    dc->RSSetState(rasterState);

    return hr;
}

HRESULT DX11Renderer::CreateBlendState()
{
    HRESULT hr = S_OK;

    resources->blendStates->GetDefault(blendState);

    DC->OMSetBlendState(blendState, NULL, 0xFF);

    return hr;
}

void DX11Renderer::SetViewPort()
{
    D3D11_VIEWPORT viewPort = {};
    viewPort.Width = (float)width;
    viewPort.Height = (float)height;
    viewPort.MinDepth = 0.0f;
    viewPort.MaxDepth = 1.0f;
    viewPort.TopLeftX = 0.0f;
    viewPort.TopLeftY = 0.0f;

    dc->RSSetViewports(1, &viewPort);
}

void DX11Renderer::OnResize()
{
    if (this == nullptr) return;

    GetWindowInfo(hWnd, &windowInfo);
        width = windowInfo.rcClient.right - windowInfo.rcClient.left;
        height = windowInfo.rcClient.bottom - windowInfo.rcClient.top;

    if (dc != nullptr)
    {
        CreateRtv();
     
        CreateAndSetDepthStencilView();

         for (UINT i = 0; i < 3; i++)
             rtts[i]->OnResize();

        SetViewPort();
    }
    if (cam != nullptr)
        cam->OnResize(height, width);
}

void DX11Renderer::BeginRender()
{
    float color[4] = { 0.0f, 0.7f, 1.0f, 1.0f };
    float black[4] = { 0.0f,0.0f,0.0f,0.0f };//todo: 구차나서 일단 이렇게 해놓음

    dc->ClearRenderTargetView(rtv, color);
    dc->ClearRenderTargetView(rtts[0]->rtv, black);
    dc->ClearRenderTargetView(rtts[1]->rtv, black);
    dc->ClearRenderTargetView(rtts[2]->rtv, black);

    dc->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DX11Renderer::Render()
{
    PreRender();
    PostRender();
}

#include "Utility\ASELoader.h"

void DX11Renderer::TestInit()
{
    cam = new Camera(); 

    skyBox = new Skybox(cam);

    defferedRenderer = new DefferedRenderer();

    for (UINT i = 0; i < 3; i++)
        rtts[i] = new RenderTargetTexutre();

    canvas = new Canvas(2 / 3.0f, 0 / 3.0f, 1 / 3.0f, 1 / 3.0f);
    normalCanvas = new Canvas(2 / 3.0f, 1 / 3.0f, 1 / 3.0f, 1 / 3.0f);
    albedoCanvas = new Canvas(2 / 3.0f, 2 / 3.0f, 1 / 3.0f, 1 / 3.0f);

    grid = new Grid(cam);
    //////////////////////////
    defferedRenderer->AddRTT(rtts[0]);
    defferedRenderer->AddRTT(rtts[1]);
    defferedRenderer->AddRTT(rtts[2]);

    canvas->SetSRV(&rtts[0]->srv);
    normalCanvas->SetSRV(&rtts[1]->srv);
    albedoCanvas->SetSRV(&rtts[2]->srv);
    ///////////////////////
    ASELoader aseLoader;

    aseLoader.LoadASE("genji_max.ASE");
    aseLoader.CreateObjects(testObjects);

    aseLoader.LoadASE("babypig_idle_6x.ASE");
    aseLoader.CreateObjects(pig);

    aseLoader.LoadASE("03IK-Joe.ASE");
    aseLoader.CreateObjects(joe);
    ///////////////////////
    for (auto test : testObjects)
    {
        test->transform.scale *= 10.0f;
        test->transform.UpdateWorld();
    }

    for (auto test : joe)
    {
        if(test->material !=nullptr)   
            test->material->SetShader(L"NormalAsColorPixel.hlsl");
    }
}

void DX11Renderer::TestDestructor()
{
    SAFE_DELETE(cam);
    SAFE_DELETE(skyBox);
    SAFE_DELETE(defferedRenderer);
    SAFE_DELETE(canvas);
    SAFE_DELETE(normalCanvas);
    SAFE_DELETE(albedoCanvas);
    SAFE_DELETE(grid);

    for (auto test : testObjects)
    {
        SAFE_DELETE(test);
    }

    for (auto p : pig)
    {
        SAFE_DELETE(p);
    }

    for (auto p : joe)
    {
        SAFE_DELETE(p);
    }
}

void DX11Renderer::TestUpdate()
{
    cam->Update();
    skyBox->Update();
    canvas->Update();
    normalCanvas->Update();
    albedoCanvas->Update();
    defferedRenderer->Update();
    grid->Update();

    if (KEYBOARD->Press(VK_SPACE))
    for (auto test : testObjects)
    {
         test->transform.scale *= 1.01f;
    }

    if (KEYBOARD->Press(VK_F1))
    {
        for (auto test : testObjects)
        {
            test->material->SetShader(L"UVLightPixel.hlsl");
        }
    }
    if (KEYBOARD->Press(VK_F2))
    {
        for (auto test : testObjects)
        {
            test->material->SetShader(L"UVLightNormalMapPixel.hlsl");
        }
    }

    for (auto test : testObjects)
    {
        test->transform.rot.y += 0.001f;
        test->Update();
    }

    for (auto part : pig)
        part->Update();

    for (auto part : joe)
        part->Update();
}

void DX11Renderer::PreRender()
{
    cam->Render();
    ///배경
    //ID3D11RenderTargetView* a[] = { rtts[0]->testRTV, rtts[1]->testRTV, rtts[2]->testRTV };
    defferedRenderer->SetRenderTargets();
    //오브젝트들
    //dc->OMSetRenderTargets(3,a, depthStencilView);

    for (auto test : testObjects)
        test->Render();

    for (auto part : pig)
        part->Render();

    for (auto part : joe)
        part->Render();
}

void DX11Renderer::PostRender()
{
    //Post Render(DefferedRender)
    dc->OMSetRenderTargets(1, rtv, depthStencilView);//depthStencilView를 다른걸 쓰긴 해야할듯, 옵션도 다르게 줘야할까?

    canvas->Render();
    normalCanvas->Render();
    albedoCanvas->Render();

    skyBox->Render();
    grid->Render();

    defferedRenderer->Render();
    //main->Render();
}

void DX11Renderer::EndRender()
{
    swapChain->Present(0, 0);
}
