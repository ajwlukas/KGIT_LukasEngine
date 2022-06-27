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
}


ID3D11VertexShader* VertexShaderResources::Get(wstring shaderFileName)
{
	if (vertexShaders.find(shaderFileName) == vertexShaders.end())//�ش��ϴ� ���̴��� ������
	{
		HRESULT hr = DEVICE->CreateVertexShader(GetBlob(shaderFileName)->GetBufferPointer(), GetBlob(shaderFileName)->GetBufferSize(), NULL, &vertexShaders[shaderFileName]);

		assert(SUCCEEDED(hr));
	}//����

	return vertexShaders[shaderFileName];
}

ID3DBlob* VertexShaderResources::GetBlob(wstring shaderFileName)
{
	if (vertexShaderBlobs.find(shaderFileName) == vertexShaderBlobs.end())//�ش��ϴ� ���̴��� ������
	{
		//D3D_COMPILE_STANDARD_FILE_INCLUDE == hlsl���� #include �� �� �ְ� ���ִ� ��
		HRESULT hr = D3DCompileFromFile(shaderFileName.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0",
			SHADERFLAG, 0, &vertexShaderBlobs[shaderFileName], &error);//todo : Vertex.hlsl �ۼ� ����

		assert(SUCCEEDED(hr));
	}//����

	return vertexShaderBlobs[shaderFileName];
}

void VertexShaderResources::Release()
{
	for (auto shader : vertexShaderBlobs)
		shader.second->Release();
}

//ID3D11InputLayout* InputLayoutResources::Get(Vertex* vertex, wstring vertexShaderFileName)//�����Լ����̺� �������� ���� �̿��� RTTI�� �Ѵ�.
//{
//	DWORD vftptr;//�����Լ����̺� ������
//	memcpy(&vftptr, vertex, sizeof(DWORD));
//	wstring p = to_wstring(vftptr) + vertexShaderFileName;
//	if (inputLayouts.find(p) == inputLayouts.end())//�� ã����
//	{
//		HRESULT hr = DEVICE->CreateInputLayout(vertex->GetDesc(), vertex->GetDescSize(), RESOURCES->vertexShaders->GetBlob(vertexShaderFileName)->GetBufferPointer(),
//			RESOURCES->vertexShaders->GetBlob(vertexShaderFileName)->GetBufferSize(), &inputLayouts[p]);
//
//		assert(SUCCEEDED(hr));
//	}
//
//	return inputLayouts[p];
//}

//todo: �ø�ƽ �̸� ���Ѱɷ� �ٲܱ���
ID3D11InputLayout* InputLayoutResources::Get(D3D11_INPUT_ELEMENT_DESC* desc, UINT descSize, wstring vertexShaderFileName)
{
	DWORD descAddr = (DWORD)desc;//inputElementDesc�� �ּ� static�̶� ����
	wstring p = to_wstring(descAddr) + vertexShaderFileName;
	if (inputLayouts.find(p) == inputLayouts.end())//�� ã����
	{
		HRESULT hr = DEVICE->CreateInputLayout(desc, descSize, RESOURCES->vertexShaders->GetBlob(vertexShaderFileName)->GetBufferPointer(),
			RESOURCES->vertexShaders->GetBlob(vertexShaderFileName)->GetBufferSize(), &inputLayouts[p]);

		assert(SUCCEEDED(hr));
	}

	return inputLayouts[p];
}

void InputLayoutResources::Release()
{
	for (auto p : inputLayouts)
		p.second->Release();
}

ID3D11PixelShader* PixelShaderResources::Get(wstring shaderFileName)
{
	if (pixelShaders.find(shaderFileName) == pixelShaders.end())//�ش��ϴ� ���̴��� ������
	{
		HRESULT hr = DEVICE->CreatePixelShader(GetBlob(shaderFileName)->GetBufferPointer(), GetBlob(shaderFileName)->GetBufferSize(), NULL, &pixelShaders[shaderFileName]);

		assert(SUCCEEDED(hr));
	}//����

	return pixelShaders[shaderFileName];
}

ID3DBlob* PixelShaderResources::GetBlob(wstring shaderFileName)
{
	if (pixelShaderBlobs.find(shaderFileName) == pixelShaderBlobs.end())//�ش��ϴ� ���̴��� ������
	{
		HRESULT hr = D3DCompileFromFile(shaderFileName.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0",
			SHADERFLAG, 0, &pixelShaderBlobs[shaderFileName], &error);//todo : Pixel.hlsl �ۼ� ����

		assert(SUCCEEDED(hr));
	}//����

	return pixelShaderBlobs[shaderFileName];
}

void PixelShaderResources::Release()
{
	for (auto shader : pixelShaderBlobs)
		shader.second->Release();
}

ID3D11ShaderResourceView** SRVResources::Get(wstring fileName)
{
	if (srvs.find(fileName) == srvs.end())//�ش��ϴ� srv�� ������
	{
		ID3D11Resource* res;
		//todo : Ȯ���� üũ
		HRESULT hr = CreateDDSTextureFromFile(DEVICE, fileName.c_str(), &res, &srvs[fileName]);

		res->Release();

		assert(SUCCEEDED(hr));
	}//����

	return &(srvs[fileName]);
}

void SRVResources::Release()
{
	for (auto srv : srvs)
		srv.second->Release();
}

