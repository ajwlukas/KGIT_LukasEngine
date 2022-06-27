#pragma once
/// <summary>
/// ��� ���ҽ��� unordered_map���� �����Ѵ�.
/// �������̸� �����ڳ�, Init()���� ������ �ѹ��� ȣ���ؼ� �޾Ƽ� ����ϱ� ����(�˻� �ѹ��� �ϵ���)
/// 
/// </summary>
#include "DX11Renderer.h"
#include "Meshes/Vertex.h"

//class VertexShader ������ �𸣰����� static���� ������ ���� �ʴ´�(��Ŀ����). �ؼ�.. �̱������� ���� �� ��
//{
//public:
//	static ID3DBlob* Get(wstring shaderFileName);
//
//	static void Release();
//private:
//	static unordered_map<wstring, ID3DBlob*> vertexShaders;
//	static ID3DBlob* error;
//};

#ifdef _DEBUG
#define SHADERFLAG D3DCOMPILE_DEBUG | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR |\
D3DCOMPILE_PARTIAL_PRECISION | D3DCOMPILE_OPTIMIZATION_LEVEL0

#else
#define SHADERFLAG D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR |\
D3DCOMPILE_PARTIAL_PRECISION | D3DCOMPILE_OPTIMIZATION_LEVEL3
#endif

class DX11Renderer;

class InputLayoutResources;

class VertexShaderResources;
class PixelShaderResources;

class SRVResources;

class SamplerStateResources;
class RasterizerStateResources;
class DepthStencilStateResources;
class BlendStateResources;

class Resources//��� �ڿ� �Ŵ����� �����ִ� Ŭ����
{
public:
	Resources();
	~Resources();
public:
	VertexShaderResources* vertexShaders;
	InputLayoutResources* inputLayouts;
	PixelShaderResources* pixelShaders;
	SRVResources* srvs;
	SamplerStateResources* samplerStates;
	RasterizerStateResources* rasterStates;
	DepthStencilStateResources* depthStencilStates;
	BlendStateResources* blendStates;
};

class VertexShaderResources
{
public:
	ID3D11VertexShader* Get(wstring shaderFileName);

	ID3DBlob* GetBlob(wstring shaderFileName);

	void Release();

private:
	unordered_map<wstring, ID3D11VertexShader*> vertexShaders;
	unordered_map<wstring, ID3DBlob*> vertexShaderBlobs;
	ID3DBlob* error = nullptr;
};

class InputLayoutResources
{
public:
	ID3D11InputLayout* Get(D3D11_INPUT_ELEMENT_DESC* desc, UINT descSize, wstring vertexShaderFileName);

	void Release();

private:
	unordered_map<wstring, ID3D11InputLayout*> inputLayouts;
} ;

class PixelShaderResources
{
public:
	ID3D11PixelShader* Get(wstring shaderFileName);
	ID3DBlob* GetBlob(wstring shaderFileName);

	void Release();

private:
	unordered_map<wstring, ID3D11PixelShader*> pixelShaders;
	unordered_map<wstring, ID3DBlob*> pixelShaderBlobs;
	ID3DBlob* error;
};

class SRVResources
{
public:
	ID3D11ShaderResourceView** Get(wstring fileName);

	void Release();

private:
	unordered_map<wstring, ID3D11ShaderResourceView*> srvs;
};


/// <summary>
/// States for DX
/// </summary>
class SamplerStateResources
{
public:
	ID3D11SamplerState** Get(D3D11_SAMPLER_DESC desc);

	void Release();

private:
	unordered_map<string, ID3D11SamplerState*> samplerStates;
};

class RasterizerStateResources
{
public:
	ID3D11RasterizerState* Get(D3D11_RASTERIZER_DESC desc);

	void SetDefault(D3D11_RASTERIZER_DESC desc);
	ID3D11RasterizerState* GetDefault() { return rasterizerStates.at(defaultStateKey); }

	void Release();

private:
	string defaultStateKey;
	unordered_map<string, ID3D11RasterizerState*> rasterizerStates;
};

class DepthStencilStateResources
{
public:
	ID3D11DepthStencilState* Get(D3D11_DEPTH_STENCIL_DESC desc);

	void SetDefault(D3D11_DEPTH_STENCIL_DESC desc);
	ID3D11DepthStencilState* GetDefault() { return depthStencilStates.at(defaultStateKey); }

	void Release();
private:
	string defaultStateKey;
	unordered_map<string, ID3D11DepthStencilState*> depthStencilStates;
};

class BlendStateResources
{
public:
	ID3D11BlendState* Get(D3D11_BLEND_DESC desc);

	void SetDefault(D3D11_BLEND_DESC desc);
	ID3D11BlendState* GetDefault() { return blendStates.at(defaultStateKey); }

	void Release();
private:
	string defaultStateKey;
	unordered_map<string, ID3D11BlendState*> blendStates;
};


