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
	ID3D11RasterizerState* defaultRasterizerState = nullptr;//다시 돌려줄 용도
	ID3D11DepthStencilState* depthStencilState = nullptr;
	ID3D11DepthStencilState* defaultDepthStencilState = nullptr;//다시 돌려줄 용도


	ID3D11ShaderResourceView** srvSkyBox;
};