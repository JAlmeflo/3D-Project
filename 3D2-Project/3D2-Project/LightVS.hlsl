//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix lightViewMatrix;
	matrix lightProjectionMatrix;
};

cbuffer LightBuffer2
{
	float3 lightPosition;
	float padding;
}

cbuffer FogBuffer
{
	float fogStart;
	float fogEnd;
	float padding1;
	float padding2;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 instancePosition : TEXCOORD1;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 lightViewPosition : TEXCOORD1;
	float3 lightPos : TEXCOORD2;
	float fogFactor : FOG;
};

// Vertex Shader
PixelInputType main(VertexInputType input)
{
	PixelInputType output;
	float4 worldPosition;
	float4 cameraPosition;

	input.position.w = 1.0f;

	input.position.x += input.instancePosition.x;
	input.position.y += input.instancePosition.y;
	input.position.z += input.instancePosition.z;

	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.lightViewPosition = mul(input.position, worldMatrix);
	output.lightViewPosition = mul(output.lightViewPosition, lightViewMatrix);
	output.lightViewPosition = mul(output.lightViewPosition, lightProjectionMatrix);

	output.tex = input.tex;

	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	worldPosition = mul(input.position, worldMatrix);
	output.lightPos = lightPosition.xyz - worldPosition.xyz;
	output.lightPos = normalize(output.lightPos);

	// calc fog
	cameraPosition = mul(input.position, worldMatrix);
	cameraPosition = mul(cameraPosition, viewMatrix);

	output.fogFactor = saturate((fogEnd - cameraPosition.z) / (fogEnd - fogStart));

	return output;
}