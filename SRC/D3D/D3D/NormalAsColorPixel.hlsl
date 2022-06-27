#include "PixelHeader.hlsli"

struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
};

float4 PS(PSInput input) : SV_Target
{
    float4 ret = float4(1, 1, 1, 1);
    
    ////////////////////////////////
    
    ret.rgb = input.normal;
    
    ////////////////////////////////
    
    ret.a = 1.0f;
    
    ////////////////////////////////
    
    return ret;
}