//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}

cbuffer cbAcceleration
{
	float3 acceleration = { 1.0f, -9.82f, 0.0f };
}

struct VertexInputType
{
	float3 initialPosition : POSITION;
	float3 initialVelocity : VELOCITY;
	float2 size : SIZE;
	float age : AGE;
	uint type : TYPE;
};

struct VertexOutputType
{
	float3 position : POSITION;
	uint type : TYPE;
};

VertexOutputType main(VertexInputType input)
{
	VertexOutputType output;

	float t = input.age;

	output.position = 0.5f * t * t * acceleration + t * input.initialVelocity + input.initialPosition;

	output.type = input.type;

	return output;
}