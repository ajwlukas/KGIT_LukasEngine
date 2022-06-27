#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dcommon.h>
#include <DirectXMath.h>
using namespace DirectX;

struct MaterialDesc
{
	MaterialDesc()
		:pixelShaderName(L"LightPixel.hlsl"), diffuseFileName(L""),samplerDesc{}
	{
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	}
	wstring pixelShaderName;
	wstring diffuseFileName;
	D3D11_SAMPLER_DESC samplerDesc;
};

class Material
{
public:
	Material(const MaterialDesc& desc = MaterialDesc());
	~Material();

	void Set();

	void SetShader(wstring fileName);
	void SetDiffuseMap(wstring fileName);
	void SetDiffuseMap(ID3D11ShaderResourceView** srv);
	void SetNormalMap(wstring fileName);
	void SetSamplerState(D3D11_SAMPLER_DESC samplerDesc);


	wstring pixelShaderName;
private:
	ID3D11PixelShader* pixelShader;
	ID3D11ShaderResourceView** diffuse;
	ID3D11ShaderResourceView** normal;
	ID3D11SamplerState** samplerState;
};
