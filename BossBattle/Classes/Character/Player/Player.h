//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:�v���C���[
//
//day:2017/06/30
//
//acotr:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
#pragma once
#include "..\Character.h"
#include "..\..\Weapon\Weapon.h"

class PlayerState;

class Player : public Character
{
public:
	/// <summary>
	/// �ړ���
	/// </summary>
	struct Horizontal
	{
		float x;
		float z;
	};

	static const float WALK_SPEED;			//�������x
	static const float RUN_SPEED;			//���鑬�x
	static const float ATTACK_SPEED;		//�U�����̑��x
	static const float EVADE_SPEED;			//������̑��x
	static const float GRAVITY_ACC;			//�d�͉����x
	static const float JUMP_SPEED_FIRST;	//�W�����v�����x
	static const float JUMP_SPEED_MAX;		//�ő�W�����v���x
	static const float DAMAGE_EFFECT_TIMER;	//�_���[�W�G�t�F�N�g�̃^�C�}�[
	static const int   MAX_HP;				//�ő�̗�

public:
	Player();
	virtual ~Player();

	//������
	void Initialize()override {};
	//�X�V
	void Update()override;
	//�`��
	void Render()override;
	//�ړ�
	void Move(DirectX::SimpleMath::Vector3 movev)override;

	/// <summary>
	/// �Z�b�^�[
	/// </summary>
	//�J�����p�x�̐ݒ�
	void SetCameraRotate(const DirectX::SimpleMath::Vector3& rotate) { m_camera_rotate = rotate; }
	//����̐ݒ�
	void SetWeapon(Weapon* weapon);
	//���ʈړ��ʂ̐ݒ�
	void SetHorizontal(float horizontalX,float horizontalZ)
	{
		m_horizontal.x = horizontalX;
		m_horizontal.z = horizontalZ;
	}
	//�G�ɗ^����_���[�W�̐ݒ�
	void SetDamage(int damage) { m_damage = damage; }
	//�㏸�ʂ̐ݒ�
	void SetVelocity(float velocity) { m_velocity.y = velocity; }
	//��U�����͏�Ԃ̐ݒ�
	void SetLightAttackInputState(bool state) { m_is_lightattack = state; }
	//�����͏�Ԃ̐ݒ�
	void SetEvadeInputState(bool is_evade) { m_is_evade = is_evade; }

	/// <summary>
	/// �Q�b�^�[
	/// </summary>
	//�J�����p�x�̎擾
	const DirectX::SimpleMath::Vector3& GetCameraRotate() { return m_camera_rotate; }
	//�ړ��ʂ̎擾
	const Horizontal GetHorizontal() { return m_horizontal; }
	//���x�̎擾
	const DirectX::SimpleMath::Vector3& GetVelocity() { return m_velocity; }
	//������̎擾
	const Weapon* GetWeaponInfo() { return m_weapon; }
	//�W�����v��Ԃ̎擾
	const bool GetJumpState() { return m_is_jump; }
	//������͏�Ԃ̎擾
	const bool GetRunState() { return m_is_run; }
	//��U�����͏�Ԃ̎擾
	const bool GetLightAttackInputState() { return m_is_lightattack; }
	//�����Ԃ̎擾
	const bool GetEvadeInputState() { return m_is_evade; }
	//�ړ���Ԃ̎擾
	bool IsMove();


	//��]
	void Rotation(const DirectX::SimpleMath::Vector3& anglev);
	//�J������]
	void CameraRotatin(const DirectX::SimpleMath::Vector3& rotate);

	//�W�����v�J�n
	void StartJump();
	//�W�����v�I��
	void StopJump();
	//�����J�n
	void StartFall();
	//����
	void Run(bool is_run);

	//�_���[�W�G�t�F�N�gON
	void DamageEffect() { m_status_canvas.SetVisible("damage_effect", true); }

	//�U���q�b�g�G�t�F�N�g����
	void HitEffect(const DirectX::SimpleMath::Vector3& pos);
	//���ՃG�t�F�N�g����
	void Footsteps();
private:
	PlayerState* m_state;					        //�v���C���[�̏��

	Weapon* m_weapon;						        //����

	DirectX::SimpleMath::Vector3 m_camera_rotate;	//�J�����p�x

	Horizontal m_horizontal;				        //�ړ���

	bool m_is_jump;							        //�W�����v�t���O

	bool m_is_run;							        //����t���O

	DirectX::SimpleMath::Vector3 m_velocity;        //�㉺�̈ړ���

	bool m_is_lightattack;					        //��U���J�n

	bool m_is_heavyattack;					        //���U���J�n

	bool m_is_evade;						        //����J�n

	float m_damage_effect_timer;			        //�_���[�W�G�t�F�N�g�̕\���^�C�}�[
};


//�v���C���[�̏�ԑJ��
#pragma region State
class PlayerState
{
public:
	virtual ~PlayerState() {}

	virtual void Initialize(Player& player) = 0;

	virtual PlayerState* Update(Player& player) { return nullptr; }
};

//�������
class StandingState : public PlayerState
{
public:
	StandingState() {}

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};

//�������
class WalkingState : public PlayerState
{
public:
	WalkingState() { m_timer = 0; }

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
private:
	float m_timer;		//�����p�^�C�}�[
};

//������
class RunningState : public PlayerState
{
public:
	RunningState() { m_timer = 0; }

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
private:
	float m_timer;		//�����p�^�C�}�[
};

//�W�����v���
class JumpingState : public PlayerState
{
public:
	JumpingState() {}

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};

//��U��1
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

//��U��2
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

//��U��3
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

//�W�����v�U��1
class JumpAttack1State : public PlayerState
{
public:
	JumpAttack1State() {}

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};

//�W�����v�U��2
class JumpAttack2State : public PlayerState
{
public:
	JumpAttack2State() {}

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};

//�W�����v�U��3
class JumpAttack3State : public PlayerState
{
public:
	JumpAttack3State() {}

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};

//����������
class PushInState : public PlayerState
{
public:
	PushInState() {}

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};

//���
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

//���S
class DeadState : public PlayerState
{
public:
	DeadState() {}

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};

#pragma endregion