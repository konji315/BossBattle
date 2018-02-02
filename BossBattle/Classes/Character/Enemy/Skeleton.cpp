#include "Skeleton.h"
#include "..\..\Utility\Singleton.h"
#include "..\..\Manager\AudioManager.h"
#include "..\..\Manager\ParticleEffectManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

const int Skeleton::MAX_HP = 100;       //�ő�̗�
const int Skeleton::MAX_STUN = 50;      //�ő�X�^���l
const int Skeleton::ATTACK_PATTERN = 2; //�U���p�^�[��
const int Skeleton::DAMAGE = 15;        //�_���[�W�l

/// <summary>
/// �R���X�g���N�^
/// </summary>
Skeleton::Skeleton()
{
    //m_stun = Skeleton::MAX_STUN;
    m_hp = Skeleton::MAX_HP;
    m_damage = Skeleton::DAMAGE;
    m_damage_color_timer = 0;

    m_dead = false;

    //�X�P���g�����f���̓ǂݍ���
    m_model.LoadModel(L"Resources/skeleton.cmo");

    //UI�̐ݒ�
    m_status_canvas.SetTexture("Boss_stun", L"Resources/Sprites/enemy_stun_bar.png");
    m_status_canvas.SetTexturePos("Boss_stun", Vector2(778, 121));
    m_status_canvas.SetTexture("Boss_HP", L"Resources/Sprites/boss_hp_frame.png");
    m_status_canvas.SetTexturePos("Boss_HP", Vector2(512, 100));
    m_status_canvas.SetTexture("Boss_HP_bar", L"Resources/Sprites/enemy_HP_bar.png");
    m_status_canvas.SetTexturePos("Boss_HP_bar", Vector2(511, 79));

    //��̓����蔻��
    for (int i = 0; i < HAND_NUM; i++)
    {
        m_hand_collision[i].Initialize();
        m_hand_collision[i].SetParent(&m_model);
        m_hand_collision[i].SetLocalRadius(5.0f);
    }

    //�p�̓����蔻��
    for (int i = 0; i < HORN_NUM; i++)
    {
        m_horn_collision[i].Initialize();
        m_horn_collision[i].SetParent(&m_model);
        m_horn_collision[i].SetOffset(Vector3(0, 11, 0));
        m_horn_collision[i].SetLocalRadius(6.0f);
    }

    //���f���̐F�̏�����
    m_model.ResetColor();

    //������
    Initialize();

    //������Ԃ̐ݒ�
    m_enemy_state = new Skeleton::IdleState();
    m_enemy_state->Initialize(this);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Skeleton::~Skeleton()
{
    //�X�e�[�g�̍폜
    if (m_enemy_state != nullptr)
        delete m_enemy_state;
}

/// <summary>
/// ������
/// </summary>
void Skeleton::Initialize()
{
    m_anim_name = L"Idle";
    InitStun();
}

/// <summary>
/// �X�V
/// </summary>
void Skeleton::Update()
{
    //���񂾂�X�V�I��
    if (m_dead)
        return;

    //��ԑJ��
    EnemyState* state = m_enemy_state->Update(this);
    if (state != nullptr)
    {
        delete m_enemy_state;
        m_enemy_state = state;
        m_enemy_state->Initialize(this);
    }

    //�ʒu���̐ݒ�
    m_model.SetPos(m_pos);
    m_model.SetAngle(m_angle);
    m_model.Update();
    m_model.UpdateAnimation(m_anim_name);

    //��̃{�[�����Z�b�g
    m_hand_collision[HAND::RIGHT_TOP].SetBoneWorld(m_model.GetBoneWorld(22));
    m_hand_collision[HAND::RIGHT_BOTTOM].SetBoneWorld(m_model.GetBoneWorld(12));
    m_hand_collision[HAND::LEFT_TOP].SetBoneWorld(m_model.GetBoneWorld(19));
    m_hand_collision[HAND::LEFT_BOTTOM].SetBoneWorld(m_model.GetBoneWorld(15));

    //�p�̃{�[�����Z�b�g
    m_horn_collision[HORN::RIGHT].SetBoneWorld(m_model.GetBoneWorld(4));
    m_horn_collision[HORN::LEFT].SetBoneWorld(m_model.GetBoneWorld(8));

    //��̓����蔻��̍X�V
    for (int i = 0; i < HAND_NUM; i++)
    {
        m_hand_collision[i].Update();
    }
    //�p�̓����蔻��
    for (int i = 0; i < HORN_NUM; i++)
    {
        m_horn_collision[i].Update();
    }

    //�̗͂������
    if (m_hp > 0)
    {
        //�̗̓o�[�̐ݒ�
        UINT width = m_status_canvas.GetDesc("Boss_HP_bar").Width;
        RECT rect = m_status_canvas.GetRect("Boss_HP_bar");
        rect.right = width * m_hp / Skeleton::MAX_HP;

        m_status_canvas.SetRect("Boss_HP_bar", rect);

        //�X�^���o�[�̐ݒ�
        width = m_status_canvas.GetDesc("Boss_stun").Width;
        rect = m_status_canvas.GetRect("Boss_stun");
        //m_stun = min(m_stun, MAX_STUN);
        rect.right = LONG(width * (1 - (float)m_stun / Skeleton::MAX_STUN));

        m_status_canvas.SetRect("Boss_stun", rect);
    }

    //�_���[�W�F�ɂȂ����Ƃ�
    if (m_damage_color_timer != 0)
    {
        m_damage_color_timer -= 1 / 60.0f;
        if (m_damage_color_timer <= 0)
            m_model.ResetColor();
    }
}

/// <summary>
/// �`��
/// </summary>
void Skeleton::Render()
{
    m_model.Render();

    //��̓����蔻��̕`��
    for (int i = 0; i < HAND_NUM; i++)
    {
        m_hand_collision[i].Render();
    }
    //�p�̓����蔻��
    for (int i = 0; i < HORN_NUM; i++)
    {
        m_horn_collision[i].Render();
    }

    //�̗͂��s������UI��\��
    if (m_hp > 0)
    {
        //UI�̕\��
        m_status_canvas.Render();
    }
}

#pragma region SkeletonState

/// <summary>
/// �X�P���g���f����ԏ�����
/// </summary>
void Skeleton::DemoState::Initialize(Enemy * enemy)
{
    enemy->ChangeAnimation(L"Demo");
}

/// <summary>
/// �X�P���g���f����ԍX�V
/// </summary>
Skeleton::EnemyState * Skeleton::DemoState::Update(Enemy * enemy)
{
    return nullptr;
}

/// <summary>
/// �X�P���g���ҋ@��ԏ�����
/// </summary>
void Skeleton::IdleState::Initialize(Enemy* enemy)
{
    enemy->ChangeAnimation(L"Idle");    //�ҋ@�A�j���[�V�����Đ�
    enemy->SetCollisionState(false);    //�����蔻��OFF
    enemy->SetAttackState(false);       //�U������OFF
}

/// <summary>
/// �X�P���g���ҋ@��ԍX�V
/// </summary>
Skeleton::EnemyState* Skeleton::IdleState::Update(Enemy* enemy)
{
    const Player* player = enemy->GetPlayerInfo();

    //�v���C���[������Ȃ�
    if (player)
    {
        Vector3 player_pos = player->GetPos();
        //���X�Ƀv���C���[�̕����Ɍ���
        float enemy_angle = enemy->GetAngle().y;
        float radian = -atan2(player_pos.z, player_pos.x);
        float angle = GetShortAngleRad(enemy_angle, radian + XMConvertToRadians(90));
        enemy_angle += angle * 0.01f;

        enemy->SetAngle(Vector3(0, enemy_angle, 0));
    }


    //�ҋ@�I��������U��
    if (!enemy->IsAnim(L"Idle"))
    {
        EnemyState* state;

        int pattern = rand() % Skeleton::ATTACK_PATTERN;

        switch (pattern)
        {
        case 0:
            state = new Skeleton::Attack1State();
            break;
        case 1:
            state = new Skeleton::Attack2State();
            break;
        default:
            break;
        }
        return state;
    }
    return nullptr;
}

/// <summary>
/// �U��1�̏�����
/// </summary>
void Skeleton::Attack1State::Initialize(Enemy * enemy)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();
    am.PlaySounds(CRI_PLAYSOUNDS_BOSS_VOICE2);  //�{�X����2�Đ�

    enemy->ChangeAnimation(L"Attack1"); //�U��1�A�j���[�V�����Đ�
    enemy->SetCollisionState(true);		//�����蔻��ON
    enemy->SetAttackState(true);		//�U������ON
}

