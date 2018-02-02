#include "ParticleEffects.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void ParticleEffects::RegistParticles()
{
	ParticleEffectManager& pem = singleton<ParticleEffectManager>::GetInstance();

	//斬撃Hit
	{
		ParticleEmitter* halo;
		halo = new ParticleEmitter();
		halo->Initialize(
			2.0f									//持続時間
			, Vector3(0, 0, 0)						//座標
			, Vector2(0, 1)							//角度
			, L"Resources/Textures/Halo.png"		//テクスチャ
			, 0.0f									//最初の角度
			, 0.0f									//最後の角度
			, 0.0f									//初期大きさ
			, 5.0f									//最終大きさ
			, Vector4(1, 1, 0.6f, 1)					//最初の色
			, Vector4(1, 1, 1, 0.0f)					//最後の色
			, 0.2f									//寿命
			, 0.0f									//初速度
			, 0.0f									//最終速度
			, 0.00f									//重力
			, 90.0f									//発射角度
			, 1										//レート
			, true									//バースト
			, false									//ループ
			, ParticleEmitter::CONE);				//発射方法

		pem.RegisterParticle("halo", halo);
	}
	//斬撃
	{
		ParticleEmitter* slash;
		slash = new ParticleEmitter();
		slash->Initialize(
			2.0f									//持続時間
			, Vector3(0, 0, 0)						//座標
			, Vector2(0, 1)							//角度
			, L"Resources/Textures/slash.png"		//テクスチャ
			, 0.0f									//最初の角度
			, 0.0f									//最後の角度
			, 0.0f									//初期大きさ
			, 10.0f									//最終大きさ
			, Vector4(1, 1, 0.6f, 1)					//最初の色
			, Vector4(1, 1, 1, 0.0f)					//最後の色
			, 0.2f									//寿命
			, 0.0f									//初速度
			, 0.0f									//最終速度
			, 0.00f									//重力
			, 90.0f									//発射角度
			, 1										//レート
			, true									//バースト
			, false									//ループ
			, ParticleEmitter::CONE);				//発射方法

		pem.RegisterParticle("slash", slash);
	}

	//衝撃波
	{
		ParticleEmitter* shockwave;
		shockwave = new ParticleEmitter();
		shockwave->Initialize(
			2.0f									//持続時間
			, Vector3(0, 10, 0)						//座標
			, Vector2(0, 1)							//角度
			, L"Resources/Textures/Line_2.png"		//テクスチャ
			, 0.0f									//最初の角度
			, 0.0f									//最後の角度
			, 0.0f									//初期大きさ
			, 10.0f									//最終大きさ
			, Vector4(1, 1, 1, 1)					//最初の色
			, Vector4(0, 0, 0, 0)					//最後の色
			, 2.0f									//寿命
			, 40.0f									//初速度
			, 50.0f									//最終速度
			, 0.00f									//重力
			, 90.0f									//発射角度
			, 100									//レート
			, true									//バースト
			, false									//ループ
			, ParticleEmitter::CONE);				//発射方法

		pem.RegisterParticle("shockwave", shockwave);
	}

	//足跡エフェクト
	{
		ParticleEmitter* footsteps;
		footsteps = new ParticleEmitter();
		footsteps->Initialize(
			1.0f									//持続時間
			, Vector3(0, 0, 0)						//座標
			, Vector2(0, 1)							//角度
			, L"Resources/Textures/Burst_2.png"		//テクスチャ
			, 0.0f									//最初の角度
			, 0.0f									//最後の角度
			, 0.0f									//初期大きさ
			, 5.0f									//最終大きさ
			, Vector4(1, 1, 1, 1)					//最初の色
			, Vector4(1, 1, 1, 0.0f)					//最後の色
			, 0.2f									//寿命
			, 0.0f									//初速度
			, 0.0f									//最終速度
			, 0.00f									//重力
			, 90.0f									//発射角度
			, 1										//レート
			, true									//バースト
			, false									//ループ
			, ParticleEmitter::CONE);				//発射方法

		pem.RegisterParticle("footsteps", footsteps);
	}

	//ボス撃破エフェクト
	{
		ParticleEmitter* explosion1;
		explosion1 = new ParticleEmitter();
		explosion1->Initialize(
			2.0f									//持続時間
			, Vector3(0, 0, 0)						//座標
			, Vector2(0, 1)							//角度
			, L"Resources/Textures/Particle_Soft.png"		//テクスチャ
			, 0.0f									//最初の角度
			, 0.0f									//最後の角度
			, 5.0f									//初期大きさ
			, 1.0f									//最終大きさ
			, Vector4(1, 1, 0.6f, 1)					//最初の色
			, Vector4(1, 1, 1, 0.0f)					//最後の色
			, 1.0f									//寿命
			, 30.0f									//初速度
			, 0.0f									//最終速度
			, 0.1f									//重力
			, 180.0f									//発射角度
			, 50										//レート
			, true									//バースト
			, false									//ループ
			, ParticleEmitter::SPHERE);				//発射方法

		pem.RegisterParticle("explosion1", explosion1);
	}
}