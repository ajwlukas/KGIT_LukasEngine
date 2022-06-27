#include "PixelHeader.hlsli"


struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
};

float4 PS(PSInput input) : SV_Target
{
    float4 ret = float4(1, 1, 1, 1);
    
    ////////////////////////////////
    
    ret = diffuseMap.Sample(Sampler, input.uv);
    
    ////////////////////////////////
    
    ret.a = 1.0f;
    
    ////////////////////////////////
    
    return ret;
}