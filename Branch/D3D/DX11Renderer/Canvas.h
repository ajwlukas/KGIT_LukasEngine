#pragma once

/// <summary>
/// 뷰포트에 대한 비율, 예시, 0.5 , 0.0 , 0.5, 0.5 하면 화면 오른쪽 상단 4분의 1 차지하게,
///  uv 좌표와 동일
/// 
/// written by Ssreppa 22 06 18
/// </summary>
class Canvas : public Object//todo : 추후 layer 나눌 수 있게 추가 할 것
{
public:
	Canvas(float posLeftRatio, float posTopRatio, float widthRatio, float heightRatio);
	~Canvas();

	virtual void Update() override;
	virtual void Render() override;

	void SetSRV(wstring fileName) { material->SetDiffuseMap(fileName); }
	void SetSRV(ID3D11ShaderResourceView** srv) { material->SetDiffuseMap(srv); }

private:
	float posLeftRatio, posTopRatio, widthRatio, heightRatio;
	void CreateCanvas();


};