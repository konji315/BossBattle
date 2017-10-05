//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:�{�X�N���X
//
//day:2017/07/22
//
//acotr:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
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

	BOSSState* m_state;								//�{�X�̏�ԑJ��

	float m_idle_timer;								//�ҋ@�^�C�}�[
	int m_idle_count;								//�ҋ@���[�V�����J�E���^�[

	DirectX::SimpleMath::Vector3 m_idle_start_pos;	//�ҋ@���[�V�����J�n�n�_
	DirectX::SimpleMath::Vector3 m_idle_end_pos;	//�ҋ@���[�V�����I���n�_

	float m_move_timer;								//�ړ��^�C�}�[

	DirectX::SimpleMath::Vector3 m_move_start_pos;	//�ړ����[�V�����J�n�n�_
	DirectX::SimpleMath::Vector3 m_move_end_pos;	//�ړ����[�V�����I���n�_

	float m_shake_angle;							//���S���o�p�ϐ�
};

//�{�X�̏�ԑJ��
class BOSSState
{
public:
	virtual ~BOSSState() {};

	virtual void Initialize(BOSS& boss) = 0;

	virtual BOSSState* Update(BOSS& boss) { return nullptr; }
};

//�ҋ@
class IdleState : public BOSSState
{
public:
	IdleState();

	void Initialize(BOSS& boss)override;

	BOSSState* Update(BOSS& boss)override;
};

//�ړ�
class MoveState : public BOSSState
{
public:
	MoveState();

	void Initialize(BOSS& boss)override;

	BOSSState* Update(BOSS& boss)override;
};

//�U��1
class Attack1State : public BOSSState
{
public:
	Attack1State();

	void Initialize(BOSS& boss)override;

	BOSSState* Update(BOSS& boss)override;
};

//�U��2
class Attack2State : public BOSSState
{
public:
	Attack2State();

	void Initialize(BOSS& boss)override;

	BOSSState* Update(BOSS& boss)override;
};

//���S
class DeathState : public BOSSState
{
public:
	DeathState();

	void Initialize(BOSS& boss)override;

	BOSSState* Update(BOSS& boss)override;
};