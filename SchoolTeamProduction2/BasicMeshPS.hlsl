#include"BasicMeshShader.hlsli"
float4 main(VSOutput input) : SV_TARGET
{
	float3 light = normalize(float3(0,0,1));
	float3 light_diffuse = saturate(dot(-light, input.normal));
	return float4(light_diffuse+0.5f, 1);
}