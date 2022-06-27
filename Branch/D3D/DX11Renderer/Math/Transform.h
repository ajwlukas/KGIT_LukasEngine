#pragma once

class Vector3;

class Transform
{
public:
	Transform();
	~Transform();

	Matrix* World() { return &world.data; }
	ID3D11Buffer*& WorldBuffer() {return worldBuffer;	}

	static Vector3 WorldUp, WorldRight, WorldForward;

	Vector3& Up() {
		XMStoreFloat4(&up.data, XMVector4Transform(WorldUp, world.data));
		return  up;
	}
	Vector3& Right() {
		XMStoreFloat4(&right.data, XMVector4Transform(WorldRight, world.data));
		return  right;
	}
	Vector3& Forward() {
		XMStoreFloat4(&forward.data, XMVector4Transform(WorldForward, world.data));
		return  forward;
	}

	void UpdateWorld();

public:

	Vector3 pos;
	Vector3 rot;
	Vector3 scale;

	Vector3 up, right, forward;

	//Matrix initial;//�߸��� ������ ��ġ�� ��, �ڸ� ���� �ִٴ���, ���� ��ķ� ���� �Ⱦ��°� ����
	Matrix postS, postR, postT;//���� �������ִ� Ʈ������ �̿ܿ� �ٸ� Ʈ�������� ������ �ް� ���� ��
	Matrix local;//S * R * T
private:
	Matrix S, R, T;
	struct Data
	{
		Data()
		{
			data = XMMatrixIdentity();
		}
		Matrix data;
	}world;
public:
	Transform* parent;
private:
	ID3D11Buffer* worldBuffer;
};