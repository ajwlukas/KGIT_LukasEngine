

cbuffer World : register(b0)
{
	float4x4 world;
};

cbuffer ViewProj : register(b1)
{
	float4x4 viewproj;
};

struct VSInput
{
	float dummy : DUMMY;
	float3 position : POSITION;
	float4 color : COLOR;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PSInput VS(VSInput input)
{
	PSInput output;
	
    output.position = mul(world, float4(input.position, 1.0f));//���켱�̶� �ݴ�� ����, �̰� �� �����ٴµ�
    output.position = mul(viewproj, output.position);
	output.color = input.color;
	
	return output;
}
