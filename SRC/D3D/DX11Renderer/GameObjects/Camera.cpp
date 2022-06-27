#include "pch_dx_11.h"
#include "Camera.h"

constexpr float pi  = 3.141592f;

Camera::Camera()
	:viewproj{}, view{}, proj{}, fov(pi / 2.0f), bMovable(true), camMovSpeed(0.1015f), camRotSpeed(0.01f)
{
	view = XMMatrixInverse(nullptr, *(transform.World()));
	proj = XMMatrixPerspectiveFovLH(fov, ((float)DX->width / (float)DX->height),1.0f, 1000.0f);

	viewproj.view = view;
	viewproj.proj = proj;

	transform.pos = { 0,100, -50 };
	viewproj.camPos = transform.pos;

	D3D11_BUFFER_DESC cbd = {};
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(Data);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = 0;
	cbd.MiscFlags = 0;
	cbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
	initData.pSysMem = &viewproj;
	HRESULT hr = DEVICE->CreateBuffer(&cbd, &initData, &viewprojBuffer);

	DC->VSSetConstantBuffers(1, 1, &viewprojBuffer);
}

Camera::~Camera()
{
}

void Camera::Update()
{
	if (bMovable)
		Move();

	transform.UpdateWorld();
	viewproj.camPos = transform.pos;
	view = XMMatrixInverse(nullptr, *(transform.World()));
	viewproj.view = view;
}

void Camera::Render()
{
	DC->UpdateSubresource(viewprojBuffer, 0, 0, &viewproj, 0, 0);
	DC->VSSetConstantBuffers(1, 1, &viewprojBuffer);
}

void Camera::OnResize(UINT height, UINT width)
{
	proj = XMMatrixPerspectiveFovLH(fov, width / (float)height, 1.0f, 1000.0f);

	viewproj.proj = proj;
}

void Camera::Move()
{
	if (KeyBoard::Get()->Press(VK_LBUTTON))
		transform.rot.y += KeyBoard::Get()->mouseDx * camRotSpeed;
	if (KeyBoard::Get()->Press(VK_LBUTTON))
		transform.rot.x += KeyBoard::Get()->mouseDy * camRotSpeed;

	Vector3 way;
	way.w = 0.0f;
	
	if (KeyBoard::Get()->Press('D'))
		way.x += camMovSpeed;
	if (KeyBoard::Get()->Press('A'))
		way.x -= camMovSpeed;
	if (KeyBoard::Get()->Press('W'))
		way.z += camMovSpeed;
	if (KeyBoard::Get()->Press('S'))
		way.z -= camMovSpeed;
	if (KeyBoard::Get()->Press('E'))
		way.y += camMovSpeed;
	if (KeyBoard::Get()->Press('Q'))
		way.y -= camMovSpeed;

	transform.pos += way * *(transform.World());
}
