#include "pch_dx_11.h"
#include "SkinningAnimation.h"

SkinningAnimator::SkinningAnimator(Object* object)
	:bones{}, Component(object)
{
	//애니메이션용 본 버퍼
	D3D11_BUFFER_DESC cbd;
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(Data);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = 0;
	cbd.MiscFlags = 0;
	cbd.StructureByteStride = 0;// sizeof(XMFLOAT4X4); todo : 여기??
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &bones;
	HRESULT hr = DEVICE->CreateBuffer(&cbd, &initData, &animBuffer);
}

SkinningAnimator::~SkinningAnimator()
{
}

void SkinningAnimator::Update()
{
	for (UINT i = 0; i < refTransforms.size(); i++)
	{
		Matrix finalBoneTM;
		 finalBoneTM = skinWorldTMInv[i] * *(refTransforms[i]->World());

		XMStoreFloat4x4(&(bones.data[i]), finalBoneTM);
	}
}
