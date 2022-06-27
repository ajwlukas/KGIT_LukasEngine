#pragma once

class Camera : public Object
{
public:
	Camera();
	~Camera();

	// GameObject을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;

	void OnResize(UINT height, UINT width);

	void Move();
	bool bMovable;
private:
	float camMovSpeed, camRotSpeed;

private:
	struct Data
	{
		Matrix view;
		Matrix proj;
		float3 camPos;
	}viewproj;

	ID3D11Buffer* viewprojBuffer;

	Matrix view, proj;

	float fov;//시야각
};