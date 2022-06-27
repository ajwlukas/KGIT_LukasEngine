#include "WVP.hlsli"

struct VertexInput
{
    float3 position : POSITION;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float3 uvw : UVW;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    
	////////////////////////////////////
	
    output.position = LocalToNDC(input.position);
    
	////////////////////////////////////
    
    output.uvw = input.position;
    
	////////////////////////////////////
	
    return output;
}