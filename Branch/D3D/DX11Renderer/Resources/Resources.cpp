#include "pch_dx_11.h"
#include "Resources.h"

Resources::Resources()
{
	vertexShaders = new VertexShaderResources();
	inputLayouts = new InputLayoutResources();
	pixelShaders = new PixelShaderResources();
	srvs = new SRVResources();
	samplerStates = new SamplerStateResources();
	rasterStates = new RasterizerStateResources();
	depthStencilStates = new DepthStencilStateResources();
	blendStates = new BlendStateResources();
	texture2Ds = new Texture2DResources();
	rtvs = new RenderTargetViewResources();
	depthStencilViews = new DepthStencilViewResources();
}

Resources::~Resources()
{
	vertexShaders->Release();
	inputLayouts->Release();
	srvs->Release();
	pixelShaders->Release();
	samplerStates->Release();
	rasterStates->Release();
	depthStencilStates->Release();
	blendStates->Release();
	texture2Ds->Release();
	rtvs->Release();
	depthStencilViews->Release();
}


ID3D11VertexShader* VertexShaderResources::Get(wstring shaderFileName)
{
	if (vertexShaders.find(shaderFileName) == vertexShaders.end())//해당하는 쉐이더가 없으면
	{
		HRESULT hr = DEVICE->CreateVertexShader(GetBlob(shaderFileName)->GetBufferPointer(), GetBlob(shaderFileName)->GetBufferSize(), NULL, &vertexShaders[shaderFileName]);

		assert(SUCCEEDED(hr));
	}//생성

	return vertexShaders[shaderFileName];
}

ID3DBlob* VertexShaderResources::GetBlob(wstring shaderFileName)
{
	if (vertexShaderBlobs.find(shaderFileName) == vertexShaderBlobs.end())//해당하는 쉐이더가 없으면
	{
		//D3D_COMPILE_STANDARD_FILE_INCLUDE == hlsl에서 #include 쓸 수 있게 해주는 놈
		HRESULT hr = D3DCompileFromFile(shaderFileName.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0",
			SHADERFLAG, 0, &vertexShaderBlobs[shaderFileName], &error);//todo : Vertex.hlsl 작성 안함

		assert(SUCCEEDED(hr));
	}//생성

	return vertexShaderBlobs[shaderFileName];
}

void VertexShaderResources::Release()
{
	for (auto shader : vertexShaderBlobs)
		shader.second->Release();
}

//ID3D11InputLayout* InputLayoutResources::Get(Vertex* vertex, wstring vertexShaderFileName)//가상함수테이블 포인터의 값을 이용해 RTTI를 한다.
//{
//	DWORD vftptr;//가상함수테이블 포인터
//	memcpy(&vftptr, vertex, sizeof(DWORD));
//	wstring p = to_wstring(vftptr) + vertexShaderFileName;
//	if (inputLayouts.find(p) == inputLayouts.end())//못 찾으면
//	{
//		HRESULT hr = DEVICE->CreateInputLayout(vertex->GetDesc(), vertex->GetDescSize(), RESOURCES->vertexShaders->GetBlob(vertexShaderFileName)->GetBufferPointer(),
//			RESOURCES->vertexShaders->GetBlob(vertexShaderFileName)->GetBufferSize(), &inputLayouts[p]);
//
//		assert(SUCCEEDED(hr));
//	}
//
//	return inputLayouts[p];
//}

//todo: 시멘틱 이름 더한걸로 바꿀까함

void InputLayoutResources::Get(Resource<ID3D11InputLayout>& dest, D3D11_INPUT_ELEMENT_DESC* desc, UINT descSize, wstring vertexShaderFileName)
{
	dest.Return();

	string key = "";
	for (UINT i = 0; i < descSize; i++)
		key += (string)desc[i].SemanticName;
	string vsName(vertexShaderFileName.begin(), vertexShaderFileName.end());
	key = key + vsName;

	if (inputLayouts.find(key) == inputLayouts.end())//못 찾으면
	{
		HRESULT hr = DEVICE->CreateInputLayout(desc, descSize, RESOURCES->vertexShaders->GetBlob(vertexShaderFileName)->GetBufferPointer(),
			RESOURCES->vertexShaders->GetBlob(vertexShaderFileName)->GetBufferSize(), &inputLayouts[key].data);

		assert(SUCCEEDED(hr));
	}//생성

	//있으면 그냥, 없으면 생성한 후
	dest.resource = inputLayouts[key].data;

	// 사용 횟수 늘려주고
	dest.Copy = [&, key]()
	{
		inputLayouts[key].refCount++;
	};
	dest.Copy();

	//리소스 반환 정의
	dest.Ret = [&, key]()
	{
		inputLayouts[key].refCount--;
		if (inputLayouts[key].refCount == 0)
		{
			ID3D11InputLayout* del = dest;
			inputLayouts.erase(key);
			SAFE_RELEASE(del);
		}
	};
}

