#include "Particle.hlsli"

//頂点数
static const uint vnum = 4;

//中心点からのオフセット
static const float4 offset_array[vnum] =
{
	float4(-0.5f , -0.5f, 0, 0) ,	//左下
	float4( 0.5f , -0.5f, 0, 0) ,	//右下
	float4(-0.5f ,  0.5f, 0, 0) ,	//左上
	float4( 0.5f ,  0.5f, 0, 0)		//右上
};

//
static const float2 uv_array[vnum] =
{
	float2(0,1),	//左下
	float2(1,1),	//右下
	float2(0,0),	//左上
	float2(1,0)		//右上
};

[maxvertexcount(4)]
void main(
	point GSInput input[1],
	inout TriangleStream< PSInput > output
)
{
	//for (uint j = 0; j < 3; j++)
	{
		GSInput element;

		float4 offset = float4(0, 0, 0, 0);

		float scale   = input[0].other.x;
		float rot     = input[0].other.y;

		//4点描画
		for (uint i = 0; i < vnum; i++)
		{
			//指定角度分回転
			offset.x = offset_array[i].x * cos(rot)  + offset_array[i].y * sin(rot);
			offset.y = offset_array[i].x * -sin(rot) + offset_array[i].y * cos(rot);
			offset.z = 0;
			offset.w = 0;

			//ビルボード回転
			offset = mul(offset, g_Billboard);
			//平行移動
			element.pos = input[0].pos + offset * scale;
			//座標変換(3D->2D)
			element.pos = mul(element.pos, g_WVP);
			element.color = input[0].color;
			element.other = uv_array[i];

			output.Append(element);
		}

		//×

		//{
		//	element.pos = input[0].pos + float4((j / 1000.0f), (j / 1000.0f), 0, 0);
		//	element.pos = mul(element.pos, g_WVP);
		//	element.color = input[0].color;
		//	element.uv = input[0].uv;
		//	output.Append(element);
		//	element.pos = input[0].pos + float4((j / 1000.0f), 50 / 1000.0f - (j / 1000.0f), 0, 0);
		//	element.pos = mul(element.pos, g_WVP);
		//	output.Append(element);
		//}

		//円

		//{
		//	element.pos = input[0].pos + float4(cos(j / 360.0f * 100.0f) / 10, sin(j / 360.0f * 100.0f) / 10, 0, 0);
		//	element.pos = mul(element.pos, g_WVP);
		//	element.color = input[0].color;
		//	element.uv = input[0].uv;
		//	output.Append(element);
		//}

		//８

		//{
		//	element.pos = input[0].pos + float4(sin((j / 360.0f * 100.0f) + (j / 360.0f * 100.0f)) / 10, cos(j / 360.0f * 100.0f) / 10, 0, 0);
		//	element.pos = mul(element.pos, g_WVP);
		//	element.color = input[0].color;
		//	element.uv = input[0].uv;
		//	output.Append(element);
		//}

		////プリミティブの区切り
		//output.RestartStrip();
	}
}