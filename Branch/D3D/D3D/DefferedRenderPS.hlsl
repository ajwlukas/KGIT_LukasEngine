Texture2D albedoMap : register(t0);
Texture2D normalMap : register(t1);

SamplerState Sampler : register(s0);

float4 GammaStart(float4 albedo)
{
    return albedo * albedo;
}

float4 GammaEnd(float4 albedo)
{
    return sqrt(albedo);
}


struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
};

float4 PS(PSInput input) : SV_Target
{
    float4 ret = float4(0, 0, 0, 0);
    
    ////////////////////////////////
    
    float4 albedo = saturate(albedoMap.Sample(Sampler, input.uv));
    
    if(albedo.r == 0.0f &&  albedo.g == 0.0f && albedo.b == 0.0f && albedo.a == 0.0f)
        return ret;
    
    ////////////////////////////////
    
    float3 normal = normalMap.Sample(Sampler, input.uv).xyz;
    
    ////////////////////////////////
    
    albedo = GammaStart(albedo);
    
    float4 diffuse = albedo * saturate(dot(normal, float3(1, 0, 0)));
    
    diffuse = GammaEnd(diffuse);
    
    ////////////////////////////////
    
    float4 ambient = albedo * 0.3f;
    
    ////////////////////////////////
    
    ret= diffuse + ambient;
    ret.a = 1.0f;
    
    ////////////////////////////////
    
    return ret;
}