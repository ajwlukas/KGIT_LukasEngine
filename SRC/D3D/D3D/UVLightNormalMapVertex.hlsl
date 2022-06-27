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
    
    output.posW = LocalToWorld(input.position);
    output.position = WorldToNDC(output.posW);
    
    ////////////////////////////////////
    
    output.uv = input.uv;

    ////////////////////////////////////
    
    output.normal = input.normal;
    output.tangent = normalize(input.tangent - (dot(output.normal, input.tangent) * output.normal));
    output.binormal = cross(output.tangent, output.normal);
    
    output.normal = normalize(LocalToWorldDirOnly(output.normal));
    output.binormal = normalize(LocalToWorldDirOnly(output.binormal));
    output.tangent = normalize(LocalToWorldDirOnly(output.tangent));

    ////////////////////////////////////
    
    output.campos = camPos;
    
	////////////////////////////////////
    
    return output;
}