#include "PixelHeader.hlsli"

struct PSInput
{
    float4 position : SV_POSITION;
    float4 posW : POS_WORLD;
};

bool CloseToEqual(float a, float b, float gap)//gap == ¼±ÀÇ ±½±â
{
    return abs(a - b) < gap;
}

float4 PS(PSInput input) : SV_Target
{
    float4 ret = float4(0, 0, 0, 0);
    
    ////////////////////////////////
    
    //if (closeEqual(pow(input.posW.z, 2.0f) + pow(input.posW.x, 2.0f), 25.0f, 1.0f))
   
    if (CloseToEqual(input.posW.x, 0.0f, 1.0f))
        return float4(0, 0, 1, 1);
    
    if (CloseToEqual(input.posW.z, 0.0f, 1.0f))
        return float4(1, 0, 0, 1);
    
    if (CloseToEqual(input.posW.x % 10, 0.0f, 0.5f))
        return float4(1, 1, 1, 1);
    
    if (CloseToEqual(input.posW.z % 10, 0.0f, 0.5f))
        return float4(1, 1, 1, 1);
    
    /// ¾ÈÀç¿ì »Ç±Û»Ç±Û ¸Ó¸® »Ç±Û»Ç±Û»Ç±Û»Ç±Û ¸ÀÁÁÀº ¶ó¸é Â¥Àå¹ü¹÷ ¾ßÈ£
    
    ////////////////////////////////
    
    return ret;
}