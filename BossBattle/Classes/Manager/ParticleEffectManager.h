//～/～/～//～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/
//Content:Particle Effect Manager
//
//day:2017/11/02
//
//actor:Koshida Kenji
//～/～/～//～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/

#include <vector>
#include <Windows.h>
#include <wrl\client.h>
#include <memory>

#include <d3d11.h>

#include <SimpleMath.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <CommonStates.h>
#include <string>
#include <map>

#include "..\Camera\Camera.h"
#include "..\Common\DXTKResouces.h"
#include "..\Utility\BinaryFile.h"

#pragma once

class ParticleEmitter
{
public:
	enum EmitterType
	{
		CONE,		//コーン
		SPHERE		//スフィア
	};
public:
	//コンストラクタ
	ParticleEmitter() {}
	//デストラクタ
	~ParticleEmitter()
	{
		//swap処理によるメモリの解放
		std::vector<DirectX::VertexPositionColorTexture>().swap(m_Vertices);
		std::vector<DirectX::SimpleMath::Vector3>().swap(m_MoveVec);
		std::vector<float>().swap(m_LifeTimer);
		std::vector<float>().swap(m_GravInf);
	}

	//初期化
	void Initialize(
		const float duration							                //持続時間
		, const DirectX::SimpleMath::Vector3 pos		                //位置
		, const DirectX::SimpleMath::Vector2 rotate		                //発射角度
		, const wchar_t* filepath						                //テクスチャのパス
		, const float startrotate						                //初期角度
		, const float endrotate							                //最終角度
		, const float startscale = 1.0f					                //初期サイズ
		, const float endscale = 1.0f					                //最終サイズ
		, const DirectX::SimpleMath::Vector4 startcolor = { 1,1,1,1 }	//初期カラー
		, const DirectX::SimpleMath::Vector4 endcolor = { 1,1,1,1 }     //最終カラー
		, const float lifetime = 5						                //寿命
		, const float startspeed = 5					                //初速度
		, const float endspeed = 5						                //最終速度
		, const float gravity = 0						                //重力
		, const float angle = 25						                //放射角度
		, const int ratetime = 10						                //発射レート
		, const bool burst = false						                //一斉発射
		, const bool loop = true										//ループ
		, const EmitterType type = EmitterType::CONE);			        //発射方法

	//更新
	void Update();
	//描画
	void Render(DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>* pb);

	/// <summary>
	/// セッター
	/// </summary>
	void SetPos(const DirectX::SimpleMath::Vector3& pos) { m_Pos = pos; }
	void SetRotate(const DirectX::SimpleMath::Vector2& rotate) { m_Rotate = rotate; }
	void SetChild(std::string child_name) { m_child_emitter_name = child_name; }

private:
	//タイマー
	int m_PlayBackTimer;
	//パーティクル
	std::vector<DirectX::VertexPositionColorTexture> m_Vertices;
	//テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_Texture;
	//移動方向
	std::vector<DirectX::SimpleMath::Vector3> m_MoveVec;
	//パーティクル個々の残り寿命
	std::vector<float> m_LifeTimer;
	//重力影響度
	std::vector<float> m_GravInf;

	//持続時間
	float m_Duration;
	//発生位置
	DirectX::SimpleMath::Vector3 m_Pos;
	//最初の色
	DirectX::SimpleMath::Vector4 m_StartColor;
	//最後の色
	DirectX::SimpleMath::Vector4 m_EndColor;
	//発生角度
	DirectX::SimpleMath::Vector2 m_Rotate;
	//最初のパーティクルの角度
	float m_StartRotate;
	//最後のパーティクルの角度
	float m_EndRotate;
	//最初の大きさ
	float m_StartScale;
	//最後の大きさ
	float m_EndScale;
	//寿命
	float m_LifeTime;
	//ループ
	bool m_Loop;
	//最初のパーティクル速度
	float m_StartSpeed;
	//最後のパーティクル速度
	float m_EndSpeed;
	//重力
	float m_Gravity;
	//発射範囲
	float m_Angle;
	//バースト
	bool m_Burst;
	//発射レート
	int m_Rate;
	//発射方法
	EmitterType m_Type;
	//子供の名前
	std::string m_child_emitter_name;

	//パーティクルの生成
	void CreateParticle();
};


class ParticleEffectManager : public singleton<ParticleEffectManager>
{
public:
	static const int PARTICLE_NUM_MAX;

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	struct Constants
	{
		//ワールドビュープロジェクション合成行列
		DirectX::SimpleMath::Matrix WVP;
		DirectX::SimpleMath::Matrix BILLBOARD;
	};

public:
	friend class singleton<ParticleEffectManager>;

	ParticleEffectManager();
	virtual ~ParticleEffectManager();

	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Render();

	//パーティクルエミッター生成
	ParticleEmitter* Play(std::string name, DirectX::SimpleMath::Vector3 pos, float lifetime, std::string childname = "");

	/// <summary>
	/// エミッターの操作
	/// </summary>
	void SetCamera(Camera* camera) { m_Camera = camera; }

	//パーティクル登録
	void RegisterParticle(std::string name , ParticleEmitter* pe);

	//テクスチャ群
	std::map<const wchar_t*, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>	  m_TextureResources;

private:
	//頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader>                                    m_VertexShader;
	//ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>                                  m_GeometryShader;
	//ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader>                                     m_PixelShader;
	//プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_PrimitiveBatch;
	//入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout>                                     m_InputLayout;
	//汎用描画設定
	std::unique_ptr<DirectX::CommonStates>                                        m_CommonStates;
	//ウィンドウサイズ
	RECT                                                                          m_WindowSize;
	//コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>                                          m_ConstantsBuffer;
	//テクスチャサンプラ
	Microsoft::WRL::ComPtr<ID3D11SamplerState>                                    m_Sampler;
	//ブレンドステート
	Microsoft::WRL::ComPtr<ID3D11BlendState>									  m_BlendState;
	//カメラ
	Camera*															  m_Camera;
	//登録パーティクル群
	std::map<std::string,ParticleEmitter*>										  m_RegisteredEmitters;
	//更新パーティクル群
	std::vector<ParticleEmitter*>												  m_ParticleEmitters;
	//エミッターの残り寿命
	std::vector<float>															  m_LifeTimer;
};


//class Particle
//{
//public:
//
//private:
//	DirectX::SimpleMath::Vector2 m_MoveVec;	//移動方向
//};