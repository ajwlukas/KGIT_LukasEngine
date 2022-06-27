#pragma once

class Object;
class Camera;

class Skybox : public Object
{
public:
	Skybox(Camera* camera);

	virtual void Update() override;
	virtual void Render() override;

private:
	void CreateSkyBox();
	void CreateRasterState();
	void CreateDepthStencilState();

	Camera* cam;

	ID3D11RasterizerState* rasterizerState = nullptr;
	ID3D11RasterizerState* defaultRasterizerState = nullptr;//�ٽ� ������ �뵵
	ID3D11DepthStencilState* depthStencilState = nullptr;
	ID3D11DepthStencilState* defaultDepthStencilState = nullptr;//�ٽ� ������ �뵵


	ID3D11ShaderResourceView** srvSkyBox;
};