void InputLayoutResources::Release()
{
	for (auto inputLayout : inputLayouts)
		SAFE_RELEASE(inputLayout.second.data);
}

ID3D11PixelShader* PixelShaderResources::Get(wstring shaderFileName)
{
	if (pixelShaders.find(shaderFileName) == pixelShaders.end())//해당하는 쉐이더가 없으면
	{
		HRESULT hr = DEVICE->CreatePixelShader(GetBlob(shaderFileName)->GetBufferPointer(), GetBlob(shaderFileName)->GetBufferSize(), NULL, &pixelShaders[shaderFileName]);

		assert(SUCCEEDED(hr));
	}//생성

	return pixelShaders[shaderFileName];
}

ID3DBlob* PixelShaderResources::GetBlob(wstring shaderFileName)
{
	if (pixelShaderBlobs.find(shaderFileName) == pixelShaderBlobs.end())//해당하는 쉐이더가 없으면
	{
		HRESULT hr = D3DCompileFromFile(shaderFileName.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0",
			SHADERFLAG, 0, &pixelShaderBlobs[shaderFileName], &error);//todo : Pixel.hlsl 작성 안함

		assert(SUCCEEDED(hr));
	}//생성

	return pixelShaderBlobs[shaderFileName];
}

void PixelShaderResources::Release()
{
	for (auto shader : pixelShaderBlobs)
		shader.second->Release();
}

void SRVResources::Create(Resource<ID3D11ShaderResourceView>& dest, D3D11_SHADER_RESOURCE_VIEW_DESC desc, ID3D11Resource* buffer)
{
	dest.Return();

	HRESULT hr = DEVICE->CreateShaderResourceView(buffer, &desc, dest);

	assert(SUCCEEDED(hr));

	//있으면 그냥, 없으면 생성한 후
	srvs[dest].data = dest.resource;

	// 사용 횟수 늘려주고
	dest.Copy = [&]()
	{
		srvs[dest].refCount++;
	};

	dest.Copy();

	//리소스 반환 정의
	dest.Ret = [&]()
	{
		srvs[dest].refCount--;
		if (srvs[dest].refCount == 0)
		{
			ID3D11ShaderResourceView* del = dest;
			if (srvs.find(del) != srvs.end())
				srvs.erase(del);
			SAFE_RELEASE(del);
		}
	};
}

void SRVResources::GetFromFile(Resource<ID3D11ShaderResourceView>& dest, wstring fileName)
{
	dest.Return();

	if (srvsFromTexture.find(fileName) == srvsFromTexture.end())//해당하는 srv가 없으면
	{
		ID3D11Resource* res;
		//todo : 확장자 체크
		HRESULT hr = CreateDDSTextureFromFile(DEVICE, fileName.c_str(), &res, &srvsFromTexture[fileName].data);

		res->Release();

		assert(SUCCEEDED(hr));
	}//생성

	//있으면 그냥, 없으면 생성한 후
	dest.resource = srvsFromTexture[fileName].data;

	// 사용 횟수 늘려주고
	dest.Copy = [&, fileName]()
	{
		srvsFromTexture[fileName].refCount++;
	};
	dest.Copy();

	//리소스 반환 정의
	dest.Ret = [&, fileName]()
	{
		srvsFromTexture[fileName].refCount--;
		if (srvsFromTexture[fileName].refCount == 0)
		{
			ID3D11ShaderResourceView* del = dest;
			srvsFromTexture.erase(fileName);
			SAFE_RELEASE(del);
		}
	};
}

void SRVResources::Release()
{
	for (auto srv : srvsFromTexture)
		SAFE_RELEASE(srv.second.data);

	for (auto srv : srvs)
		SAFE_RELEASE(srv.second.data);
}

void SamplerStateResources::Get(Resource<ID3D11SamplerState>& dest, D3D11_SAMPLER_DESC desc)
{
	dest.Return();

	string key;
	char token = '_';
	key = to_string(desc.AddressU) + token + to_string(desc.AddressV) + token + to_string(desc.AddressW) + token
		+ to_string(desc.BorderColor[0]) + token + to_string(desc.BorderColor[1]) + token
		+ to_string(desc.BorderColor[2]) + token + to_string(desc.BorderColor[3]) + token
		+ to_string(desc.ComparisonFunc) + token + to_string(desc.Filter)
		+ token + to_string(desc.MaxAnisotropy) + token + to_string(desc.MaxLOD)
		+ token + to_string(desc.MinLOD) + token + to_string(desc.MipLODBias);

	if (samplerStates.find(key) == samplerStates.end())//해당하는 srv가 없으면
	{
		HRESULT hr = DEVICE->CreateSamplerState(&desc, &samplerStates[key].data);

		assert(SUCCEEDED(hr));
	}//생성

	//있으면 그냥, 없으면 생성한 후
	dest.resource = samplerStates[key].data;


	dest.Copy = [&, key]()
	{
		// 사용 횟수 늘려주고
		samplerStates[key].refCount++;
	};
	dest.Copy();

	//리소스 반환 정의
	dest.Ret = [&, key]()
	{
		samplerStates[key].refCount--;
		if (samplerStates[key].refCount == 0)
		{
			ID3D11SamplerState* del = dest;
			samplerStates.erase(key);
			SAFE_RELEASE(del);
		}
	};
}

