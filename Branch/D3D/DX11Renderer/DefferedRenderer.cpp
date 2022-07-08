#include "pch_dx_11.h"
#include "DefferedRenderer.h"

DefferedRenderer::DefferedRenderer()
{
	transform.pos.x = 0.0f;
	transform.pos.y = 0.0f;

	transform.scale.x = 1.0f;
	transform.scale.y = 1.0f;
	CreateCanvas();
}

DefferedRenderer::~DefferedRenderer()
{
}

void DefferedRenderer::Update()
{
	__super::Update();

	srvs.resize(rtts.size());
	renderTargets.resize(rtts.size());
	for (UINT i = 0; i < rtts.size(); i++)
	{
		srvs[i] = rtts[i]->srv;
		renderTargets[i] = rtts[i]->rtv;
	}
}

void DefferedRenderer::Render()
{
	if (mesh == nullptr) return;

	mesh->Set();//setVertexBuffer; indexBuffer;

	//worldBuffer
	DC->UpdateSubresource(transform.WorldBuffer(), 0, 0, transform.World(), 0, 0);
	DC->VSSetConstantBuffers(0, 1, &(transform.WorldBuffer()));

	//PS
	material->Set();

	DC->PSSetShaderResources(0, srvs.size(), srvs.data());

	DC->DrawIndexed(mesh->GetIndexCount(), 0, 0);

}

void DefferedRenderer::CreateCanvas()
{
	name = "DefferedRenderTexture";

	VertexSet vertexSet;

	vertexSet.AddElementToDesc(sizeof(float) * 3, DataType::FLOAT, "POSITION");
	vertexSet.AddElementToDesc(sizeof(float) * 2, DataType::FLOAT, "UV");

	struct temp
	{
		float3 pos;
		float2 uv;
	};

	temp v1[4];

	v1[0].pos = float3(-1, +1, 0.0f);
	v1[0].uv = float2(0, 0);

	v1[1].pos = float3(+1, +1, 0.0f);
	v1[1].uv = float2(+1, 0);

	v1[2].pos = float3(-1, -1, 0.0f);
	v1[2].uv = float2(0, +1);

	v1[3].pos = float3(+1, -1, 0.0f);
	v1[3].uv = float2(+1, +1);

	vertexSet.AddData(v1, sizeof(v1));

	vector<UINT> indices =
	{
		0,1,2,
		2,1,3
	};

	mesh = new Mesh(vertexSet, indices.data(), indices.size(), L"CanvasVS.hlsl");


	MaterialDesc matDesc;
	matDesc.pixelShaderName = L"DefferedRenderPS.hlsl";
	matDesc.diffuseFileName = L"WoodCrate01.dds";
	material = new Material(matDesc);
}
