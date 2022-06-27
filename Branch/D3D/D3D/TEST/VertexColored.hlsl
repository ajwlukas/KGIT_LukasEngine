
struct VSInput
{
	float4 position : POSITION;
	float4 color : COLOR;
};

VSInput main( VSInput input )
{
	return input;
}