void SamplerStateResources::Release()
{
	for (auto samplerState : samplerStates)
		samplerState.second.data->Release();
}

void RasterizerStateResources::Get(Resource<ID3D11RasterizerState>& dest, D3D11_RASTERIZER_DESC desc)
{
	dest.Return();

	string key = "";
	char token = '_';
	key = to_string(desc.FillMode) + token
		+ to_string(desc.CullMode) + token
		+ to_string(desc.FrontCounterClockwise) + token
		+ to_string(desc.DepthBias) + token
		+ to_string(desc.DepthBiasClamp) + token
		+ to_string(desc.SlopeScaledDepthBias) + token
		+ to_string(desc.DepthClipEnable) + token
		+ to_string(desc.ScissorEnable) + token
		+ to_string(desc.MultisampleEnable) + token
		+ to_string(desc.AntialiasedLineEnable);


	if (rasterizerStates.find(key) == rasterizerStates.end())//해당하는 srv가 없으면
	{
		HRESULT hr = DEVICE->CreateRasterizerState(&desc, &rasterizerStates[key].data);

		assert(SUCCEEDED(hr));
	}//생성

	//있으면 그냥, 없으면 생성한 후
	dest.resource = rasterizerStates[key].data;

	dest.Copy = [&, key]()
	{
		// 사용 횟수 늘려주고
		rasterizerStates[key].refCount++;
	};
	dest.Copy();

	//리소스 반환 정의
	dest.Ret = [&, key]()
	{
		rasterizerStates[key].refCount--;
		if (rasterizerStates[key].refCount == 0)
		{
			ID3D11RasterizerState* del = dest;
			rasterizerStates.erase(key);
			SAFE_RELEASE(del);
		}
	};
}

void RasterizerStateResources::SetDefault(D3D11_RASTERIZER_DESC desc)
{
	defaultDesc = desc;
}

void RasterizerStateResources::Release()
{
	for (auto rasterizerState : rasterizerStates)
		rasterizerState.second.data->Release();
}

void DepthStencilStateResources::Get(Resource<ID3D11DepthStencilState>& dest, D3D11_DEPTH_STENCIL_DESC desc)
{
	dest.Return();

	string key = "";
	char token = '_';
	key = to_string(desc.DepthEnable) + token
		+ to_string(desc.DepthWriteMask) + token
		+ to_string(desc.DepthFunc) + token
		+ to_string(desc.StencilEnable) + token
		+ to_string(desc.StencilReadMask) + token
		+ to_string(desc.StencilWriteMask) + token
		+ to_string(desc.FrontFace.StencilFailOp) + token
		+ to_string(desc.FrontFace.StencilDepthFailOp) + token
		+ to_string(desc.FrontFace.StencilPassOp) + token
		+ to_string(desc.FrontFace.StencilFunc) + token
		+ to_string(desc.BackFace.StencilFailOp) + token
		+ to_string(desc.BackFace.StencilDepthFailOp) + token
		+ to_string(desc.BackFace.StencilPassOp) + token
		+ to_string(desc.BackFace.StencilFunc);

	if (depthStencilStates.find(key) == depthStencilStates.end())//해당하는 srv가 없으면
	{
		HRESULT hr = DEVICE->CreateDepthStencilState(&desc, &depthStencilStates[key].data);

		assert(SUCCEEDED(hr));
	}//생성

	//있으면 그냥, 없으면 생성한 후
	dest.resource = depthStencilStates[key].data;


	dest.Copy = [&, key]()
	{
		// 사용 횟수 늘려주고
		depthStencilStates[key].refCount++;
	};
	dest.Copy();

	//리소스 반환 정의
	dest.Ret = [&, key]()
	{
		depthStencilStates[key].refCount--;
		if (depthStencilStates[key].refCount == 0)
		{
			ID3D11DepthStencilState* del = dest;
			depthStencilStates.erase(key);
			SAFE_RELEASE(del);
		}
	};
}

void DepthStencilStateResources::SetDefault(D3D11_DEPTH_STENCIL_DESC desc)
{
	defaultDesc = desc;
}

void DepthStencilStateResources::Release()
{
	for (auto depthStencilState : depthStencilStates)
		depthStencilState.second.data->Release();
}

