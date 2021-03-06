//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:vC[
//
//day:2017/06/30
//
//acotr:Koshida Kenji
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
#pragma once
#include "..\Character.h"
#include "..\..\Weapon\Weapon.h"

class PlayerState;

class Player : public Character
{
public:
	/// <summary>
	/// Ú®Ê
	/// </summary>
	struct Horizontal
	{
		float x;
		float z;
	};

	static const float WALK_SPEED;			//à­¬x
	static const float RUN_SPEED;			//é¬x
	static const float ATTACK_SPEED;		//UÌ¬x
	static const float EVADE_SPEED;			//ñðÌ¬x
	static const float GRAVITY_ACC;			//dÍÁ¬x
	static const float JUMP_SPEED_FIRST;	//Wv¬x
	static const float JUMP_SPEED_MAX;		//ÅåWv¬x
	static const float DAMAGE_EFFECT_TIMER;	//_[WGtFNgÌ^C}[
	static const int   MAX_HP;				//ÅåÌÍ

public:
	Player();
	virtual ~Player();

	//ú»
	void Initialize()override {};
	//XV
	void Update()override;
	//`æ
	void Render()override;
	//Ú®
	void Move(DirectX::SimpleMath::Vector3 movev)override;

	/// <summary>
	/// Zb^[
	/// </summary>
	//JpxÌÝè
	void SetCameraRotate(const DirectX::SimpleMath::Vector3& rotate) { m_camera_rotate = rotate; }
	//íÌÝè
	void SetWeapon(Weapon* weapon);
	//½ÊÚ®ÊÌÝè
	void SetHorizontal(float horizontalX,float horizontalZ)
	{
		m_horizontal.x = horizontalX;
		m_horizontal.z = horizontalZ;
	}
	//GÉ^¦é_[WÌÝè
	void SetDamage(int damage) { m_damage = damage; }
	//ã¸ÊÌÝè
	void SetVelocity(float velocity) { m_velocity.y = velocity; }
	//ãUüÍóÔÌÝè
	void SetLightAttackInputState(bool state) { m_is_lightattack = state; }
	//ñðüÍóÔÌÝè
	void SetEvadeInputState(bool is_evade) { m_is_evade = is_evade; }

	/// <summary>
	/// Qb^[
	/// </summary>
	//JpxÌæ¾
	const DirectX::SimpleMath::Vector3& GetCameraRotate() { return m_camera_rotate; }
	//Ú®ÊÌæ¾
	const Horizontal GetHorizontal() { return m_horizontal; }
	//¬xÌæ¾
	const DirectX::SimpleMath::Vector3& GetVelocity() { return m_velocity; }
	//íîñÌæ¾
	const Weapon* GetWeaponInfo() { return m_weapon; }
	//WvóÔÌæ¾
	const bool GetJumpState() { return m_is_jump; }
	//èüÍóÔÌæ¾
	const bool GetRunState() { return m_is_run; }
	//ãUüÍóÔÌæ¾
	const bool GetLightAttackInputState() { return m_is_lightattack; }
	//ñðóÔÌæ¾
	const bool GetEvadeInputState() { return m_is_evade; }
	//Ú®óÔÌæ¾
	bool IsMove();


	//ñ]
	void Rotation(const DirectX::SimpleMath::Vector3& anglev);
	//Jñ]
	void CameraRotatin(const DirectX::SimpleMath::Vector3& rotate);

	//WvJn
	void StartJump();
	//WvI¹
	void StopJump();
	//ºJn
	void StartFall();
	//é
	void Run(bool is_run);

	//_[WGtFNgON
	void DamageEffect() { m_status_canvas.SetVisible("damage_effect", true); }

	//UqbgGtFNg­¶
	void HitEffect(const DirectX::SimpleMath::Vector3& pos);
	//«ÕGtFNg­¶
	void Footsteps();
private:
	PlayerState* m_state;					        //vC[ÌóÔ

	Weapon* m_weapon;						        //í

	DirectX::SimpleMath::Vector3 m_camera_rotate;	//Jpx

	Horizontal m_horizontal;				        //Ú®Ê

	bool m_is_jump;							        //WvtO

	bool m_is_run;							        //étO

	DirectX::SimpleMath::Vector3 m_velocity;        //ãºÌÚ®Ê

	bool m_is_lightattack;					        //ãUJn

	bool m_is_heavyattack;					        //­UJn

	bool m_is_evade;						        //ñðJn

	float m_damage_effect_timer;			        //_[WGtFNgÌ\¦^C}[
};


//vC[ÌóÔJÚ
#pragma region State
class PlayerState
{
public:
	virtual ~PlayerState() {}

	virtual void Initialize(Player& player) = 0;

	virtual PlayerState* Update(Player& player) { return nullptr; }
};

//§¿óÔ
class StandingState : public PlayerState
{
public:
	StandingState() {}

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};

//à«óÔ
class WalkingState : public PlayerState
{
public:
	WalkingState() { m_timer = 0; }

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
private:
	float m_timer;		//¹ºp^C}[
};

//èóÔ
class RunningState : public PlayerState
{
public:
	RunningState() { m_timer = 0; }

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
private:
	float m_timer;		//¹ºp^C}[
};

//WvóÔ
class JumpingState : public PlayerState
{
public:
	JumpingState() {}

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};

//ãU1
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

//ãU2
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

//ãU3
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

//WvU1
class JumpAttack1State : public PlayerState
{
public:
	JumpAttack1State() {}

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};

//WvU2
class JumpAttack2State : public PlayerState
{
public:
	JumpAttack2State() {}

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};

//WvU3
class JumpAttack3State : public PlayerState
{
public:
	JumpAttack3State() {}

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};

//·µÝ
class PushInState : public PlayerState
{
public:
	PushInState() {}

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};

//ñð
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

//S
class DeadState : public PlayerState
{
public:
	DeadState() {}

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};

#pragma endregion