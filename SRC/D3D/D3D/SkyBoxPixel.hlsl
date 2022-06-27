#include "PixelHeader.hlsli"

struct PSInput
{
    float4 position : SV_POSITION;
    float3 uvw : UVW;
};

float4 PS(PSInput input) : SV_Target
{
    float4 ret = float4(1, 1, 1, 1);
    
    ////////////////////////////////
    
    ret = skyBox.Sample(Sampler, input.uvw);
    
    ////////////////////////////////
    
    ret.a = 1.0f;
    
    ////////////////////////////////
    
    return ret;
}