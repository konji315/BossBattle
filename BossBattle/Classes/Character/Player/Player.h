//～/～/～//～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/
//Content:プレイヤー
//
//day:2017/06/30
//
//acotr:Koshida Kenji
//～/～/～//～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/
#pragma once
#include "..\Character.h"
#include "..\..\Weapon\Weapon.h"

class PlayerState;

class Player : public Character
{
public:
	/// <summary>
	/// 移動量
	/// </summary>
	struct Horizontal
	{
		float x;
		float z;
	};

	static const float WALK_SPEED;			//歩く速度
	static const float RUN_SPEED;			//走る速度
	static const float ATTACK_SPEED;		//攻撃時の速度
	static const float EVADE_SPEED;			//回避時の速度
	static const float GRAVITY_ACC;			//重力加速度
	static const float JUMP_SPEED_FIRST;	//ジャンプ初速度
	static const float JUMP_SPEED_MAX;		//最大ジャンプ速度
	static const float DAMAGE_EFFECT_TIMER;	//ダメージエフェクトのタイマー
	static const int   MAX_HP;				//最大体力

public:
	Player();
	virtual ~Player();

	//初期化
	void Initialize()override {};
	//更新
	void Update()override;
	//描画
	void Render()override;
	//移動
	void Move(DirectX::SimpleMath::Vector3 movev)override;

	/// <summary>
	/// セッター
	/// </summary>
	//カメラ角度の設定
	void SetCameraRotate(const DirectX::SimpleMath::Vector3& rotate) { m_camera_rotate = rotate; }
	//武器の設定
	void SetWeapon(Weapon* weapon);
	//平面移動量の設定
	void SetHorizontal(float horizontalX,float horizontalZ)
	{
		m_horizontal.x = horizontalX;
		m_horizontal.z = horizontalZ;
	}
	//敵に与えるダメージの設定
	void SetDamage(int damage) { m_damage = damage; }
	//上昇量の設定
	void SetVelocity(float velocity) { m_velocity.y = velocity; }
	//弱攻撃入力状態の設定
	void SetLightAttackInputState(bool state) { m_is_lightattack = state; }
	//回避入力状態の設定
	void SetEvadeInputState(bool is_evade) { m_is_evade = is_evade; }

	/// <summary>
	/// ゲッター
	/// </summary>
	//カメラ角度の取得
	const DirectX::SimpleMath::Vector3& GetCameraRotate() { return m_camera_rotate; }
	//移動量の取得
	const Horizontal GetHorizontal() { return m_horizontal; }
	//速度の取得
	const DirectX::SimpleMath::Vector3& GetVelocity() { return m_velocity; }
	//武器情報の取得
	const Weapon* GetWeaponInfo() { return m_weapon; }
	//ジャンプ状態の取得
	const bool GetJumpState() { return m_is_jump; }
	//走り入力状態の取得
	const bool GetRunState() { return m_is_run; }
	//弱攻撃入力状態の取得
	const bool GetLightAttackInputState() { return m_is_lightattack; }
	//回避状態の取得
	const bool GetEvadeInputState() { return m_is_evade; }
	//移動状態の取得
	bool IsMove();


	//回転
	void Rotation(const DirectX::SimpleMath::Vector3& anglev);
	//カメラ回転
	void CameraRotatin(const DirectX::SimpleMath::Vector3& rotate);

	//ジャンプ開始
	void StartJump();
	//ジャンプ終了
	void StopJump();
	//落下開始
	void StartFall();
	//走る
	void Run(bool is_run);

	//ダメージエフェクトON
	void DamageEffect() { m_status_canvas.SetVisible("damage_effect", true); }

	//攻撃ヒットエフェクト発生
	void HitEffect(const DirectX::SimpleMath::Vector3& pos);
	//足跡エフェクト発生
	void Footsteps();
private:
	PlayerState* m_state;					        //プレイヤーの状態

	Weapon* m_weapon;						        //武器

	DirectX::SimpleMath::Vector3 m_camera_rotate;	//カメラ角度

	Horizontal m_horizontal;				        //移動量

	bool m_is_jump;							        //ジャンプフラグ

	bool m_is_run;							        //走るフラグ

	DirectX::SimpleMath::Vector3 m_velocity;        //上下の移動量

	bool m_is_lightattack;					        //弱攻撃開始

	bool m_is_heavyattack;					        //強攻撃開始

	bool m_is_evade;						        //回避開始

	float m_damage_effect_timer;			        //ダメージエフェクトの表示タイマー
};


//プレイヤーの状態遷移
#pragma region State
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
	StandingState() {}

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};

//歩き状態
class WalkingState : public PlayerState
{
public:
	WalkingState() { m_timer = 0; }

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
private:
	float m_timer;		//音声用タイマー
};

//走り状態
class RunningState : public PlayerState
{
public:
	RunningState() { m_timer = 0; }

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
private:
	float m_timer;		//音声用タイマー
};

//ジャンプ状態
class JumpingState : public PlayerState
{
public:
	JumpingState() {}

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};

//弱攻撃1
class LightAttack1State : public PlayerState
{
public:
	LightAttack1State() {}

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
private:
	DirectX::SimpleMath::Vector2 m_move_vec;
	float m_attack_speed;
};

//弱攻撃2
class LightAttack2State : public PlayerState
{
public:
	LightAttack2State() {}

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
private:
	DirectX::SimpleMath::Vector2 m_move_vec;
	float m_attack_speed;
};

//弱攻撃3
class LightAttack3State : public PlayerState
{
public:
	LightAttack3State() {}

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
private:
	DirectX::SimpleMath::Vector2 m_move_vec;
	float m_attack_speed;
};

//ジャンプ攻撃1
class JumpAttack1State : public PlayerState
{
public:
	JumpAttack1State() {}

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};

//ジャンプ攻撃2
class JumpAttack2State : public PlayerState
{
public:
	JumpAttack2State() {}

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};

//ジャンプ攻撃3
class JumpAttack3State : public PlayerState
{
public:
	JumpAttack3State() {}

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};

//剣差し込み
class PushInState : public PlayerState
{
public:
	PushInState() {}

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};

//回避
class EvadeState : public PlayerState
{
public:
	EvadeState() {}

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
private:
	DirectX::SimpleMath::Vector2 m_move_vec;
	float m_evade_speed;
};

//死亡
class DeadState : public PlayerState
{
public:
	DeadState() {}

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};

#pragma endregion