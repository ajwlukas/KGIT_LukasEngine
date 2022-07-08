#include "PixelHeader.hlsli"

struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
};

PSOutput PS(PSInput input)
{
    PSOutput output;
    
    float4 ret = float4(1, 1, 1, 1);
    
    ////////////////////////////////
    
    ret.rgb = input.normal;
    
    ////////////////////////////////
    
    ret.a = 1.0f;
    
    ////////////////////////////////
    
    output.original = ret;
    output.normal = float4(input.normal, 1.0f);
    output.albedo = float4(input.normal, 1.0f);
    return output;
}