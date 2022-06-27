struct PSInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};


float4 PS(PSInput input) : SV_Target
{
    return float4(0, 1.0f, 0, 1.0f);
}