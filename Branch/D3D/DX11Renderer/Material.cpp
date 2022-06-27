#include "pch_dx_11.h"
#include "Material.h"

Material::Material(const MaterialDesc& desc)
	:normal(nullptr),diffuse(nullptr), specular(nullptr),
	samplerState(nullptr), pixelShader(nullptr), pixelShaderName(desc.pixelShaderName)
{
	pixelShader = RESOURCES->pixelShaders->Get(desc.pixelShaderName);

	if (desc.diffuseFileName.length() > 0)
	{ 
		diffuse = RESOURCES->srvs->Get(desc.diffuseFileName);//todo : 비어있을 때 에러가 안남
	}

		samplerState = RESOURCES->samplerStates->Get(desc.samplerDesc);
}

Material::~Material()
{
}

void Material::Set()
{//todo
	if (diffuse != nullptr)
		DC->PSSetShaderResources(0, 1, diffuse);
	if (normal != nullptr)
		DC->PSSetShaderResources(1, 1, normal);
	if (specular != nullptr)
		DC->PSSetShaderResources(2, 1, specular);

		DC->PSSetSamplers(0, 1, samplerState);
		DC->PSSetShader(pixelShader,0,0);
}

void Material::SetShader(wstring fileName)
{
	pixelShaderName = fileName;
	pixelShader = RESOURCES->pixelShaders->Get(fileName);
}

void Material::SetDiffuseMap(wstring fileName)
{
	diffuse = RESOURCES->srvs->Get(fileName);
}

void Material::SetDiffuseMap(ID3D11ShaderResourceView** srv)
{
	diffuse = srv;
}

void Material::SetNormalMap(wstring fileName)
{
	normal = RESOURCES->srvs->Get(fileName);
}

void Material::SetSpecularMap(wstring fileName)
{
	specular = RESOURCES->srvs->Get(fileName);
}

void Material::SetNormalMap(ID3D11ShaderResourceView** srv)
{
	normal = srv;
}

void Material::SetSpecularMap(ID3D11ShaderResourceView** srv)
{
	specular = srv;
}

void Material::SetSamplerState(D3D11_SAMPLER_DESC samplerDesc)
{
	samplerState = RESOURCES->samplerStates->Get(samplerDesc);
}
