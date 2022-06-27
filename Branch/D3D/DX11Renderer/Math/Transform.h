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

	//Matrix initial;//잘못된 데이터 고치는 용, 뒤를 보고 있다던지, 단위 행렬로 놓고 안쓰는게 좋음
	Matrix postS, postR, postT;//내가 생성해주는 트랜스폼 이외에 다른 트랜스폼의 영향을 받고 있을 때
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