//ID3D11SamplerState** SamplerStateResources::Get(D3D11_TEXTURE_ADDRESS_MODE mode)
//{
//	if (samplerStates.find(mode) == samplerStates.end())//�ش��ϴ� srv�� ������
//	{
//		D3D11_SAMPLER_DESC desc;
//		ZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));
//		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
//		desc.AddressU = mode;//U
//		desc.AddressV = mode;//V
//		desc.AddressW = mode;//W
//
//		HRESULT hr = DEVICE->CreateSamplerState(&desc, &samplerStates[mode]);
//
//		assert(SUCCEEDED(hr));
//	}//����
//
//	return &(samplerStates[mode]);
//}

ID3D11SamplerState** SamplerStateResources::Get(D3D11_SAMPLER_DESC desc)
{
	string key;
	char token = '_';
	key = to_string(desc.AddressU) + token + to_string(desc.AddressV) + token + to_string(desc.AddressW) + token
		+ to_string(desc.BorderColor[0]) + token + to_string(desc.BorderColor[1]) + token
		+ to_string(desc.BorderColor[2]) + token + to_string(desc.BorderColor[3]) + token
		+ to_string(desc.ComparisonFunc) + token + to_string(desc.Filter)
		+ token + to_string(desc.MaxAnisotropy) + token + to_string(desc.MaxLOD)
		+ token + to_string(desc.MinLOD) + token + to_string(desc.MipLODBias);
	if (samplerStates.find(key) == samplerStates.end())//�ش��ϴ� srv�� ������
	{
		HRESULT hr = DEVICE->CreateSamplerState(&desc, &samplerStates[key]);

		assert(SUCCEEDED(hr));
	}//����

	return &(samplerStates[key]);
}

void SamplerStateResources::Release()
{
	for (auto samplerState : samplerStates)
		samplerState.second->Release();
}


ID3D11RasterizerState* RasterizerStateResources::Get(D3D11_RASTERIZER_DESC desc)
{
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
		

	if (rasterizerStates.find(key) == rasterizerStates.end())//�ش��ϴ� srv�� ������
	{
		HRESULT hr = DEVICE->CreateRasterizerState(&desc, &rasterizerStates[key]);

		assert(SUCCEEDED(hr));
	}//����

	return rasterizerStates[key];
}

void RasterizerStateResources::SetDefault(D3D11_RASTERIZER_DESC desc)
{
	char token = '_';
	defaultStateKey = to_string(desc.FillMode) + token
		+ to_string(desc.CullMode) + token
		+ to_string(desc.FrontCounterClockwise) + token
		+ to_string(desc.DepthBias) + token
		+ to_string(desc.DepthBiasClamp) + token
		+ to_string(desc.SlopeScaledDepthBias) + token
		+ to_string(desc.DepthClipEnable) + token
		+ to_string(desc.ScissorEnable) + token
		+ to_string(desc.MultisampleEnable) + token
		+ to_string(desc.AntialiasedLineEnable);

	Get(desc);//register
}

void RasterizerStateResources::Release()
{
	for (auto rasterizerState : rasterizerStates)
		rasterizerState.second->Release();
}


ID3D11DepthStencilState* DepthStencilStateResources::Get(D3D11_DEPTH_STENCIL_DESC desc)
{
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

	if (depthStencilStates.find(key) == depthStencilStates.end())//�ش��ϴ� srv�� ������
	{
		HRESULT hr = DEVICE->CreateDepthStencilState(&desc, &depthStencilStates[key]);

		assert(SUCCEEDED(hr));
	}//����

	return depthStencilStates[key];
}

void DepthStencilStateResources::SetDefault(D3D11_DEPTH_STENCIL_DESC desc)
{
	char token = '_';
	defaultStateKey = to_string(desc.DepthEnable) + token
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

	Get(desc);//register
}

void DepthStencilStateResources::Release()
{
	for (auto depthStencilState : depthStencilStates)
		depthStencilState.second->Release();
}

ID3D11BlendState* BlendStateResources::Get(D3D11_BLEND_DESC desc)
{
	string key = "";
	char token = '_';
	key = to_string(desc.RenderTarget[0].BlendEnable) + token
		+ to_string(desc.RenderTarget[0].SrcBlend) + token
		+ to_string(desc.RenderTarget[0].DestBlend) + token
		+ to_string(desc.RenderTarget[0].SrcBlendAlpha) + token
		+ to_string(desc.RenderTarget[0].DestBlendAlpha) + token
		+ to_string(desc.RenderTarget[0].BlendOpAlpha) + token
		+ to_string(desc.RenderTarget[0].RenderTargetWriteMask);

	if (blendStates.find(key) == blendStates.end())//�ش��ϴ� srv�� ������
	{
		HRESULT hr = DEVICE->CreateBlendState(&desc, &blendStates[key]);

		assert(SUCCEEDED(hr));
	}//����

	return blendStates[key];
}

void BlendStateResources::SetDefault(D3D11_BLEND_DESC desc)
{
	string key = "";
	char token = '_';
	key = to_string(desc.RenderTarget[0].BlendEnable) + token
		+ to_string(desc.RenderTarget[0].SrcBlend) + token
		+ to_string(desc.RenderTarget[0].DestBlend) + token
		+ to_string(desc.RenderTarget[0].SrcBlendAlpha) + token
		+ to_string(desc.RenderTarget[0].DestBlendAlpha) + token
		+ to_string(desc.RenderTarget[0].BlendOpAlpha) + token
		+ to_string(desc.RenderTarget[0].RenderTargetWriteMask);

	Get(desc);//regist
}

void BlendStateResources::Release()
{
	for (auto blendState : blendStates)
		blendState.second->Release();
}
