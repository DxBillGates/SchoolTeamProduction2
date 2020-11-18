#include"BasicMeshShader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = tex.Sample(smp,input.uv);
	float3 light = normalize(float3(0,-1,1));
	float3 light_diffuse = saturate(dot(-light, input.normal));
	return float4(texcolor.xyz * light_diffuse, texcolor.a);
}