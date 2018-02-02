#include "Player.h"
#include "..\..\Common\DXTKResouces.h"
#include "..\..\Weapon\Sword.h"
#include "..\..\Manager\AudioManager.h"
#include "..\..\Manager\ParticleEffectManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

const float Player::WALK_SPEED = 0.3f;              //�������x
const float Player::RUN_SPEED = 0.8f;               //���鑬�x
const float Player::ATTACK_SPEED = 0.5f;            //�U�����̑��x
const float Player::EVADE_SPEED = 0.8f;             //������̑��x
const float Player::GRAVITY_ACC = 0.03f;            //�d�͉����x
const float Player::JUMP_SPEED_FIRST = 0.7f;        //�W�����v�����x
const float Player::JUMP_SPEED_MAX = 3.0f;          //�ő�W�����v���x
const float Player::DAMAGE_EFFECT_TIMER = 0.3f;     //�_���[�W�G�t�F�N�g�̃^�C�}�[
const int Player::MAX_HP = 50;                      //�ő�̗�

/// <summary>
/// �R���X�g���N�^
/// </summary>
Player::Player()
{
    DXTK::DXTKResources& dxtk = singleton<DXTK::DXTKResources>::GetInstance();

    m_hp = Player::MAX_HP;
    m_horizontal.x = 0;
    m_horizontal.z = 0;
    m_is_jump = false;
    m_is_run = false;
    m_move_speed = 0;
    m_damage_effect_timer = DAMAGE_EFFECT_TIMER;

    m_is_lightattack = false;
    m_is_heavyattack = false;
    m_is_evade = false;

    m_dead = false;

    //�v���C���[���f���̓ǂݍ���
    m_model.LoadModel(L"Resources/player.cmo");

    //�����蔻��̐ݒ�
    m_collision_sphere.Initialize();
    m_collision_sphere.SetParent(&m_model);
    m_collision_sphere.SetOffset(Vector3(0, 1.0f, 0));
    m_collision_sphere.SetLocalRadius(3.0f);
    m_collision_sphere.Update();

    //��Ԃ̐ݒ�
    m_state = new StandingState();
    m_state->Initialize(*this);

    //���퐶��
    m_weapon = new Sword();
    m_weapon->Initialize();

    //�����蔻��ON
    SetCollisionState(true);

    //UI�ݒ�
    m_status_canvas.SetTexture("HP", L"Resources/Sprites/player_hp.png");
    m_status_canvas.SetTexturePos("HP", Vector2(820, 650));

    m_status_canvas.SetTexture("HP_bar", L"Resources/Sprites/player_hp_bar.png");
    m_status_canvas.SetTexturePos("HP_bar", Vector2(820, 650));

    m_status_canvas.SetTexture("damage_effect", L"Resources/Sprites/damage_effect.png");
    m_status_canvas.SetTexturePos("damage_effect", Vector2(512, 384));
    m_status_canvas.SetVisible("damage_effect", false);
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
/// �v���C���[�̍X�V
/// </summary>
void Player::Update()
{
    PlayerState* state = m_state->Update(*this);

    if (state != nullptr)
    {
        delete m_state;

        //���S��
        if (!ExistsHP())
            m_state = new DeadState();
        else
            m_state = state;

        m_state->Initialize(*this);
    }

    //�W�����v��
    if (m_is_jump || ExistsHP())
    {
        m_velocity.y -= GRAVITY_ACC;
        if (m_velocity.y < -JUMP_SPEED_MAX)
            m_velocity.y = -JUMP_SPEED_MAX;
    }

    //�����Ă���Ԃ͍��W�X�V
    if (!IsDead())
    {
        //�d��
        m_pos += m_velocity;

        //���f���̍X�V
        m_model.SetPos(m_pos);
        m_model.SetAngle(m_angle);
        m_model.UpdateAnimation(m_anim_name);
        m_model.Update();

        //����̍X�V
        m_weapon->SetPlayerPos(m_pos);
        m_weapon->SetPlayerAngle(m_angle);
        m_weapon->SetBoneWorld(m_model.GetBoneWorld(7));
        m_weapon->Update();

        //�����蔻��̍X�V
        m_collision_sphere.Update();
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

    if (m_hp <= 0)
        m_hp = 0;

    if (m_hp >= 0)
    {
        UINT width = m_status_canvas.GetDesc("HP_bar").Width;
        RECT rect = m_status_canvas.GetRect("HP_bar");
        rect.right = width * m_hp / Player::MAX_HP;
        m_status_canvas.SetRect("HP_bar", rect);

        m_hp = min(m_hp, MAX_HP);
    }
}

/// <summary>
/// �v���C���[�̕`��
/// </summary>
void Player::Render()
{
    //����̕`��
    m_weapon->Render();
    m_collision_sphere.Render();

    //�v���C���[���f���̕`��
    m_model.Render();

    //UI�̕`��
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
/// �ړ������ǂ���
/// </summary>
/// <returns>�ړ�����</returns>
bool Player::IsMove()
{
    if (std::abs(m_horizontal.x) > 0 || std::abs(m_horizontal.z) > 0)
        return true;
    return false;
}

/// <summary>
///
/// �v���C���[�̈ړ�
/// </summary>
/// <param name="movev">�ړ�����</param>
void Player::Move(DirectX::SimpleMath::Vector3 movev)
{
    //���͂��������ꍇ�̓X�L�b�v
    if (std::abs(m_horizontal.x) <= 0.6f && std::abs(m_horizontal.z) <= 0.6f)
        return;

    Matrix transmat = Matrix::CreateTranslation(m_pos);
    Matrix rotmatY = Matrix::CreateRotationY(m_camera_rotate.y - XMConvertToRadians(-90));		//�{�X���p

    Matrix world = rotmatY * transmat;

    //�ړ������x�N�g��
    movev = SimpleMath::Vector3::TransformNormal(movev, world);

    ///���͂̊p�x�Z�o(�{�X���p)
    if (m_horizontal.x != 0 || m_horizontal.z != 0)
    {
        Vector2 y(0, 1);
        Vector2 in(m_horizontal.z, m_horizontal.x);

        float cos_sita = y.Dot(in);

        float sita = acosf(cos_sita);

        //��Βl
        if (m_horizontal.z < 0)
            sita = -sita;

        SetAngle(Vector3(0, m_camera_rotate.y + sita, 0));
    }
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

//�J�����p�x�̉�]
void Player::CameraRotatin(const DirectX::SimpleMath::Vector3 & rotate)
{
    SetCameraRotate(m_camera_rotate + rotate);
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
    if (!m_is_jump)
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
/// �q�b�g�G�t�F�N�g����
/// </summary>
void Player::HitEffect(const DirectX::SimpleMath::Vector3 & pos)
{
    ParticleEffectManager& pem = singleton<ParticleEffectManager>::GetInstance();

    pem.Play("halo", pos, 1);
    pem.Play("slash", pos, 1);
}

/// <summary>
/// ���ՃG�t�F�N�g
/// </summary>
void Player::Footsteps()
{
    ParticleEffectManager& pem = singleton<ParticleEffectManager>::GetInstance();

    //���̈ʒu�ɒ���
    pem.Play("footsteps", m_pos - Vector3(0, 2, 0), 1);
}

#pragma region PlayerState

/// <summary>
/// ������Ԃ̏�����
/// </summary>
/// <param name="player">�v���C���[</param>
void StandingState::Initialize(Player & player)
{
    player.ChangeAnimation(L"Idle");	    //�ҋ@���[�V�����Đ�
    player.SetLightAttackInputState(false);	//��U���I��
    player.SetEvadeInputState(false);		//��𔻒�OFF
    player.SetAttackState(false);		    //�U������OFF
}

/// <summary>
/// ������Ԃ̍X�V
/// </summary>
/// <param name="player">�v���C���[</param>
/// <returns>���̃X�e�[�g</returns>
PlayerState * StandingState::Update(Player & player)
{
    //�W�����v�J�n
    if (player.GetJumpState())
    {
        AudioManager& am = singleton<AudioManager>::GetInstance();
        am.PlaySounds(CRI_PLAYSOUNDS_JUMP1, 0.6f);

        player.Footsteps();
        return new JumpingState();
    }
    //�ړ��J�n
    if (player.GetHorizontal().x || player.GetHorizontal().z)
        return new WalkingState();

    //��U���J�n
    if (player.GetLightAttackInputState())
        return new LightAttack1State();

    //����J�n
    if (player.GetEvadeInputState())
        return new EvadeState();

    return nullptr;
}

/// <summary>
/// ������Ԃ̏�����
/// </summary>
/// <param name="player">�v���C���[</param>
void WalkingState::Initialize(Player & player)
{
    player.ChangeAnimation(L"Walk");	//���s�A�j���[�V�����Đ�
    player.SetAttackState(false);		//�U������OFF
}

/// <summary>
/// ������Ԃ̍X�V
/// </summary>
/// <param name="player">�v���C���[</param>
/// <returns>���̃X�e�[�g</returns>
PlayerState * WalkingState::Update(Player & player)
{
    //����
    m_timer += 1 / 60.0f;
    if (m_timer >= 0.4f)
    {
        AudioManager& am = singleton<AudioManager>::GetInstance();
        am.PlaySounds(CRI_PLAYSOUNDS_LANDING1, 0.4f);
        m_timer = 0;
    }

    //���͕����ւ̈ړ��ʂ̐���
    Vector3 moveV(player.GetHorizontal().x * player.GetMoveSpeed(), 0.0f, -player.GetHorizontal().z * player.GetMoveSpeed());
    player.Move(moveV);

    //L�������ꂽ�瑖����
    if (player.GetRunState())
        return new RunningState();

    //�W�����v�J�n
    if (player.GetJumpState())
    {
        AudioManager& am = singleton<AudioManager>::GetInstance();
        am.PlaySounds(CRI_PLAYSOUNDS_JUMP1, 0.6f);

        player.Footsteps();
        return new JumpingState();
    }
    //�ړ��I����
    if (!player.IsMove())
        return new StandingState();

    //��U���J�n
    if (player.GetLightAttackInputState())
        return new LightAttack1State();

    //����J�n
    if (player.GetEvadeInputState())
        return new EvadeState();

    return nullptr;
}

/// <summary>
/// �W�����v��Ԃ̏�����
/// </summary>
/// <param name="player">�v���C���[</param>
void JumpingState::Initialize(Player & player)
{
    player.ChangeAnimation(L"Jump");		//�W�����v�A�j���[�V�����Đ�
    player.SetEvadeInputState(false);		//����I��
    player.SetAttackState(false);			//�U������OFF
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

    //���n
    if (!player.GetJumpState())
    {
        AudioManager& am = singleton<AudioManager>::GetInstance();
        am.PlaySounds(CRI_PLAYSOUNDS_LANDING1, 0.8f);

        return new StandingState();
    }
    //�W�����v�U��
    if (player.GetLightAttackInputState())
        return new JumpAttack1State();

    //����J�n
    if (player.GetEvadeInputState())
        return new EvadeState();

    return nullptr;
}

/// <summary>
/// �����Ԃ̏�����
/// </summary>
/// <param name="player">�v���C���[</param>
void RunningState::Initialize(Player & player)
{
    player.ChangeAnimation(L"Run");     //����A�j���[�V�����Đ�
    player.SetAttackState(false);		//�U������OFF
}

/// <summary>
/// �����Ԃ̍X�V
/// </summary>
/// <param name="player">�v���C���[</param>
/// <returns>�X�e�[�g�̕ω�</returns>
PlayerState * RunningState::Update(Player & player)
{
    //����
    m_timer += 1 / 60.0f;
    if (m_timer >= 0.25f)
    {
        AudioManager& am = singleton<AudioManager>::GetInstance();
        am.PlaySounds(CRI_PLAYSOUNDS_LANDING1, 0.8f);
        player.Footsteps();
        m_timer = 0;
    }

    //���͕����ւ̈ړ��ʂ̐���
    Vector3 moveV(player.GetHorizontal().x * player.GetMoveSpeed(), 0.0f, -player.GetHorizontal().z * player.GetMoveSpeed());
    player.Move(moveV);

    //L�������ꂽ�����
    if (!player.GetRunState())
        return new WalkingState();

    //�W�����v�J�n
    if (player.GetJumpState())
    {
        AudioManager& am = singleton<AudioManager>::GetInstance();
        am.PlaySounds(CRI_PLAYSOUNDS_JUMP1, 0.6f);

        player.Footsteps();
        return new JumpingState();
    }
    //�ړ��I����
    if (!player.IsMove())
        return new StandingState();

    //��U���J�n
    if (player.GetLightAttackInputState())
        return new LightAttack1State();

    //����J�n
    if (player.GetEvadeInputState())
        return new EvadeState();

    return nullptr;
}


/// <summary>
///	��U��1��Ԃ̏�����
/// </summary>
/// <param name="player">�v���C���[</param>
void LightAttack1State::Initialize(Player & player)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();
    am.PlaySounds(CRI_PLAYSOUNDS_SWORD_GESTURE1);

    player.ChangeAnimation(L"Attack1");     //�U��1�A�j���[�V�����Đ�
    player.SetLightAttackInputState(false);	//��U���I��
    player.SetAttackState(true);			//�U������ON

    int weapon_damage = player.GetWeaponInfo()->GetLightAttack1Damage();
    player.SetDamage(weapon_damage);    	//��U��1�̃_���[�W��ݒ�

    //���_�����̎擾
    m_move_vec = player.GetMoveVec();

    m_attack_speed = Player::ATTACK_SPEED;
}

/// <summary>
/// ��U��1��Ԃ̍X�V
/// </summary>
/// <param name="player">�v���C���[</param>
PlayerState * LightAttack1State::Update(Player & player)
{
    //���_�����ւ̈ړ��ʂ̐���
    Vector3 moveV(m_move_vec.x * m_attack_speed, 0.0f, -m_move_vec.y * m_attack_speed);
    player.SetPos(player.GetPos() + moveV);

    //�u���[�L
    m_attack_speed *= 0.9f;

    //�A�j���[�V�������Ԃ��I��������ҋ@��Ԃ�
    if (!player.IsAnim(L"Attack1"))
    {
        //�R���{����
        if (player.GetLightAttackInputState())
            return new LightAttack2State();

        return new StandingState();
    }

    //�W�����v
    if (player.GetJumpState())
    {
        player.Footsteps();

        return new JumpingState();
    }
    //����J�n
    if (player.GetEvadeInputState())
        return new EvadeState();

    return nullptr;
}

/// <summary>
/// ��U��2�̏�����
/// </summary>
/// <param name="player">�v���C���[</param>
void LightAttack2State::Initialize(Player & player)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();
    am.PlaySounds(CRI_PLAYSOUNDS_SWORD_GESTURE1);

    player.ChangeAnimation(L"Attack2");     //�U��2�A�j���[�V�����Đ�
    player.SetLightAttackInputState(false); //��U���I��
    player.SetAttackState(true);		    //�U������ON

    int weapon_damage = player.GetWeaponInfo()->GetLightAttack2Damage();
    player.SetDamage(weapon_damage);	    //��U��2�̃_���[�W��ݒ�

    //���_�����̎擾
    m_move_vec = player.GetMoveVec();

    m_attack_speed = Player::ATTACK_SPEED;
}

/// <summary>
/// ��U��2�̍X�V
/// </summary>
/// <param name="player">�v���C���[</param>
PlayerState * LightAttack2State::Update(Player & player)
{
    //���_�����ւ̈ړ��ʂ̐���
    Vector3 moveV(m_move_vec.x * m_attack_speed, 0.0f, -m_move_vec.y * m_attack_speed);
    player.SetPos(player.GetPos() + moveV);

    //�u���[�L
    m_attack_speed *= 0.9f;

    //�A�j���[�V�������Ԃ��I��������ҋ@��Ԃ�
    if (!player.IsAnim(L"Attack2"))
    {
        //�R���{����
        if (player.GetLightAttackInputState())
            return new LightAttack3State();

        return new StandingState();
    }

    //�W�����v�J�n
    if (player.GetJumpState())
    {
        player.Footsteps();
        return new JumpingState();
    }
    //����J�n
    if (player.GetEvadeInputState())
        return new EvadeState();

    return nullptr;
}

/// <summary>
/// ��U��3�̏�����
/// </summary>
/// <param name="player">�v���C���[</param>
void LightAttack3State::Initialize(Player & player)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();
    am.PlaySounds(CRI_PLAYSOUNDS_SWORD_GESTURE1);

    player.ChangeAnimation(L"Attack3");     //�U��3�A�j���[�V�����Đ�
    player.SetLightAttackInputState(false); //��U���I��
    player.SetAttackState(true);		    //�U������ON

    int weapon_damage = player.GetWeaponInfo()->GetLightAttack3Damage();
    player.SetDamage(weapon_damage);	    //��U��3�̃_���[�W��ݒ�

    //���_�����̎擾
    m_move_vec = player.GetMoveVec();

    m_attack_speed = Player::ATTACK_SPEED;
}

/// <summary>
/// ��U��3�̍X�V
/// </summary>
/// <param name="player">�v���C���[</param>
PlayerState * LightAttack3State::Update(Player & player)
{
    //���_�����ւ̈ړ��ʂ̐���
    Vector3 moveV(m_move_vec.x * m_attack_speed, 0.0f, -m_move_vec.y * m_attack_speed);
    player.SetPos(player.GetPos() + moveV);

    //�u���[�L
    m_attack_speed *= 0.9f;

    //�A�j���[�V�������Ԃ��I��������ҋ@��Ԃ�
    if (!player.IsAnim(L"Attack3"))
        return new StandingState();

    //�W�����v�J�n
    if (player.GetJumpState())
    {
        player.Footsteps();
        return new JumpingState();
    }
    //����J�n
    if (player.GetEvadeInputState())
        return new EvadeState();

    return nullptr;
}

/// <summary>
/// �W�����v�U��1�̏�����
/// </summary>
void JumpAttack1State::Initialize(Player & player)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();
    am.PlaySounds(CRI_PLAYSOUNDS_SWORD_GESTURE1);

    player.ChangeAnimation(L"JumpAttack1");     //�W�����v�U��1�A�j���[�V�����Đ�
    player.SetLightAttackInputState(false);     //��U���I��
    player.SetAttackState(true);		        //�U������ON

    int weapon_damage = player.GetWeaponInfo()->GetJumpAttack1Damage();
    player.SetDamage(weapon_damage);	        //�W�����v�U��1�̃_���[�W��ݒ�

    //������Ə㏸
    player.SetVelocity(0.5f);
}

/// <summary>
/// �W�����v�U��1�̍X�V
/// </summary>
PlayerState * JumpAttack1State::Update(Player & player)
{
    //�R���{���͂����������炻�̂܂ܗ���
    if (!player.IsAnim(L"JumpAttack1"))
    {
        //�󒆃R���{����
        if (player.GetLightAttackInputState())
            return new JumpAttack2State();

        return new JumpingState();
    }
    return nullptr;
}

/// <summary>
/// �W�����v�U��2�̏�����
/// </summary>
void JumpAttack2State::Initialize(Player & player)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();
    am.PlaySounds(CRI_PLAYSOUNDS_SWORD_GESTURE1);

    player.ChangeAnimation(L"JumpAttack2");     //�W�����v�U��2�A�j���[�V�����Đ�
    player.SetLightAttackInputState(false);     //��U���I��
    player.SetAttackState(true);		        //�U������ON

    int weapon_damage = player.GetWeaponInfo()->GetJumpAttack2Damage();
    player.SetDamage(weapon_damage);	        //�W�����v�U��2�̃_���[�W��ݒ�

    //������Ə㏸
    player.SetVelocity(0.5f);
}

