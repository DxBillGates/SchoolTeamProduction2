struct VSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

cbuffer cbuff0 : register(b0)
{
	matrix view;
	matrix projection3d;
}

cbuffer cbuff1 : register(b1)
{
	matrix world;
	float4 color;
}