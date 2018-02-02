//ピクセルシェーダ
#include "Particle.hlsli"

float4 main(PSInput In) : SV_TARGET
{
	float4 color = g_texture.Sample(g_sampler,In.uv) * In.color;

	//color = float4(0, 0, 0, 1);

	//反転
	//color.r = 1-color.r;
	//color.g = 1-color.g;
	//color.b = 1-color.b;

	return color;
}