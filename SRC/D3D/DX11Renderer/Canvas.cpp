#include "pch_dx_11.h"
#include "Canvas.h"

Canvas::Canvas(float posLeftRatio, float posTopRatio, float widthRatio, float heightRatio)
	:posLeftRatio(posLeftRatio), posTopRatio(posTopRatio), widthRatio(widthRatio), heightRatio(heightRatio)
{
	transform.pos.x = -(1.0f - widthRatio) + posLeftRatio * 2.0f;
	transform.pos.y = (1.0f - heightRatio) - posTopRatio * 2.0f;

	transform.scale.x = widthRatio;
	transform.scale.y = heightRatio;
	CreateCanvas();
}

Canvas::~Canvas()
{
}

void Canvas::Update()
{
	__super::Update();
}

void Canvas::Render()
{
	if (mesh == nullptr) return;

	mesh->Set();//setVertexBuffer; indexBuffer;

	//worldBuffer
	DC->UpdateSubresource(transform.WorldBuffer(), 0, 0, transform.World(), 0, 0);
	DC->VSSetConstantBuffers(0, 1, &(transform.WorldBuffer()));

	//PS
	material->Set();

	DC->PSSetShaderResources(0, 1, *srv);

	DC->DrawIndexed(mesh->GetIndexCount(), 0, 0);

}

void Canvas::CreateCanvas()
{
	name = "Canvas";

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
	matDesc.pixelShaderName = L"CanvasPS.hlsl";
	matDesc.diffuseFileName = L"WoodCrate01.dds";
	material = new Material(matDesc);
}
