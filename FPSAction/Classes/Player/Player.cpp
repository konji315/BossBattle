#include "Player.h"
#include "../../DXTKResouces.h"
#include "../Weapon/Wand.h"
#include "../Collider/CollisionChecker.h"
#include "../Manager/AudioManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

const float Player::TURN_SPEED = 3.0f;
const float Player::STEP_SPEED = 0.05f;
const float Player::WALK_SPEED = 0.5f;
const float Player::RUN_SPEED = 0.8f;
const float Player::STEP_TIME = 1.0f;
const float Player::GRAVITY_ACC = 0.03f;
const float Player::JUMP_SPEED_FIRST = 0.5f;
const float Player::JUMP_SPEED_MAX = 3.0f;
const float Player::DAMAGE_EFFECT_TIMER = 0.3f;

/// <summary>
/// �R���X�g���N�^
/// </summary>
Player::Player()
{
	AudioManager& am = singleton<AudioManager>::GetInstance();
	EffectManager& efk = singleton<EffectManager>::GetInstance();
	DXTK::DXTKResources& dxtk = singleton<DXTK::DXTKResources>::GetInstance();
	CollisionChecker& cc = singleton<CollisionChecker>::GetInstance();

	m_pos = Vector3(0,0,0);
	m_angle = Vector3::Zero;

	m_max_hp = 50;
	m_hp = m_max_hp;
	m_horizontal.x = 0;
	m_horizontal.z = 0;
	m_step_timer = 0;
	m_is_step = false;
	m_step_height = 0;
	m_is_jump = false;
	m_is_run = false;
	m_move_speed = 0;
	m_damage_effect_timer = DAMAGE_EFFECT_TIMER;

	m_is_lightattack = false;
	m_is_heavyattack = false;

	m_velocity = Vector3();

	m_state = new StandingState();
	m_state->Initialize(*this);

	m_weapon = new Wand();

	m_player_collision.Initialize();
	m_player_collision.SetParent(&m_tmp_player);
	m_player_collision.SetOffset(Vector3(0, 3.0f, 0));
	m_player_collision.SetLocalRadius(3.0f);
	m_player_collision.Update();

	cc.RegisterSphereCollider(CollisionChecker::PLAYER, &m_player_collision);

	m_status_canvas.SetTexture("Weapon", L"Resources/Sprites/wandmark.png");
	m_status_canvas.SetTexturePos("Weapon", Vector2(80,80));
	m_status_canvas.SetTexture("HP", L"Resources/Sprites/player_hp.png");
	m_status_canvas.SetTexturePos("HP", Vector2(280, 60));

	m_status_canvas.SetTexture("HP_bar", L"Resources/Sprites/player_hp_bar.png");
	m_status_canvas.SetTexturePos("HP_bar", Vector2(280, 60));

	m_status_canvas.SetTexture("damage_effect", L"Resources/Sprites/damage_effect.png");
	m_status_canvas.SetTexturePos("damage_effect", Vector2(512, 384));
	m_status_canvas.SetVisible("damage_effect", false);

	Initialize();
}

Player::~Player()
{
	//����폜
	if (m_weapon != nullptr)
		delete m_weapon;
	//�X�e�[�g�폜
	if (m_state != nullptr)
		delete m_state;
}

/// <summary>
/// �v���C���[�̏�����
/// </summary>
void Player::Initialize()
{
}

