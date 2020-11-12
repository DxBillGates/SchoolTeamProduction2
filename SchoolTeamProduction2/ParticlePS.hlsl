#include"ParticleShader.hlsli"
float4 main(VSOutput input) : SV_TARGET
{
	float3 light = normalize(float3(0,0,1));
	float3 light_diffuse = saturate(dot(-light, input.normal)) + 0.3f;
	return float4(light_diffuse * color.xyz, color.a);
}