#pragma once


/// <summary>
/// 싱글톤 객체, 메인 루프 이전 Create() && Init(), 이후 Delete, Get을 이용해 객체를 받는다.
/// 
/// written by Ssreppa 22.03.11
/// </summary>
#include <d3d11.h>
#include <vector>


class Object;
class Skybox;
class Canvas;
class Grid;
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



	//////////////////////
	Skybox* skyBox;
	Canvas* main;
	Canvas* canvas;
	Canvas* normalCanvas;
	Canvas* albedoCanvas;
	Grid* grid;

	std::vector<Object*> testObjects;
	std::vector<Object*> pig;
	std::vector<Object*> joe;

	std::vector<int> a;

	void TestInit();
	void TestDestructor();
	void TestUpdate();
	void TestRender();
	void PostRender();

	void TestCreateRTT();
	/////////////////////////


	void EndRender();
private:
	HRESULT CreateDeviceAndSwapChain();
	HRESULT CreateRtv();
	HRESULT CreateAndSetDepthStencilView();
	HRESULT CreateAndSetRasterizerState();
	HRESULT CreateBlendState();
	void SetViewPort();

public:
	void OnResize();
	UINT width, height;

private:
	HWND hWnd;
	WINDOWINFO windowInfo;
	ID3D11Device* device;
	ID3D11DeviceContext* dc;
 
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* rtv;

	//postProcssing test Field
	ID3D11Texture2D* testTexture[3];
	ID3D11ShaderResourceView* testSRV[3];
	ID3D11RenderTargetView* testRTV[3];


	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView* depthStencilView;

	ID3D11RasterizerState* rasterState;

	ID3D11BlendState* blendState;
	
	class Resources* resources;

public:
	ID3D11Device* GetDevice() { return device; }
	ID3D11DeviceContext* GetDC() { return dc; }

	Resources* GetResources() {return resources;}

	class Camera* cam;
};