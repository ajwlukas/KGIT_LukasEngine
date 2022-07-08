#include "pch_dx_11.h"
#include "Material.h"

Material::Material(const MaterialDesc& desc)
	:normal{}, diffuse{}, specular{},
	samplerState{}, pixelShader(nullptr), pixelShaderName(desc.pixelShaderName)
{
	pixelShader = RESOURCES->pixelShaders->Get(desc.pixelShaderName);

	if (desc.diffuseFileName.length() > 0)
	{ 
		RESOURCES->srvs->GetFromFile(diffuse, desc.diffuseFileName);//todo : 비어있을 때 에러가 안남
	}

		RESOURCES->samplerStates->Get(samplerState, desc.samplerDesc);
}

Material::~Material()
{
}

void Material::Set()
{//todo
	if (diffuse.resource != nullptr)
		DC->PSSetShaderResources(0, 1, diffuse);
	if (normal.resource != nullptr)
		DC->PSSetShaderResources(1, 1, normal);
	if (specular.resource != nullptr)
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
	RESOURCES->srvs->GetFromFile(diffuse, fileName);
}

void Material::SetNormalMap(wstring fileName)
{
	RESOURCES->srvs->GetFromFile(normal, fileName);
}

void Material::SetSpecularMap(wstring fileName)
{
	RESOURCES->srvs->GetFromFile(specular, fileName);
}

void Material::SetSamplerState(D3D11_SAMPLER_DESC samplerDesc)
{
	RESOURCES->samplerStates->Get(samplerState, samplerDesc);
}
