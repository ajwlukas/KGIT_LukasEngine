#pragma once
#include "Resource.h"
/// <summary>
/// �������̸� �����ڳ�, Init()���� ������ �ѹ��� ȣ���ؼ� �޾Ƽ� ����ϱ� ����(�˻� �ѹ��� �ϵ���)
/// Resource<ResourceType>���� ���, ��� �� Return() ���� ��
/// written by Ssreppa 22.06.28
/// 
/// Get()�Լ��� Create()�Լ� ����, Get()�� ���� �ɼ��̸� �ִ��� ��,
/// Create()�� ���� �ɼ��̾ ���� ����� ��
/// �� �̻� Return()���� �ʿ� ����, �� �����ϰ� ����� ���� ��, �Ҹ��� ���� ���ص� ��
/// written by Ssreppa 22.07.01
/// 
/// 
///template <typename T>
///class Resource
///{
///public:
///	operator T* () { return resource; }
///	operator T** () { return &resource; }
///
///	T* resource;
///
///	std::function<void()> Return;
///};
/// </summary>

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

class Texture2DResources;
class RenderTargetViewResources;
class DepthStencilViewResources;

//todo : refCount �ؼ� �Ⱦ��� ���ҽ� �ڵ� ���� �ϵ��� ���� ��

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
	Texture2DResources* texture2Ds;
	RenderTargetViewResources* rtvs;
	DepthStencilViewResources* depthStencilViews;
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
	void Get(Resource< ID3D11InputLayout>& dest, D3D11_INPUT_ELEMENT_DESC* desc, UINT descSize, wstring vertexShaderFileName);

private:
	friend class Resources;
	void Release();

	struct Data
	{
		Data() : data(nullptr), refCount(0) {}
		ID3D11InputLayout* data;
		UINT refCount;
	};
	unordered_map<string, Data> inputLayouts;
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
	void Create(Resource<ID3D11ShaderResourceView>& dest, D3D11_SHADER_RESOURCE_VIEW_DESC desc, ID3D11Resource* buffer);
	void GetFromFile(Resource< ID3D11ShaderResourceView>& dest, wstring fileName);

private:
	friend class Resources;
	void Release();

	struct Data
	{
		Data() : data(nullptr), refCount(0) {}
		ID3D11ShaderResourceView* data;
		UINT refCount;
	};
	unordered_map<ID3D11ShaderResourceView*, Data> srvs;
	unordered_map<wstring, Data> srvsFromTexture;
};

/// States for DX
class SamplerStateResources
{
public:
	void Get(Resource< ID3D11SamplerState>& dest, D3D11_SAMPLER_DESC desc);

private:
	friend class Resources;
	void Release();

	struct Data
	{
		Data() : data(nullptr), refCount(0) {}
		ID3D11SamplerState* data;
		UINT refCount;
	};
	unordered_map<string, Data> samplerStates;
};

class RasterizerStateResources
{
public:
	RasterizerStateResources()
	{
		D3D11_RASTERIZER_DESC desc = {};
		desc.AntialiasedLineEnable = false;
		desc.CullMode = D3D11_CULL_BACK;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0.0f;
		desc.DepthClipEnable = true;
		desc.FillMode = D3D11_FILL_SOLID;
		//rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
		desc.FrontCounterClockwise = false;
		desc.MultisampleEnable = false;
		desc.ScissorEnable = false;
		desc.SlopeScaledDepthBias = 0.0f;

		SetDefault(desc);
	}

	void Get(Resource< ID3D11RasterizerState>& dest, D3D11_RASTERIZER_DESC desc);

	void GetDefault(Resource< ID3D11RasterizerState>& dest) { Get(dest, defaultDesc); }
	void SetDefault(D3D11_RASTERIZER_DESC desc);

private:
	friend class Resources;
	void Release();

	struct Data
	{
		Data() : data(nullptr), refCount(0) {}
		ID3D11RasterizerState* data;
		UINT refCount;
	};
	D3D11_RASTERIZER_DESC defaultDesc;
	unordered_map<string, Data> rasterizerStates;
};