/// <summary>
/// �v���C���[�̍X�V
/// </summary>
void Player::Update()
{
	AudioManager& am = singleton<AudioManager>::GetInstance();
	CollisionChecker& cc = singleton<CollisionChecker>::GetInstance();

	PlayerState* state = m_state->Update(*this);

	if (state != nullptr)
	{
		delete m_state;
		m_state = state;
		m_state->Initialize(*this);
	}

	//�W�����v��
	if (m_is_jump)
	{
		m_velocity.y -= GRAVITY_ACC;
		if (m_velocity.y < -JUMP_SPEED_MAX)
			m_velocity.y = -JUMP_SPEED_MAX;
	}

	m_pos += m_velocity;
	
	//��U����
	if (m_is_lightattack)
	{
		if (m_weapon->LightAttack())
		{
			m_is_lightattack = false;
		}
	}

	//���U����
	if (m_is_heavyattack)
	{
		m_weapon->HeavyAttack();
	}

	m_tmp_player.SetPos(m_pos);
	m_tmp_player.SetAngle(m_angle);

	m_tmp_player.Update();

	m_weapon->SetPlayerPos(m_pos);
	m_weapon->SetPlayerAngle(m_angle);
	m_weapon->Update();

	m_player_collision.Update();

	//�G�̍U���ɓ���������
	if (cc.CheckSphereCollider(CollisionChecker::TAG::BOSS_BALL, m_player_collision))
	{
		am.PlaySounds(CRI_PLAYSOUNDS_PLAYER_HIT);

		m_status_canvas.SetVisible("damage_effect", true);

		m_hp -= 10;
	}

	//�_���[�W�G�t�F�N�g�̔�\��
	if (m_status_canvas.GetVisible("damage_effect"))
	{
		m_damage_effect_timer -= 1 / 60.0f;
		if (m_damage_effect_timer < 0)
		{
			m_status_canvas.SetVisible("damage_effect", false);
			m_damage_effect_timer = DAMAGE_EFFECT_TIMER;
		}
	}

	float width = m_status_canvas.GetDesc("HP_bar").Width;
	RECT rect = m_status_canvas.GetRect("HP_bar");
	rect.right = width * m_hp / m_max_hp;

	m_status_canvas.SetRect("HP_bar", rect);
}

/// <summary>
/// �v���C���[�̕`��
/// </summary>
void Player::Render()
{
	m_weapon->Render();
	m_player_collision.Render();

	m_status_canvas.Render();
}

/// <summary>
/// ����̃Z�b�g
/// </summary>
/// <param name="weapon">����</param>
void Player::SetWeapon(Weapon * weapon)
{
	if (m_weapon != nullptr)
	{
		delete m_weapon;
		m_weapon = nullptr;
	}

	m_weapon = weapon;
}

/// <summary>
/// �v���C���[�̃X�e�b�v
/// </summary>
bool Player::Step(Player::Horizontal horizontal)
{
	//�X�e�b�v���ł͂Ȃ��ꍇ�X�e�b�v�J�n
	if (!m_is_step)
	{
		m_step_timer = STEP_TIME;
		m_is_step = true;
	}

	//�X�e�b�v��
	if(m_is_step)
	{
		if (m_step_timer >= 0.0f)
		{
			m_step_timer -= STEP_SPEED;

			m_step_ammo = sinf(XM_PI * (m_step_timer)) * 0.2f - m_step_height;

			//���͕����ւ̈ړ��ʂ̐���
			Vector3 moveV(horizontal.x * m_move_speed, m_step_ammo, -horizontal.z * m_move_speed);

			Move(moveV);

			m_step_height += m_step_ammo;
		}
		else
		{
			m_is_step = false;
			return true;
		}
	}

	return false;
}

/// <summary>
/// 
/// �v���C���[�̈ړ�
/// </summary>
/// <param name="movev"></param>
void Player::Move(DirectX::SimpleMath::Vector3 movev)
{
	Matrix transmat = Matrix::CreateTranslation(m_pos);
	Matrix rotmatY = Matrix::CreateRotationY(m_angle.y);

	Matrix world = rotmatY * transmat;

	movev = SimpleMath::Vector3::TransformNormal(movev, world);

	//�v���C���[�̍��W���ړ�
	SetPos(m_pos + movev);
}

/// <summary>
/// �v���C���[�̉�]
/// </summary>
/// <param name="anglev"></param>
void Player::Rotation(const DirectX::SimpleMath::Vector3 & anglev)
{
	SetAngle(m_angle + anglev);
}

