//struct GSOutput
//{
//	float4 pos : SV_POSITION;
//};
//
//[maxvertexcount(3)]
//void main(triangle float4 input[3] : SV_POSITION, inout TriangleStream< GSOutput > output)
//{
//	for (uint i = 0; i < 3; i++)
//	{
//		GSOutput element;
//		element.pos = input[i];
//		output.Append(element);
//	}
//}

#define PT_EMITTER 0
#define PT_FLARE 1

cbuffer MatrixBuffer
{
	matrix viewMatrix;
}

cbuffer cbAcceleration
{
	float3 acceleration = { 1.0f, -9.82f, 0.0f };
}

struct VertexOutputType
{
	float3 position : POSITION;
	uint type : TYPE;
};

struct GeometryOutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
};

[maxvertexcount(2)]
void main(point VertexOutputType input[1], inout LineStream<GeometryOutputType> output)
{
	if (input[0].type != PT_EMITTER)
	{
		float3 p0 = input[0].position;
		float3 p1 = input[0].position + 0.07f * acceleration;

		GeometryOutputType v0;
		v0.position = mul(float4(p0, 1.0f), viewMatrix);
		v0.tex = float2(0.0f, 0.0f);
		output.Append(v0);

		GeometryOutputType v1;
		v1.position = mul(float4(p1, 1.0f), viewMatrix);
		v1.tex = float2(1.0f, 1.0f);
		output.Append(v1);
	}
}