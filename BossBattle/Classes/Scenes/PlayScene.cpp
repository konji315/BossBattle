#include "PlayScene.h"
#include "WICTextureLoader.h"
#include <SimpleMath.h>
#include "..\Common\DXTKResouces.h"
#include "..\Manager\AudioManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

PlayScene::~PlayScene()
{
    //�G�̏I������
    for (auto enemy : m_enemys)
    {
        if (enemy != nullptr)
        {
            delete enemy;
            enemy = nullptr;
        }
    }
}

void PlayScene::Enter(int width, int height, Camera* camera)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();
    DXTK::DXTKResources& dxtk = singleton<DXTK::DXTKResources>::GetInstance();
    ParticleEffectManager& pem = singleton<ParticleEffectManager>::GetInstance();

    //����~
    am.StopSound();
    am.PlaySounds(CRI_PLAYSOUNDS_BATTLE3_BGM, 0.3f);

    //�J�������ݒ�
    m_window_width = width;
    m_window_height = height;
    m_camera = camera;
    m_camera->SetEyePos(Vector3(0, 12, 100));

    //�v���C���[�ʒu���ݒ�
    m_player.SetPos(Vector3(0, 30, 50));

    //��ԊO�̕�
    m_wall.LoadModel(L"Resources/Wall.cmo");
    m_wall.SetScaling(1 / 40.0f);
    m_wall.SetPos(Vector3(0, -100, 0));

    //�O���̕�
    m_collision_wall.Initialize(L"Wall", L"");
    m_collision_wall.SetScale(1 / 58.0f);
    m_collision_wall.SetTrans(Vector3(0, -100, 0));

    //�n��
    m_ground.Initialize(L"Field", L"Field");

    //�����̕�
    m_center_wall.Initialize(L"Field", L"");
    m_center_wall.SetScale(0.23f);
    m_center_wall.SetTrans(Vector3(0, 80, 0));

    //�X�P���g���ݒ�
    m_skeleton = std::make_unique<Skeleton>();
    m_skeleton->SetPlayerInfo(&m_player);
    m_skeleton->SetCamera(m_camera);

    m_alpha = 0.0f;
    m_txt_grav = 0.0f;

    //UI�ݒ�
    m_canvas.SetTexture("FadeOut", L"Resources/Sprites/fade_white.png");
    m_canvas.SetTexturePos("FadeOut", Vector2(512, 384));
    m_canvas.SetColor("FadeOut", { 1,1,1,m_alpha });
    m_canvas.SetTexture("GameOver", L"Resources/Sprites/gameover.png");
    m_canvas.SetTexturePos("GameOver", Vector2(512, -384));

    dynamic_cast<BossCamera*>(m_camera)->SetBoss(m_skeleton.get());
}

