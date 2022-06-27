#include "pch_dx_11.h"
#include "Transform.h"

Vector3 Transform::WorldUp = { 0,1,0 };
Vector3 Transform::WorldRight = { 1,0,0 };
Vector3 Transform::WorldForward = { 0,0,1 };

Transform::Transform()
	:pos({ 0,0,0 }), rot({ 0,0,0 }), scale({ 1,1,1 }), S(XMMatrixIdentity()), R(XMMatrixIdentity()), T(XMMatrixIdentity()),
	parent(nullptr), postS(XMMatrixIdentity()), postR(XMMatrixIdentity()), postT(XMMatrixIdentity()), local(XMMatrixIdentity())
{
	D3D11_BUFFER_DESC cbd;
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(Data);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = 0;
	cbd.MiscFlags = 0;
	cbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &world;
	HRESULT hr = DEVICE->CreateBuffer(&cbd, &initData, &worldBuffer);//vertices �迭 ������� �ǳ�..? ���� �������� ����Ǽ� create�ϰ� ���� ������� �ǳ�....?
}

Transform::~Transform()
{
}

void Transform::UpdateWorld()
{
	S = XMMatrixScalingFromVector(XMLoadFloat4(scale));
	R = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat4(rot));
	T = XMMatrixTranslationFromVector(XMLoadFloat4(pos));

	local = S * R * T;

	world.data = local ;

	////todo: �ӽ÷� ���� ��
	Transform* temp = parent;
	while (temp != nullptr)
	{
		temp->UpdateWorld();
		world.data *= temp->local;
		temp = temp->parent;
	}
}
