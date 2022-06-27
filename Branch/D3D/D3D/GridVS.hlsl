#include "WVP.hlsli"

struct VertexInput
{
    float3 position : POSITION;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float4 posW : POS_WORLD;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    
	////////////////////////////////////
	
    output.posW = LocalToWorld(input.position);
    
	////////////////////////////////////
    
    output.position = WorldToNDC(output.posW);
    
	////////////////////////////////////
	
    return output;
}