/// <summary>
/// �W�����v�U��2�̍X�V
/// </summary>
PlayerState * JumpAttack2State::Update(Player & player)
{

    //�R���{���͂����������炻�̂܂ܗ���
    if (!player.IsAnim(L"JumpAttack2"))
    {
        //�󒆃R���{����
        if (player.GetLightAttackInputState())
            return new JumpAttack3State();

        return new JumpingState();
    }
    return nullptr;
}

/// <summary>
/// �W�����v�U��3�̏�����
/// </summary>
void JumpAttack3State::Initialize(Player & player)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();
    am.PlaySounds(CRI_PLAYSOUNDS_SWORD_GESTURE1);

    player.ChangeAnimation(L"JumpAttack3");     //�W�����v�U��3�A�j���[�V�����Đ�
    player.SetLightAttackInputState(false);     //��U���I��
    player.SetAttackState(true);		        //�U������ON

    int weapon_damage = player.GetWeaponInfo()->GetJumpAttack3Damage();
    player.SetDamage(weapon_damage);	        //�W�����v�U��3�̃_���[�W��ݒ�

    player.SetVelocity(-0.6f);
}

/// <summary>
/// �W�����v�U��3�̍X�V
/// </summary>
PlayerState * JumpAttack3State::Update(Player & player)
{
    //���n
    if (!player.GetJumpState())
        return new PushInState();

    return nullptr;
}

