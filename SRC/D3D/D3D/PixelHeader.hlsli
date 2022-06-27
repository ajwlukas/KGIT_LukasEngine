Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1);
TextureCube skyBox : register(t2);
SamplerState Sampler : register(s0);

float4 GammaStart(float4 albedo)
{
    return albedo * albedo;
}

float4 GammaEnd(float4 albedo)
{
    return sqrt(albedo);
}