class DepthStencilStateResources
{
public:
	DepthStencilStateResources()
	{
		D3D11_DEPTH_STENCIL_DESC desc = {};

		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS;

		desc.StencilEnable = true;
		desc.StencilReadMask = 0xFF;
		desc.StencilWriteMask = 0xFF;

		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		SetDefault(desc);
	}
	void Get(Resource< ID3D11DepthStencilState>& dest,D3D11_DEPTH_STENCIL_DESC desc);

	void GetDefault(Resource< ID3D11DepthStencilState>& dest) { Get(dest, defaultDesc); }
	void SetDefault(D3D11_DEPTH_STENCIL_DESC desc);

private:
	friend class Resources;
	void Release();

	struct Data
	{
		Data() : data(nullptr), refCount(0) {}
		ID3D11DepthStencilState* data;
		UINT refCount;
	};
	D3D11_DEPTH_STENCIL_DESC defaultDesc;
	unordered_map<string, Data> depthStencilStates;
};

class BlendStateResources
{
public:
	BlendStateResources()
	{
		D3D11_BLEND_DESC desc = {};
		desc.RenderTarget[0].BlendEnable = TRUE;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		SetDefault(desc);
	}
	void Get(Resource<ID3D11BlendState>& dest, D3D11_BLEND_DESC desc);

	void GetDefault(Resource<ID3D11BlendState>& dest) { Get(dest, defaultDesc); };
	void SetDefault(D3D11_BLEND_DESC desc);

private:
	friend class Resources;
	void Release();

	struct Data
	{
		Data() : data(nullptr), refCount(0) {}
		ID3D11BlendState* data;
		UINT refCount;
	};
	D3D11_BLEND_DESC defaultDesc;
	unordered_map<string, Data> blendStates;
};

/////////////////////////////////////////////////////////////// �������� �ʴ� �ڿ���
//��� �� �Ҹ��� ���� ������ Release(�ڿ�) �ؼ� ����� ���� ����

class Texture2DResources
{
public:
	 void Create(Resource<ID3D11Texture2D>& dest, D3D11_TEXTURE2D_DESC desc);

private:
	friend class Resources;
	void Release();

	struct Data
	{
		Data() : data(nullptr), refCount(0) {}
		ID3D11Texture2D* data;
		UINT refCount;
	};
	unordered_map<ID3D11Texture2D*, Data> texture2Ds;// leak �ȳ��� �����ϴ� �뵵, ��ȿ���������� �ٸ� ����� ������ �ȳ�
};

class RenderTargetViewResources
{
public:
	RenderTargetViewResources()
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		SetDefault(desc);
	}
	void Create(Resource<ID3D11RenderTargetView>& dest,D3D11_RENDER_TARGET_VIEW_DESC desc, ID3D11Resource* buffer);

	void CreateDefault(Resource<ID3D11RenderTargetView>& dest, ID3D11Resource* buffer) { Create(dest, defaultDesc, buffer); };
	void SetDefault(D3D11_RENDER_TARGET_VIEW_DESC desc) {	defaultDesc = desc;};
private:
	friend class Resources;
	void Release();

	struct Data
	{
		Data() : data(nullptr), refCount(0) {}
		ID3D11RenderTargetView* data;
		UINT refCount;
	};
	D3D11_RENDER_TARGET_VIEW_DESC defaultDesc;
	unordered_map<ID3D11RenderTargetView*, Data> rtvs;// leak �ȳ��� �����ϴ� �뵵
};


class DepthStencilViewResources
{
public:
	DepthStencilViewResources()
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc = {};

		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		SetDefault(desc);
	}
	void Create(Resource<ID3D11DepthStencilView>& dest,D3D11_DEPTH_STENCIL_VIEW_DESC desc, ID3D11Resource* buffer);

	void CreateDefault(Resource<ID3D11DepthStencilView>& dest, ID3D11Resource* buffer) { Create(dest, defaultDesc, buffer); };
	void SetDefault(D3D11_DEPTH_STENCIL_VIEW_DESC desc) {	defaultDesc = desc;};
private:
	friend class Resources;
	void Release();

	struct Data
	{
		Data() : data(nullptr), refCount(0) {}
		ID3D11DepthStencilView* data;
		UINT refCount;
	};
	D3D11_DEPTH_STENCIL_VIEW_DESC defaultDesc;
	unordered_map<ID3D11DepthStencilView*, Data> depthStencilViews;// leak �ȳ��� �����ϴ� �뵵
};



