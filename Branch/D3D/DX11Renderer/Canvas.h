#pragma once

/// <summary>
/// ����Ʈ�� ���� ����, ����, 0.5 , 0.0 , 0.5, 0.5 �ϸ� ȭ�� ������ ��� 4���� 1 �����ϰ�,
///  uv ��ǥ�� ����
/// 
/// written by Ssreppa 22 06 18
/// </summary>
class Canvas : public Object//todo : ���� layer ���� �� �ְ� �߰� �� ��
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