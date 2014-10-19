//float4 main() : SV_TARGET
//{
//	return float4(1.0f, 1.0f, 1.0f, 1.0f);
//}

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 depthPosition : TEXTURE0;
};

// PS
float4 main(PixelInputType input) : SV_TARGET
{
	float depthValue;
	float4 color;

	depthValue = input.depthPosition.z / input.depthPosition.w;

	color = float4(1.0f * depthValue, 1.0f * depthValue, 1.0f * depthValue, 1.0f);

	return color;
}