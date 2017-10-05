//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:�v���C���[
//
//day:2017/06/30
//
//acotr:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
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

	static const float TURN_SPEED;			//��]���x
	static const float STEP_SPEED;			//�X�e�b�v���x
	static const float WALK_SPEED;			//�������x
	static const float RUN_SPEED;			//���鑬�x
	static const float STEP_TIME;			//�X�e�b�v����
	static const float GRAVITY_ACC;			//�d�͉����x
	static const float JUMP_SPEED_FIRST;	//�W�����v�����x
	static const float JUMP_SPEED_MAX;		//�ő�W�����v���x
	static const float DAMAGE_EFFECT_TIMER;	//�_���[�W�G�t�F�N�g�̃^�C�}�[

public:
	Player();
	virtual ~Player();

	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Render();

	//�ꏊ�̐ݒ�
	void SetPos(const DirectX::SimpleMath::Vector3& pos) { m_pos = pos; }

	//�p�x�̐ݒ�
	void SetAngle(const DirectX::SimpleMath::Vector3& angle) { m_angle = angle; }

	//����̐ݒ�
	void SetWeapon(Weapon* weapon);

	//X���ړ��ʂ̐ݒ�
	void SetHorizontal(float horizontalX,float horizontalZ)
	{
		m_horizontal.x = horizontalX;
		m_horizontal.z = horizontalZ;
	}

	//�ꏊ�̎擾
	const DirectX::SimpleMath::Vector3& GetPos() { return m_pos; }

	//�p�x�̎擾
	const DirectX::SimpleMath::Vector3& GetAngle() { return m_angle; }

	//�ړ��ʂ̎擾
	const Horizontal GetHorizontal() { return m_horizontal; }

	//�ړ����x�̎擾
	const float GetMoveSpeed() { return m_move_speed; }

	//�����蔻��̎擾
	const SphereNode& GetCollisionNodeBody() { return m_player_collision; }

	//���x�̎擾
	const DirectX::SimpleMath::Vector3& GetVelocity() { return m_velocity; }

	//�X�e�b�v
	bool Step(Horizontal horizontal);

	//�ړ�
	void Move(DirectX::SimpleMath::Vector3 movev);

	//��]
	void Rotation(const DirectX::SimpleMath::Vector3& anglev);

	//�W�����v�J�n
	void StartJump();

	//�W�����v�I��
	void StopJump();

	//�����J�n
	void StartFall();

	//�W�����v��Ԃ̎擾
	bool IsJump() { return m_is_jump; }

	//����
	void Run(bool is_run);

	//��U���J�n
	void StartLightAttack();

	//���U���J�n
	void StartHeavyAttack();
private:
	PlayerState* m_state;					//�v���C���[�̏��

	int m_max_hp;							//�ő�̗�
	int m_hp;								//���݂̗̑�

	Weapon* m_weapon;						//����

	DirectX::SimpleMath::Vector3 m_pos;		//���W

	DirectX::SimpleMath::Vector3 m_angle;	//�p�x

	Horizontal m_horizontal;				//�ړ���

	float m_step_timer;						//�X�e�b�v�^�C�}�[

	bool m_is_step;							//�X�e�b�v�t���O

	float m_step_height;					//�X�e�b�v�̍���

	float m_step_ammo;						//�X�e�b�v�㏸��

	bool m_is_jump;							//�W�����v�t���O

	bool m_is_run;							//����t���O

	float m_move_speed;						//�ړ����x

	DirectX::SimpleMath::Vector3 m_velocity;//�㉺�̈ړ���

	bool m_is_lightattack;					//��U����

	bool m_is_heavyattack;					//���U����

	Obj3d m_tmp_player;						//�v���C���[�̉��z�I�u�W�F�N�g

	SphereNode m_player_collision;			//�v���C���[�̓����蔻��

	Canvas m_status_canvas;					//�X�e�[�^�X�L�����o�X

	float m_damage_effect_timer;			//�_���[�W�G�t�F�N�g�̕\���^�C�}�[
};


//�v���C���[�̏�ԑJ��
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
	StandingState();

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};

//�������
class WalkingState : public PlayerState
{
public:
	WalkingState();

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};

//�W�����v���
class JumpingState : public PlayerState
{
public:
	JumpingState();

	void Initialize(Player& player)override;

	PlayerState* Update(Player& player)override;
};