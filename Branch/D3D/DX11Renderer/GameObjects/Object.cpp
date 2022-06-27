#include "pch_dx_11.h"

Object::Object()
	:animator(nullptr), mesh(nullptr), material(nullptr), skinner(nullptr)
{
}

Object::~Object()
{
	if (mesh != nullptr)
		delete mesh;
	if (material != nullptr)
		delete material;
	if (animator != nullptr)
		delete animator;
	if (skinner != nullptr)
		delete skinner;
}

void Object::Update()
{
	if (animator != nullptr)
		animator->Update();
	if (skinner != nullptr)
		skinner->Update();
	transform.UpdateWorld();
}

void Object::Render()
{
	if (mesh == nullptr) return;


	if (skinner != nullptr)
	{
		DC->UpdateSubresource(skinner->AnimBuffer(), 0, 0, skinner->Matrixes(), 0, 0);
		DC->VSSetConstantBuffers(2, 1, &(skinner->AnimBuffer()));
	}

	mesh->Set();//setVertexBuffer; indexBuffer;

	//skinningBuffer

	//worldBuffer
	DC->UpdateSubresource(transform.WorldBuffer(), 0, 0, transform.World(), 0, 0);
	DC->VSSetConstantBuffers(0, 1, &(transform.WorldBuffer()));
	
	//PS
	material->Set();

	DC->DrawIndexed(mesh->GetIndexCount(), 0, 0);
}