/// <summary>
/// �U��1�̍X�V
/// </summary>
Skeleton::EnemyState * Skeleton::Attack1State::Update(Enemy * enemy)
{
    m_timer++;

    //����U���J�n��
    if (m_timer == 558)
        enemy->SetAttackState(true);    //�����蔻��ON

    //�肪�n�ʂɂ����^�C�~���O�Œn�k
    if (m_timer == 156 || m_timer == 590)
    {
        AudioManager& am = singleton<AudioManager>::GetInstance();
        am.PlaySounds(CRI_PLAYSOUNDS_EARTHQUAKE);

        enemy->SetAttackState(false);   //�U������OFF
        enemy->GetCamera()->ShakeCamera(10000);
    }

    //�X�^���l�����܂�����X�^����Ԃ�
    if (enemy->IsStun())
        return new Skeleton::StunState();

    //�U�����I��������ҋ@��Ԃ�
    if (!enemy->IsAnim(L"Attack1"))
        return new Skeleton::IdleState();

    return nullptr;
}

/// <summary>
/// �U��2�̏�����
/// </summary>
void Skeleton::Attack2State::Initialize(Enemy * enemy)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();
    am.PlaySounds(CRI_PLAYSOUNDS_BOSS_VOICE2);  //�{�X����2�Đ�

    enemy->ChangeAnimation(L"Attack2"); //�U��2�A�j���[�V�����Đ�
    enemy->SetCollisionState(true);		//�����蔻��ON
    enemy->SetAttackState(false);       //�U������OFF
}

