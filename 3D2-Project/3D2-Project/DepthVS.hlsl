//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType
{
	float4 position : POSITION;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 depthPosition : TEXTURE0;
};

// Vs
PixelInputType main(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.0f;

	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.depthPosition = output.position;

	return output;
}