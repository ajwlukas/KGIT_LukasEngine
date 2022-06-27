#pragma once
/// <summary>
/// Material �����ϴ� 2���� ���,
/// 1. MaterialDesc�� �����͸� �ۼ��ؼ� Material �����ڿ� �־��༭ ����
/// 2. Material�� ������ Set�Լ���� ������ �Է�
/// 
/// written by Ssreppa 22.06.22
/// </summary>
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
	void SetNormalMap(wstring fileName);
	void SetSpecularMap(wstring fileName);

	void SetDiffuseMap(ID3D11ShaderResourceView** srv);
	void SetNormalMap(ID3D11ShaderResourceView** srv);
	void SetSpecularMap(ID3D11ShaderResourceView** srv);

	void SetSamplerState(D3D11_SAMPLER_DESC samplerDesc);


	wstring pixelShaderName;
private:
	ID3D11PixelShader* pixelShader;
	ID3D11ShaderResourceView** diffuse;
	ID3D11ShaderResourceView** normal;
	ID3D11ShaderResourceView** specular;
	ID3D11SamplerState** samplerState;
};
