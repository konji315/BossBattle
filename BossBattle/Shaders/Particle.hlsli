//コンスタントバッファ
cbuffer global
{
	//ワールドビュープロジェクション合成行列
	row_major matrix g_WVP;
	row_major matrix g_Billboard;	//ビルボード行列
};

//テクスチャとテクスチャサンプラ
Texture2D g_texture    : register(t0);
SamplerState g_sampler : register(s0);

//ジオメトリシェーダへの入力
struct GSInput
{
	float4 pos      : SV_POSITION;
	float4 color    : COLOR;
	//x : scale , y : 回転角
	float2 other    : TEXCOORD;
};
//ピクセルシェーダへの入力
struct PSInput
{
	float4 pos   : SV_POSITION;
	float4 color : COLOR;
	float2 uv    : TEXCOORD;
};