/// <summary>
/// �U��2�̍X�V
/// </summary>
Skeleton::EnemyState * Skeleton::Attack2State::Update(Enemy * enemy)
{
    m_timer++;
    //�ガ�����ɂ��U��
    if ((m_timer >= 162 && m_timer <= 228) || (m_timer >= 400 && m_timer <= 470))
    {
        //�U���J�n
        if (m_timer == 162 || m_timer == 400)
        {
            enemy->SetAttackState(true);
            AudioManager& am = singleton<AudioManager>::GetInstance();
            am.PlaySounds(CRI_PLAYSOUNDS_EARTHQUAKE, 0.8f);
        }
        //�U���I��
        if (m_timer == 228 || m_timer == 470)
        {
            enemy->SetAttackState(false);
        }
        enemy->GetCamera()->ShakeCamera(2000);
    }

    //�X�^���l�����܂�����X�^����Ԃ�
    if (enemy->IsStun())
        return new Skeleton::StunState();

    //�U�����I��������ҋ@��Ԃ�
    if (!enemy->IsAnim(L"Attack2"))
        return new Skeleton::IdleState();

    return nullptr;
}

/// <summary>
/// �X�P���g�����S��ԏ�����
/// </summary>
void Skeleton::DeadState::Initialize(Enemy * enemy)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();
    am.PlaySounds(CRI_PLAYSOUNDS_BOSS_VOICE1);      //�{�X����1�Đ�

    enemy->ChangeAnimation(L"Dead");    //���S�A�j���[�V�����Đ�
    enemy->SetCollisionState(false);    //�����蔻��OFF
    enemy->SetAttackState(false);       //�U������OFF
}

/// <summary>
/// �X�P���g�����S��ԍX�V
/// </summary>
Skeleton::EnemyState * Skeleton::DeadState::Update(Enemy * enemy)
{
    m_timer++;

    //�����G�t�F�N�g����
    if (m_timer % 30 == 0 && m_timer < 300)
    {
        ParticleEffectManager& pem = singleton<ParticleEffectManager>::GetInstance();

        Vector3 rnd_pos((float)(rand() % 30 - 15), (float)(rand() % 20), (float)(rand() % 30 - 15));

        pem.Play("explosion1", enemy->GetPos() + rnd_pos, 3);

    }

    //���S���[�V�������I�������玀�S
    if (!enemy->IsAnim(L"Dead"))
        enemy->Dead();

    return nullptr;
}

/// <summary>
/// �X�P���g���X�^����ԏ�����
/// </summary>
void Skeleton::StunState::Initialize(Enemy * enemy)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();
    am.PlaySounds(CRI_PLAYSOUNDS_BOSS_VOICE2);  //�{�X����2�Đ�

    enemy->ChangeAnimation(L"Stun");        //�X�^���A�j���[�V�����Đ�
    enemy->SetCollisionState(true);         //�����蔻��ON
    enemy->SetAttackState(false);           //�U������OFF
}

/// <summary>
/// �X�P���g���X�^����ԍX�V
/// </summary>
Skeleton::EnemyState * Skeleton::StunState::Update(Enemy * enemy)
{
    //�̗͂��s�����玀�S��Ԃ�
    if (!enemy->ExistsHP())
        return new Skeleton::DeadState();

    //�X�^�����[�V�������I��������ҋ@��Ԃ�
    if (!enemy->IsAnim(L"Stun"))
    {
        enemy->InitStun();	//�X�^���l������
        return new Skeleton::IdleState();
    }
    return nullptr;
}

#pragma endregion
