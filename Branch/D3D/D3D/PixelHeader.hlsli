Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1);
TextureCube skyBox : register(t3);
SamplerState Sampler : register(s0);

struct PSOutput
{
    float4 albedo : SV_Target0;
    float4 normal : SV_Target1;
    float4 original : SV_Target2;
};

float4 GammaStart(float4 albedo)
{
    return albedo * albedo;
}

float4 GammaEnd(float4 albedo)
{
    return sqrt(albedo);
}