#include "WVP.hlsli"

struct VertexInput
{
    float3 position : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float4 posW : POS_WORLD;
    float3 campos : CAM_POSITION;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    
	////////////////////////////////////
	
    output.position = LocalToNDC(input.position);
    
	////////////////////////////////////
    
    output.uv = input.uv;
    
	////////////////////////////////////
    
    output.normal = normalize(LocalToWorldDirOnly(input.normal));
    
	////////////////////////////////////
	
    return output;
}