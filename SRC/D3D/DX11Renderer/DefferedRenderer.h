#pragma once

class DefferedRenderer : public Object
{
public:
	DefferedRenderer();
	~DefferedRenderer();

	virtual void Update() override;
	virtual void Render() override;

	void AddRTT(RenderTargetTexutre* rtt) { rtts.push_back(rtt); }
	void SetRenderTargets() {	DC->OMSetRenderTargets(3, renderTargets.data(), DX->depthStencilView);	}

private:
	vector< RenderTargetTexutre*> rtts;
	vector< ID3D11ShaderResourceView*> srvs;
	vector< ID3D11RenderTargetView*> renderTargets;

	//Resource<ID3D11DepthStencilView> depthStencilView;

	void CreateCanvas();
};