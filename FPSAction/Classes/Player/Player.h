//～/～/～//～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/
//Content:プレイヤー
//
//day:2017/06/30
//
//acotr:Koshida Kenji
//～/～/～//～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/
#pragma once
#include <d3d11.h>
#include <SimpleMath.h>

#include "../Weapon/Weapon.h"
#include "../Collider/CollisionNode.h"
#include "../Texture/Canvas.h"

class PlayerState;

class Player
{
public:
	struct Horizontal
	{
		float x;
		float z;
	};

	static const float TURN_SPEED;			//回転速度
	static const float STEP_SPEED;			//ステップ速度
	static const float WALK_SPEED;			//歩く速度
	static const float RUN_SPEED;			//走る速度
	static const float STEP_TIME;			//ステップ時間
	static const float GRAVITY_ACC;			//重力加速度
	static const float JUMP_SPEED_FIRST;	//ジャンプ初速度
	static const float JUMP_SPEED_MAX;		//最大ジャンプ速度
	static const float DAMAGE_EFFECT_TIMER;	//ダメージエフェクトのタイマー

public:
	Player();
	virtual ~Player();

	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Render();

	//場所の設定
	void SetPos(const DirectX::SimpleMath::Vector3& pos) { m_pos = pos; }

	//角度の設定
	void SetAngle(const DirectX::SimpleMath::Vector3& angle) { m_angle = angle; }

	//武器の設定
	void SetWeapon(Weapon* weapon);

	//X軸移動量の設定
	void SetHorizontal(float horizontalX,float horizontalZ)
	{
		m_horizontal.x = horizontalX;
		m_horizontal.z = horizontalZ;
	}

	//場所の取得
	const DirectX::SimpleMath::Vector3& GetPos() { return m_pos; }

	//角度の取得
	const DirectX::SimpleMath::Vector3& GetAngle() { return m_angle; }

	//移動量の取得
	const Horizontal GetHorizontal() { return m_horizontal; }

	//移動速度の取得
	const float GetMoveSpeed() { return m_move_speed; }

	//当たり判定の取得
	const SphereNode& GetCollisionNodeBody() { return m_player_collision; }

	//速度の取得
	const DirectX::SimpleMath::Vector3& GetVelocity() { return m_velocity; }

	//ステップ
	bool Step(Horizontal horizontal);

	//移動
	void Move(DirectX::SimpleMath::Vector3 movev);

	//回転
	void Rotation(const DirectX::SimpleMath::Vector3& anglev);

	//ジャンプ開始
	void StartJump();

	//ジャンプ終了
	void StopJump();

	//落下開始
	void StartFall();

	//ジャンプ状態の取得
	bool IsJump() { return m_is_jump; }

	//走る
	void Run(bool is_run);

	//弱攻撃開始
	void StartLightAttack();

	//強攻撃開始
	void StartHeavyAttack();
private:
	PlayerState* m_state;					//プレイヤーの状態

	int m_max_hp;							//最大体力
	int m_hp;								//現在の体力

	Weapon* m_weapon;						//武器

	DirectX::SimpleMath::Vector3 m_pos;		//座標

	DirectX::SimpleMath::Vector3 m_angle;	//角度

	Horizontal m_horizontal;				//移動量

	float m_step_timer;						//ステップタイマー

	bool m_is_step;							//ステップフラグ

	float m_step_height;					//ステップの高さ

	float m_step_ammo;						//ステップ上昇量

	bool m_is_jump;							//ジャンプフラグ

	bool m_is_run;							//走るフラグ

	float m_move_speed;						//移動速度

	DirectX::SimpleMath::Vector3 m_velocity;//上下の移動量

	bool m_is_lightattack;					//弱攻撃中

	bool m_is_heavyattack;					//強攻撃中

	Obj3d m_tmp_player;						//プレイヤーの仮想オブジェクト

	SphereNode m_player_collision;			//プレイヤーの当たり判定

	Canvas m_status_canvas;					//ステータスキャンバス

	float m_damage_effect_timer;			//ダメージエフェクトの表示タイマー
};


//プレイヤーの状態遷移
class PlayerState
{
public:
	virtual ~PlayerState() {}

	virtual void Initialize(Player& player) = 0;

	virtual PlayerState* Update(Player& player) { return nullptr; }
};

//立ち状態
class StandingState : public PlayerState
{
public:
	StandingState();

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};

//歩き状態
class WalkingState : public PlayerState
{
public:
	WalkingState();

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};

//ジャンプ状態
class JumpingState : public PlayerState
{
public:
	JumpingState();

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};