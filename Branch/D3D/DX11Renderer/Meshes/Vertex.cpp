#include "pch_dx_11.h"

ShaderNameFinder VertexSet::shaderFinder;

DXGI_FORMAT GetCorrectFormat(UINT dataCount, DataType type)
{
	switch (type)
	{
		case DataType::FLOAT:
		{
			switch (dataCount)
			{
			case 1:
				return DXGI_FORMAT_R32_FLOAT;
			case 2:
				return DXGI_FORMAT_R32G32_FLOAT;
			case 3:
				return DXGI_FORMAT_R32G32B32_FLOAT;
			case 4:
				return DXGI_FORMAT_R32G32B32A32_FLOAT;
			default:
				assert(false);
			}
		}
		case DataType::UINT:
		{
			switch (dataCount)
			{
			case 1:
				return DXGI_FORMAT_R32_UINT;
			case 2:
				return DXGI_FORMAT_R32G32_UINT;
			case 3:
				return DXGI_FORMAT_R32G32B32_UINT;
			case 4:
				return DXGI_FORMAT_R32G32B32A32_UINT;
			default:
				assert(false);
			}
		}

	default:
		assert(false);
	}
}

void VertexSet::AddElementToDesc(UINT dataSize, DataType type, LPCSTR SemanticName, int SemanticIndex,
	int inputSlot, int InstanceDataStepRate)
{
	UINT dataCount = dataSize / sizeof(DWORD);

	//FillDesc
	D3D11_INPUT_ELEMENT_DESC desc;
	desc.SemanticName = SemanticName;
	desc.SemanticIndex = SemanticIndex;
	desc.Format = GetCorrectFormat(dataCount, type);
	desc.InputSlot = inputSlot;
	desc.AlignedByteOffset = stride;
	desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;//todo : ���߿� ���� ����
	desc.InstanceDataStepRate = InstanceDataStepRate;

	descs.emplace_back(desc);

	stride += 4 * dataCount;//4����Ʈ ���� �����͸� ���� ����

	//���̴� ��ġ�� Semantic ���� ����
	semanticString += "_";
	semanticString += SemanticName;
}

void VertexSet::AddData(void* data, UINT dataSize)
{

	UINT dataCount = dataSize / sizeof(DWORD);
	//FillData
	verticesData.resize(verticesData.size() + dataCount);
	memcpy(verticesData.data() + memIndex, data, sizeof(DWORD) * dataCount);

	memIndex += dataCount;
}
