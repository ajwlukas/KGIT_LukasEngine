#include "PixelHeader.hlsli"


struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
};

PSOutput PS(PSInput input)
{
    PSOutput output;
    
    float4 ret = float4(1, 1, 1, 1);
    
    ////////////////////////////////
    
    float4 albedo = diffuseMap.Sample(Sampler, input.uv);
    output.albedo = albedo; //todo: 밑으로 내려서 정리해야징~
    
    
    ////////////////////////////////
    
    albedo = GammaStart(albedo);
    
    float4 diffuse = albedo * saturate(dot(input.normal, float3(1, 0, 0)));
    
    diffuse = GammaEnd(diffuse);
    
    ////////////////////////////////
    
    float4 ambient = albedo * 0.3f;
    
    ////////////////////////////////
    
    ret.rgb = diffuse.rgb + ambient.rgb;
    ret.a = 1.0f;
    
    ////////////////////////////////
    
    output.original = ret;
    output.normal = float4(input.normal, 1.0f);
    return output;
}