#pragma once

#include <d3d11.h>
#include <vector>
#include "Resource.h"

class Camera;
class Object;
class Skybox;
class Canvas;
class Grid;
class DefferedRenderer;
class RenderTargetTexutre;

class DX11Renderer
{
private:
	DX11Renderer();
	~DX11Renderer();
	static DX11Renderer* instance;
public:
	static void Create() { instance = nullptr ? instance : new DX11Renderer(); }
	static void Delete() { delete instance; }
	static DX11Renderer* Get() { return instance; }

	HRESULT Init();

	void BeginRender();

	void Render();

	void PreRender();
	void PostRender();

	//////////////////////
	Camera* cam;
	Skybox* skyBox;
	DefferedRenderer* defferedRenderer;
	RenderTargetTexutre* rtts[3];
	Canvas* canvas;
	Canvas* normalCanvas;
	Canvas* albedoCanvas;
	Grid* grid;

	std::vector<Object*> testObjects;
	std::vector<Object*> pig;
	std::vector<Object*> joe;

	void TestInit();
	void TestDestructor();
	void TestUpdate();

	/////////////////////////
	void EndRender();
private:
	HRESULT CreateDeviceAndSwapChain();
	HRESULT CreateRtv();
	HRESULT CreateAndSetDepthStencilView();
	HRESULT CreateAndSetRasterizerState();
	HRESULT CreateBlendState();
	void SetViewPort();

#pragma region
private:
	HWND hWnd;
	WINDOWINFO windowInfo;
public:
	void OnResize();
	UINT width, height;
#pragma endregion

#pragma region
private:
	ID3D11Device* device;
	ID3D11DeviceContext* dc;
	IDXGISwapChain* swapChain;
public:
	ID3D11Device* GetDevice() { return device; }
	ID3D11DeviceContext* GetDC() { return dc; }
#pragma endregion

#pragma region
public:
	Resource<ID3D11DepthStencilView> depthStencilView;
private:
	class Resources* resources;

	Resource<ID3D11RenderTargetView> rtv;
	Resource<ID3D11Texture2D> depthStencilBuffer;

	Resource<ID3D11DepthStencilState> depthStencilState;
	Resource<ID3D11RasterizerState> rasterState;
	Resource<ID3D11BlendState> blendState;
public:
	Resources* GetResources() {return resources;}
#pragma endregion
};