void BlendStateResources::Get(Resource<ID3D11BlendState>& dest, D3D11_BLEND_DESC desc)
{
	dest.Return();

	string key = "";
	char token = '_';
	key = to_string(desc.RenderTarget[0].BlendEnable) + token
		+ to_string(desc.RenderTarget[0].SrcBlend) + token
		+ to_string(desc.RenderTarget[0].DestBlend) + token
		+ to_string(desc.RenderTarget[0].SrcBlendAlpha) + token
		+ to_string(desc.RenderTarget[0].DestBlendAlpha) + token
		+ to_string(desc.RenderTarget[0].BlendOpAlpha) + token
		+ to_string(desc.RenderTarget[0].RenderTargetWriteMask);

	if (blendStates.find(key) == blendStates.end())//해당하는 데이터가 없으면 새로 생성
	{
		HRESULT hr = DEVICE->CreateBlendState(&desc, &blendStates[key].data);

		assert(SUCCEEDED(hr));
	}//생성

	//있으면 그냥, 없으면 생성한 후
	dest.resource = blendStates[key].data;

	dest.Copy = [&, key]()
	{
		// 사용 횟수 늘려주고
		blendStates[key].refCount++;
	};
	dest.Copy();

	//리소스 반환 정의
	dest.Ret = [&, key]()
	{
		blendStates[key].refCount--;
		if (blendStates[key].refCount == 0)
		{
			ID3D11BlendState* del = dest;
			blendStates.erase(key);
			SAFE_RELEASE(del);
		}
	};
}

void BlendStateResources::SetDefault(D3D11_BLEND_DESC desc)
{
	defaultDesc = desc;
}


void BlendStateResources::Release()
{
	for (auto blendState : blendStates)
		blendState.second.data->Release();
}

void Texture2DResources::Create(Resource<ID3D11Texture2D>& dest, D3D11_TEXTURE2D_DESC desc)
{
	dest.Return();

	ID3D11Texture2D* data;

	HRESULT hr = DEVICE->CreateTexture2D(&desc, NULL, dest);

	assert(SUCCEEDED(hr));


	texture2Ds[dest].data = dest;

	dest.Copy = [&]()
	{
		texture2Ds[dest].refCount++;
	};
	dest.Copy();

	dest.Ret = [&]()
	{
		texture2Ds[dest].refCount--;
		if (texture2Ds[dest].refCount == 0)
		{
			ID3D11Texture2D* del = dest;
			if (texture2Ds.find(del) != texture2Ds.end())
				texture2Ds.erase(del);
			SAFE_RELEASE(del);
		}
	};
}

void Texture2DResources::Release()
{
	for (auto texture2D : texture2Ds)
		SAFE_RELEASE(texture2D.second.data);
}

void RenderTargetViewResources::Create(Resource<ID3D11RenderTargetView>& dest, D3D11_RENDER_TARGET_VIEW_DESC desc, ID3D11Resource* buffer)
{
	dest.Return();

	HRESULT hr = DEVICE->CreateRenderTargetView(buffer, &desc, dest);

	assert(SUCCEEDED(hr));

	rtvs[dest].data = dest;

	dest.Copy = [&]()
	{
		rtvs[dest].refCount++;
	};
	dest.Copy();

	dest.Ret = [&]()
	{
		rtvs[dest].refCount--;
		if (rtvs[dest].refCount == 0)
		{
			ID3D11RenderTargetView* del = dest;
			if (rtvs.find(del) != rtvs.end())
				rtvs.erase(del);
			SAFE_RELEASE(del);
		}
	};
}
//RTT 클래스 내부에서 한번 더 관리해주는 걸로 하장

void RenderTargetViewResources::Release()
{
	for (auto rtv : rtvs)
		SAFE_RELEASE(rtv.second.data);
}

void DepthStencilViewResources::Create(Resource<ID3D11DepthStencilView>& dest, D3D11_DEPTH_STENCIL_VIEW_DESC desc, ID3D11Resource* buffer)
{
	dest.Return();

	HRESULT hr = DEVICE->CreateDepthStencilView(buffer, &desc, dest);

	assert(SUCCEEDED(hr));

	depthStencilViews[dest].data = dest;

	dest.Copy = [&]()
	{
		depthStencilViews[dest].refCount++;
	};
	dest.Copy();

	dest.Ret = [&]()
	{
		depthStencilViews[dest].refCount--;
		if (depthStencilViews[dest].refCount == 0)
		{
			ID3D11DepthStencilView* del = dest;
			if (depthStencilViews.find(del) != depthStencilViews.end())
				depthStencilViews.erase(del);
			SAFE_RELEASE(del);
		}
	};
}

void DepthStencilViewResources::Release()
{
	for (auto rtv : depthStencilViews)
		SAFE_RELEASE(rtv.second.data);
}
