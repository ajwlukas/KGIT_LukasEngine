#include "pch_dx_11.h"

#include "DX11Renderer.h"

#include "Math\Transform.h"
#include "GameObjects\Camera.h"

DX11Renderer* DX11Renderer::instance = nullptr;

DX11Renderer::DX11Renderer()
    :device(nullptr), dc(nullptr),hWnd(0), windowInfo{}, swapChain(nullptr), rtv(nullptr), depthStencilBuffer(nullptr),
    depthStencilState(nullptr), depthStencilView(nullptr), rasterState(nullptr), width(0), height(0)
{
}

DX11Renderer::~DX11Renderer()
{
    TestDestructor();
    delete resources;
    device->Release();
    dc->Release();
    swapChain->Release();
    rtv->Release();
    depthStencilBuffer->Release();
    depthStencilView->Release();
}

//https://copynull.tistory.com/240?category=649932
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
    TestCreateRTT();

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

    ID3D11Texture2D* backBuffer = nullptr;
    if (FAILED(hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer)))
    {
        return hr;
    }
   
    if (FAILED(hr = device->CreateRenderTargetView(backBuffer, NULL, &rtv)))//todo: 여기가 swapchain에 rtv 꽂는 쪽인 듯
    {
        return hr;
    }

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

    if (FAILED(hr = device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer)))
    {
        return hr;
    }

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
 
    resources->depthStencilStates->SetDefault(depthStencilDesc);
    depthStencilState = resources->depthStencilStates->GetDefault();

    dc->OMSetDepthStencilState(depthStencilState, 1);

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    if (FAILED(hr = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView)))
    {
        return hr;
    }

    dc->OMSetRenderTargets(1, &rtv, depthStencilView);//todo : 여기
    return hr;
}

HRESULT DX11Renderer::CreateAndSetRasterizerState()
{
    HRESULT hr = S_OK;

    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    //rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    resources->rasterStates->SetDefault(rasterDesc);
    rasterState = resources->rasterStates->GetDefault();

    dc->RSSetState(rasterState);

    return hr;
}

HRESULT DX11Renderer::CreateBlendState()
{
    HRESULT hr = S_OK;

    D3D11_BLEND_DESC desc = {};
    desc.RenderTarget[0].BlendEnable = TRUE;
    desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

    blendState = resources->blendStates->Get(desc);

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
   //바꿔줘야 하는 것 swapChain, depthStencilBuffer, viewPort, projectionBuffer;

    if (this == nullptr) return;

    GetWindowInfo(hWnd, &windowInfo);
        width = windowInfo.rcClient.right - windowInfo.rcClient.left;
        height = windowInfo.rcClient.bottom - windowInfo.rcClient.top;

    if (dc != nullptr)
    {
        rtv->Release();
        depthStencilView->Release();
        depthStencilBuffer->Release();

        ID3D11Texture2D* backBuffer = nullptr;
        swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
         swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
         if(backBuffer != nullptr)
             device->CreateRenderTargetView(backBuffer, nullptr, &rtv);

         backBuffer->Release();
         CreateAndSetDepthStencilView();


          testTexture->Release();
          testSRV->Release();
          testRTV->Release();
         TestCreateRTT();

        SetViewPort();
    }
    if (cam != nullptr)
        cam->OnResize(height, width);
}

void DX11Renderer::BeginRender()
{
    float color[4] = { 0.0f, 0.7f, 1.0f, 1.0f };

    dc->ClearRenderTargetView(rtv, color);

    dc->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

}

#include "Utility\ASELoader.h"

void DX11Renderer::TestInit()
{
    cam = new Camera(); 
    skyBox = new Skybox(cam);
    main = new Canvas(0.0f, 0.0f, 1.0f, 1.0f);
    canvas = new Canvas(2 / 3.0f, 0 / 3.0f, 1 / 3.0f, 1 / 3.0f);
    grid = new Grid(cam);
    //main = new Canvas(0.0f, 0.0f, 1.0f, 1.0f);
    //canvas->transform.pos.z -= 1.0f;
    main->SetSRV(&testSRV);
    canvas->SetSRV(&testSRV);

    ///////////////////////
    ASELoader aseLoader;

    aseLoader.LoadASE("genji_max.ASE");
    aseLoader.CreateObjects(testObjects);

    aseLoader.LoadASE("babypig_idle_6x.ASE");
    aseLoader.CreateObjects(pig);

    aseLoader.LoadASE("03IK-Joe.ASE");
    aseLoader.CreateObjects(joe);

    ///////////////////////

    //testObjects[0]->mesh->SetShader()

    for (auto test : testObjects)
    {
        test->transform.scale *= 10.0f;
        //test->transform.scale /= 100.0f;

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
    SAFE_DELETE(grid);
    SAFE_DELETE(main);
    SAFE_DELETE(canvas);
    SAFE_DELETE(cam);
    SAFE_DELETE(skyBox);

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
    main->Update();
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

void DX11Renderer::TestRender()
{
    dc->OMSetRenderTargets(1, &testRTV, depthStencilView);
    ///
    cam->Render();
    skyBox->Render();

    for (auto test : testObjects)
        test->Render();

    for (auto part : pig)
        part->Render();

    for (auto part : joe)
        part->Render();

    grid->Render();
    ///
    dc->OMSetRenderTargets(1, &rtv, depthStencilView);//depthStencilView를 다른걸 쓰긴 해야할듯, 옵션도 다르게 줘야할까?
    ///
    canvas->Render();
    main->Render();

}

void DX11Renderer::PostRender()
{
}

void DX11Renderer::TestCreateRTT()
{ /// test code
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    device->CreateTexture2D(&desc, NULL, &testTexture);

    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.Format = desc.Format;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Texture2D.MipSlice = 0;

    device->CreateRenderTargetView(testTexture, &rtvDesc, &testRTV);


    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;


    device->CreateShaderResourceView(testTexture, &srvDesc, &testSRV);
}

void DX11Renderer::EndRender()
{
    swapChain->Present(0, 0);
}
