#include "WVP.hlsli"

#define MAX_SKINNING_MATRIX 100

cbuffer Skinner : register(b2)
{
    float4x4 skin[MAX_SKINNING_MATRIX];
};

struct VertexInput
{
    float3 position : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    uint4 refIndex : REFINDEX;
    float4 weight : WEIGHT;
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
    float3 uvw : UVW;
};

float3 CalculateWeightedPosition(float3 position, uint4 refIndex, float4 weight)
{
    float3 ret = { 0.0f, 0.0f, 0.0f};
    
    ret += weight[0] * mul(skin[refIndex[0]], float4(position, 1.0f)).xyz;
    ret += weight[1] * mul(skin[refIndex[1]], float4(position, 1.0f)).xyz;
    ret += weight[2] * mul(skin[refIndex[2]], float4(position, 1.0f)).xyz;
    ret += weight[3] * mul(skin[refIndex[3]], float4(position, 1.0f)).xyz;
    
    return ret;
}

float3 CalculateWeightedNormal(float3 normal, uint4 refIndex, float4 weight)
{
    float3 ret = { 0.0f, 0.0f, 0.0f};
    
    ret += weight[0] * mul((float3x3) skin[refIndex[0]], normal);
    ret += weight[1] * mul((float3x3) skin[refIndex[1]], normal);
    ret += weight[2] * mul((float3x3) skin[refIndex[2]], normal);
    ret += weight[3] * mul((float3x3) skin[refIndex[3]], normal);
    
    return ret;
}

PixelInput VS(VertexInput input)
{
    PixelInput output;
    
	////////////////////////////////////
    
    float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    weights[0] = input.weight.x;
    weights[1] = input.weight.y;
    weights[2] = input.weight.z;
    weights[3] = 1.0f - (input.weight.x + input.weight.y + input.weight.z);
    
	////////////////////////////////////
    
    
    output.position.xyz = CalculateWeightedPosition(input.position, input.refIndex, input.weight);
    output.position = LocalToNDC(output.position.xyz);
    
	////////////////////////////////////
    
    output.uv = input.uv;
    
	////////////////////////////////////
    
    output.normal = CalculateWeightedNormal(input.normal, input.refIndex, input.weight);
    output.normal = normalize(LocalToWorldDirOnly(output.normal));
    
	////////////////////////////////////
	
    return output;
}