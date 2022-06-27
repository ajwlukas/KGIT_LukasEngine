#include "PixelHeader.hlsli"


struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
};

PSOutput PS(PSInput input)
{
    PSOutput output;
    
    float4 ret = float4(1, 1, 1, 1);
    
    ////////////////////////////////
    
    ret = diffuseMap.Sample(Sampler, input.uv);
    
    ////////////////////////////////
    
    ret.a = 1.0f;
    
    output.orginal = ret;
    output.normal = float4(0.0f, 0.0f, 0.0f, 0.0f);
    output.albedo = ret; 
    
    return output;
}