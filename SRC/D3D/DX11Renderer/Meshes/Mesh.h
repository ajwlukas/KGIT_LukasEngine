#pragma once

#include "pch_dx_11.h"


struct Mesh
{
public:
	Mesh(VertexSet& vertexSet, UINT indexData[], UINT indexCount,
		wstring vsFileName = L"UVLightVertex.hlsl", D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	~Mesh();
	void Set();

	const UINT& GetIndexCount() const { return indexCount; }

	void SetShader(wstring vsFileName) {
		this->vsFileName = vsFileName;
		inputLayout = RESOURCES->inputLayouts->Get(desc, descSize, vsFileName);
		shader = RESOURCES->vertexShaders->Get(vsFileName);
	}
	//todo : inputLayoutµµ ¹Ù²ãÁà¾ßÇßÀ½...
	/*void SetShader(wstring vsFileName) { 
		this->vsFileName = vsFileName;
		inputLayout = RESOURCES->inputLayouts->Get(&(vertexData[0]), vsFileName);
	shader = RESOURCES->vertexShaders->Get(vsFileName); }*/

private:
	wstring vsFileName;
	ID3D11InputLayout* inputLayout;
	D3D_PRIMITIVE_TOPOLOGY topology;

	UINT vertexDataSize;
	UINT indexCount;
	UINT offset = 0;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11VertexShader* shader;

	D3D11_INPUT_ELEMENT_DESC* desc;
	UINT descSize;
};