/// <summary>
/// �X�V
/// </summary>
std::unique_ptr<Scene> PlayScene::Update()
{
    AudioManager& am = singleton<AudioManager>::GetInstance();

    //�v���C���[�X�V
    m_player.SetCameraRotate(dynamic_cast<BossCamera*>(m_camera)->GetCameraAngle());
    m_player.Update();

    //�J�����X�V
    m_camera->Update();

    //����
    m_input.ButtonInput(&m_player);

    //���f���̍X�V
    m_wall.Update();

    //�O���̕ǂ̍X�V
    m_collision_wall.Update();

    //�n�ʂ̍X�V
    m_ground.Update();

    //�����̕ǂ̍X�V
    m_center_wall.Update();

    //�n�`�ւ̂߂荞�ݖh�~
    {
        //�����蔻��̎擾
        Sphere sphere = m_player.GetCollisonSphere();
        //���@�̃��[���h���W�擾
        Vector3 trans = m_player.GetPos();
        //���̒��S����ڕW�̃Z���^�[�ւ̃x�N�g��
        Vector3 sphere2Player = trans - sphere.center;

        //�߂荞�ݖh�~�x�N�g��
        Vector3 reject;

        //�n�ʂƂ̂߂荞�ݔr��
        if (m_ground.IntersectSphere(sphere, &reject))
        {
            sphere.center += reject;
        }
        //�O���̕ǂƂ̂߂荞�ݔr��
        if (m_collision_wall.IntersectSphere(sphere, &reject))
        {
            sphere.center += reject;
        }
        //�����̕ǂƂ̂߂荞�ݔr��
        if (m_center_wall.IntersectSphere(sphere, &reject))
        {
            sphere.center += reject;
        }
        m_player.SetPos(sphere.center + sphere2Player);
    }

    //�G�Ƃ̂߂荞�ݔr�ˏ���
    {
        const Sphere& player_sphere = m_player.GetCollisonSphere();

        Vector3 trans = m_player.GetPos();

        Vector3 reject;

        for (auto enemy : m_enemys)
        {
            const Sphere& enemy_sphere = enemy->GetCollisonSphere();
            //�ړ_����̕����x�N�g���擾
            if (CheckSphere2Sphere(player_sphere, enemy_sphere, &reject))
            {
                Vector3 vec = trans - reject;
                vec.Normalize();

                m_player.SetPos(trans + vec * 0.4f);
            }
        }

        //�{�X�̎�Ƃ̂߂荞�ݔr�ˏ���
        for (int i = 0; i < Skeleton::HAND_NUM; i++)
        {
            const Sphere& hand_sphere = m_skeleton->GetHandCollision(i);
            if (CheckSphere2Sphere(player_sphere, hand_sphere, &reject) && m_player.GetCollisionState())
            {
                Vector3 vec = trans - reject;
                vec.Normalize();

                m_player.SetPos(trans + vec);
            }
        }
        //�{�X�̓��Ƃ̂߂荞�ݔr�ˏ���
        for (int i = 0; i < Skeleton::HORN_NUM; i++)
        {
            const Sphere& horn_sphere = m_skeleton->GetHornCollision(i);
            if (CheckSphere2Sphere(player_sphere, horn_sphere, &reject))
            {
                Vector3 vec = trans - reject;
                vec.Normalize();

                m_player.SetPos(trans + vec);
            }
        }
    }

    //���@���n�ʂɗ�����
    if (m_player.GetVelocity().y <= 0.0f)
    {
        //���@�̓����瑫���ւ̐���
        Segment player_segment;

        Vector3 trans = m_player.GetPos();

        player_segment.start = trans + Vector3(0, -1, 0);
        player_segment.end = trans + Vector3(0, -2.5f, 0);

        //��_���W
        Vector3 inter;

        //�n�`�Ɛ����̓����蔻��
        if (m_ground.IntersectSegment(player_segment, &inter))
        {
            //Y���W�̂݌�_�ֈړ�
            trans.y = inter.y + 2.0f;

            m_player.StopJump();
        }
        else
        {
            //�����J�n
            m_player.StartFall();
        }

        m_player.SetPos(trans);

    }

    //�v���C���[����G�ւ̍U������
    {
        //�v���C���[���U�������ǂ���
        if (m_player.GetAttackState())
        {
            for (auto enemy : m_enemys)
            {
                //�G���_���[�W��t(��������)��Ԃ�
                if (enemy->GetCollisionState())
                {
                    //����ƓG�̓����蔻��
                    const Sphere& weapon_sphere = m_player.GetWeaponInfo()->GetCollisonSphere();
                    const Sphere& enemy_sphere = enemy->GetCollisonSphere();

                    Vector3 point;

                    //����ƓG������������_���[�W��^����
                    if (CheckSphere2Sphere(weapon_sphere, enemy_sphere, &point))
                        Hit(m_player, enemy, point);
                }
            }
        }
    }

    //�v���C���[����{�X�ւ̍U������
    {
        if (m_player.GetAttackState())
        {
            if (m_skeleton->GetCollisionState())
            {
                const Sphere& weapon_sphere = m_player.GetWeaponInfo()->GetCollisonSphere();
                //const Sphere* horn_sphere = m_skeleton->GetHornCollision();

                //��Ƃ̓����蔻��
                for (int i = 0; i < Skeleton::HAND_NUM; i++)
                {
                    const Sphere& hand_sphere = m_skeleton->GetHandCollision(i);
                    Vector3 point;
                    if (CheckSphere2Sphere(weapon_sphere, hand_sphere, &point))
                    {
                        //�v���C���[�̗^����_���[�W
                        int damage = m_player.GetDamage();
                        //�X�^���l�~��
                        m_skeleton->CalcStunDamage(damage);
                        //�U������������
                        m_player.SetAttackState(false);
                        m_player.HitEffect(point);

                        am.PlaySounds(CRI_PLAYSOUNDS_SLASH);
                    }
                }

                //�p�Ƃ̓����蔻��
                for (int i = 0; i < Skeleton::HORN_NUM; i++)
                {
                    const Sphere& horn_sphere = m_skeleton->GetHornCollision(i);
                    Vector3 point;
                    if (CheckSphere2Sphere(weapon_sphere, horn_sphere, &point))
                    {
                        //�v���C���[�̗^����_���[�W
                        int damage = m_player.GetDamage();
                        //�_���[�W�v�Z
                        m_skeleton->CalcDamage(damage);
                        m_skeleton->HitDamageColor(Colors::Orange);

                        //�U������������
                        m_player.SetAttackState(false);
                        m_player.HitEffect(point);

                        am.PlaySounds(CRI_PLAYSOUNDS_SLASH);
                    }
                }

            }
        }
    }

    //�G����v���C���[�ւ̍U������
    {
        //�{�X���U����
        if (m_skeleton->GetAttackState())
        {
            //�v���C���[�̔��肪����
            if (m_player.GetCollisionState())
            {
                const Sphere& player_sphere = m_player.GetCollisonSphere();

                //��Ƀq�b�g
                for (int i = 0; i < Skeleton::HAND_NUM; i++)
                {
                    const Sphere& hand_sphere = m_skeleton->GetHandCollision(i);
                    Vector3 point;
                    if (CheckSphere2Sphere(player_sphere, hand_sphere, &point))
                    {
                        //�_���[�W���󂯂�
                        Hit(m_skeleton.get(), m_player, point);
                    }
                }
            }
        }
    }

    //�v���C���[�����S������GameOver���o
    if (m_player.IsDead())
    {
        if (m_canvas.GetPos("GameOver").y <= 390)
        {
            m_txt_grav += 0.05f;
            //Game Over����ʏc�����܂ōs������
            if (m_canvas.GetPos("GameOver").y >= 384)
                m_txt_grav = -m_txt_grav + 5;

            m_canvas.SetTexturePos("GameOver", Vector2(m_canvas.GetPos("GameOver").x, m_canvas.GetPos("GameOver").y + m_txt_grav));
        }
        else
        {
            static float timer = 0;
            timer += 1 / 60.0f;
            if (timer >= 3.0f)
                return std::make_unique<PlayScene>();
        }
    }

    //�G�̍X�V
    for (auto enemy : m_enemys)
        enemy->Update();

    //�{�X�̍X�V
    if (!m_skeleton->IsDead())
        m_skeleton->Update();

    //�t�F�[�h�A�E�g
    if (!m_skeleton->ExistsHP())
    {
        if (FadeOut())
            return std::make_unique<PlayScene>();
    }

    return nullptr;
}

