#include "PixelHeader.hlsli"

struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float4 posW : POS_WORLD;
    float3 campos : CAM_POSITION;
};


PSOutput PS(PSInput input)
{
    PSOutput output;
    
    float4 ret = float4(1, 1, 1, 1);
    
    ////////////////////////////////
    
    float4 albedo = diffuseMap.Sample(Sampler, input.uv);
    output.albedo = albedo;//todo: 밑으로 내려서 정리해야징~
    
    ////////////////////////////////
    
    albedo = GammaStart(albedo);
    
    float3 normal = normalMap.Sample(Sampler, input.uv);
    normal = normal * 2.0f - 1.0f;// 0 ~ 1 to -1 ~ +1
    
    float3x3 TBN = float3x3(input.tangent, input.binormal, input.normal);
    
    normal = mul(normal,TBN);
    float4 diffuse = albedo * saturate(dot(normal, float3(1, 0, 0)));
    
    diffuse = GammaEnd(diffuse);
    
    ////////////////////////////////
    
    float4 ambient = albedo * 0.3f;
    
    ////////////////////////////////
    
    float3 cubeUVW = float3(1, 1, 1);
    float3 camToPos = normalize((float3) input.posW - input.campos);
    cubeUVW = reflect(camToPos, normal);
    float4 cubeAlbedo = skyBox.Sample(Sampler, cubeUVW);
    cubeAlbedo = cubeAlbedo * 1.0f;
    
    ////////////////////////////////
    
    ret.rgb = diffuse.rgb + ambient.rgb + cubeAlbedo.rgb;
    ret.a = 1.0f;
    
    ////////////////////////////////
    
    output.original = ret;
    output.normal = float4(normal, 1.0f);
    return output;
}