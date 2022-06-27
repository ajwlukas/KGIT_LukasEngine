#pragma once

#include "pch_dx_11.h"

class ShaderNameFinder
{
public:
	ShaderNameFinder()
	{
		shaderNames["_POSITION_NORMAL"] = L"LightVertex.hlsl";
		shaderNames["_POSITION_UV_NORMAL_TANGENT"] = L"UVLightVertex.hlsl";
		shaderNames["_POSITION_UV_NORMAL_REFINDEX_WEIGHT"] = L"SkinningAnimationVertex.hlsl";
	}

	wstring Find(string semanticString) { return shaderNames.at(semanticString); }

private:
	unordered_map<string, wstring> shaderNames;
};

enum class DataType
{
	FLOAT,
	UINT
};

class VertexSet//������ ����� ���ؽ� ����� �׽�Ʈ ����
{
public:
	VertexSet()
		:memIndex(0), stride(0), semanticString("")
	{

	}
	void AddElementToDesc(UINT dataSize, DataType type,
		LPCSTR  SemanticName, int SemanticIndex = 0,
		int inputSlot = 0, int InstanceDataStepRate = 0);
	//���� : Element ���� ������ �����͸� �־��־����

	void AddData(void* data, UINT dataSize);
	//���� : 4����Ʈ¥�� ������ �¸� �־�� ��!! ex)float, int, float3, float4 ���, bool, long, short ����x
	// ������ �ð�ȭ ���� : vertex0.pos.x | vertex0.pos.y | vertex0.pos.z | vertex0.uv.u | vertex0.uv.v | vertex1.pos.x ...

	void* GetVerticesData() { return verticesData.data(); }
	UINT GetVerticesSize() { return verticesData.size() * sizeof(DWORD); }
	UINT GetVertexSize() { return stride; }

	D3D11_INPUT_ELEMENT_DESC* GetDescsData() { return descs.data(); }
	UINT GetDescsSize() { return descs.size(); }

	wstring GetCorrespondingShaderFileName() { return shaderFinder.Find(semanticString); }
private:
	vector<DWORD> verticesData;//���� ���ؽ����� ������
	UINT memIndex;

	vector<D3D11_INPUT_ELEMENT_DESC> descs;
	UINT stride;//���ؽ� �ϳ��� ũ��

	string semanticString;
	static ShaderNameFinder shaderFinder;//��ǲ ���п� ���� �˸��� �⺻ ���̴��� �̸��� �ְ�
};