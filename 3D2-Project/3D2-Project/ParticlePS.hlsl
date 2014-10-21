//float4 main() : SV_TARGET
//{
//	return float4(1.0f, 1.0f, 1.0f, 1.0f);
//}

Texture2D shaderTexture;
SamplerState sampleType;

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
	float4 color : COLOR;
};

float4 main(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	float4 finalColor;

	textureColor = shaderTexture.Sample(sampleType, input.tex);

	return textureColor * input.color;
}