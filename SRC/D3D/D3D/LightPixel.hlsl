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
    
    float4 albedo = float4(1.0f, 0.0f, 1.0f, 1.0f);
    
    ////////////////////////////////
    
    float4 diffuse = albedo * saturate(dot(input.normal, float3(1, 0, 0)));
    
    ////////////////////////////////
    
    float4 ambient = albedo * 0.3f;
    
    ////////////////////////////////
    
    ret.rgb = diffuse.rgb + ambient.rgb;
    ret.a = 1.0f;
    
    ////////////////////////////////

    return ret;
}