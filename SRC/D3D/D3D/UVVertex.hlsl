#include "WVP.hlsli"

struct VertexInput
{
    float3 position : POSITION;
    float2 uv : UV;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    
	////////////////////////////////////
	
    output.position = LocalToNDC(input.position);
    
	////////////////////////////////////
    
    output.uv = input.uv;
    
	////////////////////////////////////
	
    return output;
}