/// <summary>
/// ���������ݏ�����
/// </summary>
/// <param name="player"></param>
void PushInState::Initialize(Player & player)
{
    player.ChangeAnimation(L"PushIn");
    player.SetLightAttackInputState(false);
}
PlayerState * PushInState::Update(Player & player)
{
    //���Ƃɖ߂�
    if (!player.IsAnim(L"PushIn"))
        return new StandingState();

    return nullptr;
}

/// <summary>
///	�����Ԃ̏�����
/// </summary>
void EvadeState::Initialize(Player & player)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();
    am.PlaySounds(CRI_PLAYSOUNDS_EVADE);

    player.ChangeAnimation(L"Evade");   //����A�j���[�V�����Đ�
    player.SetEvadeInputState(false);   //�����͏I��
    player.SetAttackState(false);		//�U������OFF
    player.SetCollisionState(false);	//�����蔻��OFF

    //���_�����̎擾
    m_move_vec = player.GetMoveVec();
    m_evade_speed = Player::EVADE_SPEED;
}

/// <summary>
/// �����Ԃ̍X�V
/// </summary>
PlayerState * EvadeState::Update(Player & player)
{
    Vector3 moveV(m_move_vec.x * m_evade_speed, 0.0f, -m_move_vec.y * m_evade_speed);
    player.SetPos(player.GetPos() + moveV);

    //������I�������瓖���蔻���߂��A������Ԃ�
    if (!player.IsAnim(L"Evade"))
    {
        player.SetCollisionState(true);	//�����蔻��ON

        //�W�����v
        if (player.GetJumpState())
            return new JumpingState();

        return new StandingState();
    }
    return nullptr;
}

/// <summary>
/// ���S��Ԃ̏�����
/// </summary>
void DeadState::Initialize(Player & player)
{
    player.ChangeAnimation(L"Dead");    //���S�A�j���[�V�����Đ�
    player.SetAttackState(false);		//�U������OFF
    player.SetCollisionState(false);	//�����蔻��OFF
}

/// <summary>
/// ���S��Ԃ̍X�V
/// </summary>
PlayerState * DeadState::Update(Player & player)
{
    //���S���[�V�����I��
    if (!player.IsAnim(L"Dead"))
        player.Dead();

    return nullptr;
}
#pragma endregion