/// <summary>
/// �W�����v�X�^�[�g
/// </summary>
void Player::StartJump()
{
	if (!m_is_jump)
	{
		m_velocity.y = JUMP_SPEED_FIRST;
		m_is_jump = true;
	}
}

/// <summary>
/// �W�����v�I��
/// </summary>
void Player::StopJump()
{
	m_is_jump = false;
	m_velocity = Vector3::Zero;
}

/// <summary>
/// �����J�n
/// </summary>
void Player::StartFall()
{
	if (!m_is_jump && !m_is_step)
	{
		m_is_jump = true;
		m_velocity = Vector3::Zero;
	}
}

/// <summary>
/// ����
/// </summary>
/// <param name="is_run">���</param>
void Player::Run(bool is_run)
{
	m_is_run = is_run;
	if (m_is_run)
		m_move_speed = RUN_SPEED;
	else
		m_move_speed = WALK_SPEED;
}

/// <summary>
/// ����ɂ���U���J�n
/// </summary>
void Player::StartLightAttack()
{
	m_is_lightattack = true;
}

/// <summary>
/// ����ɂ�鋭�U���J�n
/// </summary>
void Player::StartHeavyAttack()
{
	m_is_heavyattack = true;
}

/// <summary>
/// ������Ԃ̃R���X�g���N�^
/// </summary>
StandingState::StandingState()
{

}

/// <summary>
/// ������Ԃ̏�����
/// </summary>
/// <param name="player">�v���C���[</param>
void StandingState::Initialize(Player & player)
{

}

/// <summary>
/// ������Ԃ̍X�V
/// </summary>
/// <param name="player">�v���C���[</param>
/// <returns>���̃X�e�[�g</returns>
PlayerState * StandingState::Update(Player & player)
{
	//�W�����v�{�^���������ꂽ��W�����v
	if (player.IsJump())
		return new JumpingState();

	//�ړ��J�n
	if (player.GetHorizontal().x || player.GetHorizontal().z)
	{
		return new WalkingState();
	}

	return nullptr;
}

/// <summary>
/// ������Ԃ̃R���X�g���N�^
/// </summary>
WalkingState::WalkingState()
{

}

/// <summary>
/// ������Ԃ̏�����
/// </summary>
/// <param name="player">�v���C���[</param>
void WalkingState::Initialize(Player & player)
{
	//player.Initialize();
}

/// <summary>
/// ������Ԃ̍X�V
/// </summary>
/// <param name="player">�v���C���[</param>
/// <returns>���̃X�e�[�g</returns>
PlayerState * WalkingState::Update(Player & player)
{
	//���͕����ɃX�e�b�v

	//�W�����v�{�^���������ꂽ��W�����v
	if (player.IsJump())
		return new JumpingState();

	//�X�e�b�v�I����
	if (player.Step(player.GetHorizontal()))
	{
		return new StandingState();
	}

	return nullptr;
}

/// <summary>
/// �W�����v��Ԃ̃R���X�g���N�^
/// </summary>
JumpingState::JumpingState()
{
}

/// <summary>
/// �W�����v��Ԃ̏�����
/// </summary>
/// <param name="player">�v���C���[</param>
void JumpingState::Initialize(Player & player)
{
}

/// <summary>
/// �W�����v��Ԃ̍X�V
/// </summary>
/// <param name="player">�v���C���[</param>
/// <returns>���̃X�e�[�g</returns>
PlayerState * JumpingState::Update(Player & player)
{
	//���͕����ւ̈ړ��ʂ̐���
	Vector3 moveV(player.GetHorizontal().x * player.GetMoveSpeed(), 0.0f, -player.GetHorizontal().z * player.GetMoveSpeed());

	//�󒆈ړ�
	player.Move(moveV);

	if (!player.IsJump())
		return new StandingState();

	return nullptr;
}
