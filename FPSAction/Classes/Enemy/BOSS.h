//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:ƒ{ƒXƒNƒ‰ƒX
//
//day:2017/07/22
//
//acotr:Koshida Kenji
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
#pragma once
#include "Enemy.h"
#include "../Magic/Magic.h"

class BOSSState;

class BOSS : public Enemy
{
public:
	static const DirectX::SimpleMath::Vector3 IDLE_MOVE_POINT;
	static const DirectX::SimpleMath::Vector3 START_ANGLE;
	static const float SHAKE_MOVE;

public:
	BOSS();
	~BOSS();

	void Initialize()override;
	void Update()override;
	void Render()override;

	void SetIdlePos(const DirectX::SimpleMath::Vector3 pos1 , const DirectX::SimpleMath::Vector3 pos2);

	void SetMovePos(const DirectX::SimpleMath::Vector3& start_pos, const DirectX::SimpleMath::Vector3& end_pos);

	bool Idle();

	bool Move();

	void ShotBall();

	bool DeathMotion();
private:
	std::vector<Magic*> m_boss_ball;

	Effekseer::Handle m_death_effect;

	BOSSState* m_state;								//ƒ{ƒX‚Ìó‘Ô‘JˆÚ

	float m_idle_timer;								//‘Ò‹@ƒ^ƒCƒ}[
	int m_idle_count;								//‘Ò‹@ƒ‚[ƒVƒ‡ƒ“ƒJƒEƒ“ƒ^[

	DirectX::SimpleMath::Vector3 m_idle_start_pos;	//‘Ò‹@ƒ‚[ƒVƒ‡ƒ“ŠJn’n“_
	DirectX::SimpleMath::Vector3 m_idle_end_pos;	//‘Ò‹@ƒ‚[ƒVƒ‡ƒ“I—¹’n“_

	float m_move_timer;								//ˆÚ“®ƒ^ƒCƒ}[

	DirectX::SimpleMath::Vector3 m_move_start_pos;	//ˆÚ“®ƒ‚[ƒVƒ‡ƒ“ŠJn’n“_
	DirectX::SimpleMath::Vector3 m_move_end_pos;	//ˆÚ“®ƒ‚[ƒVƒ‡ƒ“I—¹’n“_

	float m_shake_angle;							//€–S‰‰o—p•Ï”
};

//ƒ{ƒX‚Ìó‘Ô‘JˆÚ
class BOSSState
{
public:
	virtual ~BOSSState() {};

	virtual void Initialize(BOSS& boss) = 0;

	virtual BOSSState* Update(BOSS& boss) { return nullptr; }
};

//‘Ò‹@
class IdleState : public BOSSState
{
public:
	IdleState();

	void Initialize(BOSS& boss)override;

	BOSSState* Update(BOSS& boss)override;
};

//ˆÚ“®
class MoveState : public BOSSState
{
public:
	MoveState();

	void Initialize(BOSS& boss)override;

	BOSSState* Update(BOSS& boss)override;
};

//UŒ‚1
class Attack1State : public BOSSState
{
public:
	Attack1State();

	void Initialize(BOSS& boss)override;

	BOSSState* Update(BOSS& boss)override;
};

//UŒ‚2
class Attack2State : public BOSSState
{
public:
	Attack2State();

	void Initialize(BOSS& boss)override;

	BOSSState* Update(BOSS& boss)override;
};

//€–S
class DeathState : public BOSSState
{
public:
	DeathState();

	void Initialize(BOSS& boss)override;

	BOSSState* Update(BOSS& boss)override;
};