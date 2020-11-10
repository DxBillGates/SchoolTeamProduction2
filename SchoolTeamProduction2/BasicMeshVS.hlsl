#include"BasicMeshShader.hlsli"
VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD, float3 normal : NORMAL)
{
	VSOutput output;
	output.svpos = mul(projection3d, mul(view, mul(world, pos)));
	output.uv = uv;
	output.normal = normal;
	return output;
}