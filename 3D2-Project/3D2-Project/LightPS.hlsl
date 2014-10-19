//float4 main() : SV_TARGET
//{
//	return float4(1.0f, 1.0f, 1.0f, 1.0f);
//}

Texture2D shaderTexture;
SamplerState samplerType;

cbuffer LightBuffer
{
	float4 diffuseColor;
	float3 lightDirection;
	float padding;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

// Pixel Shader
float4 main(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float lightIntensity;
	float4 color;

	textureColor = shaderTexture.Sample(samplerType, input.tex);

	lightDir = -lightDirection;

	lightIntensity = saturate(dot(input.normal, lightDir));

	color = saturate(diffuseColor * lightIntensity);

	color = (color + 0.1f) * textureColor;

    return color;
}