﻿#pragma once

#define SAFE_DELETE(x)			  \
{													  \
  if(x != nullptr)								  \
  {													  \
	delete x;									  \
	x = nullptr;								  \
	}												  \
}

#define SAFE_RELEASE(x)		  \
{													  \
  if((x) != nullptr)								  \
  {													  \
	(x)->Release();								  \
	(x) = nullptr;								  \
  }													  \
}

#define DX DX11Renderer::Get()
#define DC DX11Renderer::Get()->GetDC()
#define DEVICE DX11Renderer::Get()->GetDevice()
#define RESOURCES DX11Renderer::Get()->GetResources()

//commons
#include <unordered_map>
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <corecrt_wstring.h>
#include <assert.h>
#include <memory>
#include <functional>

using namespace std;

#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dcommon.h>
#include <DirectXMath.h>
using namespace DirectX;

#include "DXTK\d3dx11effect.h"
//#include "DXTK\WICTextureLoader\LoaderHelpers.h"
#include "DXTK\WICTextureLoader\WICTextureLoader.h"
#include "DXTK\DDSTextureLoader.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "Effects11d.lib")
#endif // _DEBUG

#ifdef NDEBUG
#pragma comment(lib, "DirectXTKr.lib")
#pragma comment(lib, "Effects11.lib")//디버그 나눌것
#endif // NDEBUG



#pragma comment(lib, "libfbxsdk.lib")

typedef XMFLOAT2 float2;
typedef XMFLOAT3 float3;
typedef XMFLOAT4 float4;
typedef XMMATRIX Matrix;

#include "pch_common.h"

#include "DX11Renderer.h"
#include "Resources/Resources.h"


#include "Math/Vector3.h"
#include "Math/Transform.h"
#include "Meshes/Vertex.h"
#include "Meshes/Mesh.h"
#include "Material.h"

#include "Component.h"
#include "MeshAnimation.h"
#include "SkinningAnimation.h"

#include "GameObjects\Object.h"

#include "GameObjects\Camera.h"
#include "RenderTargetTexture.h"
#include "DefferedRenderer.h"
#include "SkyBox.h"
#include "Canvas.h"
#include "Grid.h"