#include "WVP.hlsli"

struct VertexInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    
	////////////////////////////////////
	
    output.position = LocalToNDC(input.position);
    
	////////////////////////////////////
    
    output.normal = normalize(LocalToWorldDirOnly(input.normal));
    
	////////////////////////////////////
	
    return output;
}