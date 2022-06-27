#pragma once

class SkinningAnimator : public Component
{
private:
	struct Data
	{
		XMFLOAT4X4 data[100];
	}bones;
	ID3D11Buffer* animBuffer;
public:
	XMFLOAT4X4* Matrixes() { return bones.data; }
	ID3D11Buffer*& AnimBuffer() { return animBuffer; }

public:
	SkinningAnimator(Object* object);
	~SkinningAnimator();

	void Update();

	vector<Transform*> refTransforms;
	vector<Matrix> skinWorldTMInv;
};