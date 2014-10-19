//float4 main() : SV_TARGET
//{
//	return float4(1.0f, 1.0f, 1.0f, 1.0f);
//}

Texture2D shaderTexture : register(t0);
Texture2D depthMapTexture : register(t1);

SamplerState samplerTypeClamp : register(s0);
SamplerState samplerTypeWrap : register(s1);

cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 lightViewPosition : TEXCOORD1;
	float3 lightPos : TEXCOORD2;
};

// Pixel Shader
float4 main(PixelInputType input) : SV_TARGET
{
	float bias;
	float4 textureColor;
	float lightIntensity;
	float4 color;
	float2 projectTexCoord;
	float depthValue;
	float lightDepthValue;

	bias = 0.001f;

	color = ambientColor;

	// Calc projected texture coordinates
	projectTexCoord.x = input.lightViewPosition.x / input.lightViewPosition.w * 0.5f + 0.5f;
	projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w * 0.5f + 0.5f;

	// if (0-1) then pixel is in the view of light
	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		depthValue = depthMapTexture.Sample(samplerTypeClamp, projectTexCoord).r;

		lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;
		lightDepthValue = lightDepthValue - bias;

		// Light hit the pixel
		if (lightDepthValue < depthValue)
		{
			lightIntensity = saturate(dot(input.normal, input.lightPos));

			if (lightIntensity > 0.0f)
			{
				color += (diffuseColor * lightIntensity);
				color = saturate(color);
			}
		}
	}


	textureColor = shaderTexture.Sample(samplerTypeWrap, input.tex);	
	color = color * textureColor;

	return color;
}