/// <summary>
/// �`��
/// </summary>
void PlayScene::Render()
{
    DXTK::DXTKResources& dxtk = singleton<DXTK::DXTKResources>::GetInstance();

    dxtk.m_sprite_batch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());

    //�ǂ̕`��
    m_wall.Render();
    m_collision_wall.Draw();
    m_center_wall.Draw();

    //�n��
    m_ground.Draw();

    //�v���C���[
    m_player.Render();

    //�G�`��
    for (auto enemy : m_enemys)
        enemy->Render();

    //�{�X
    m_skeleton->Render();

    //UI
    m_canvas.Render();

    dxtk.m_sprite_batch->End();
}


/// <summary>
/// �t�F�[�h�A�E�g
/// </summary>
/// <returns>�I����</returns>
bool PlayScene::FadeOut()
{
    if (m_alpha > 1.2f)
        return true;

    m_alpha += 0.003f;

    Color color = { 1,1,1,m_alpha };

    m_canvas.SetColor("FadeOut", color);

    return false;
}

/// <summary>
/// ���킪�G�ɓ�����������
/// </summary>
/// <param name="weapon">����</param>
/// <param name="enemy">�G</param>
void PlayScene::Hit(Player& player, Enemy * enemy, const DirectX::SimpleMath::Vector3& point)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();

    //�v���C���[�̗^����_���[�W
    int damage = player.GetDamage();

    //�_���[�W�v�Z&��_�����&�F�ύX
    enemy->CalcDamage(damage);
    enemy->SetCollisionState(false);
    enemy->HitDamageColor(Colors::Orange);

    //�U������������
    player.SetAttackState(false);
    player.HitEffect(point);

    am.PlaySounds(CRI_PLAYSOUNDS_SLASH);
}

/// <summary>
/// �G���v���C���[�ɓ�����������
/// </summary>
/// <param name="enemy">�G</param>
/// <param name="player">�v���C���[</param>
void PlayScene::Hit(Enemy * enemy, Player & player, const DirectX::SimpleMath::Vector3& point)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();

    //�G�̗^����_���[�W
    int damage = enemy->GetDamage();

    //�_���[�W�v�Z����_����Ԃ�
    player.CalcDamage(damage);
    player.DamageEffect();

    //�U������������
    enemy->SetAttackState(false);

    am.PlaySounds(CRI_PLAYSOUNDS_PLAYER_HIT, 0.8f);
}
