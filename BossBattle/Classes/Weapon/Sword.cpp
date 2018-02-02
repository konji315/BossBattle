#include "Sword.h"
#include "..\Utility\Singleton.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

const int Sword::LIGHT_ATTACK1_DAMAGE = 3;      //��U��1�̃_���[�W
const int Sword::LIGHT_ATTACK2_DAMAGE = 5;      //��U��2�̃_���[�W
const int Sword::LIGHT_ATTACK3_DAMAGE = 7;      //��U��3�̃_���[�W

const int Sword::JUMP_ATTACK1_DAMAGE = 5;       //�W�����v�U��1�̃_���[�W
const int Sword::JUMP_ATTACK2_DAMAGE = 8;       //�W�����v�U��2�̃_���[�W
const int Sword::JUMP_ATTACK3_DAMAGE = 15;      //�W�����v�U��3�̃_���[�W

/// <summary>
/// ���̃R���X�g���N�^
/// </summary>
Sword::Sword()
{
    m_pos = Vector3(0, 1.0f, 0);
    m_angle = Vector3(XMConvertToRadians(160), 0, XMConvertToRadians(90));

    /// <summary>
    /// �^�_���[�W�̐ݒ�
    /// </summary>
    m_lightattack1_damage = LIGHT_ATTACK1_DAMAGE;
    m_lightattack2_damage = LIGHT_ATTACK2_DAMAGE;
    m_lightattack3_damage = LIGHT_ATTACK3_DAMAGE;
    m_jumpattack1_damage = JUMP_ATTACK1_DAMAGE;
    m_jumpattack2_damage = JUMP_ATTACK2_DAMAGE;
    m_jumpattack3_damage = JUMP_ATTACK3_DAMAGE;

    //���f���̐ݒ�
    m_model_weapon.LoadModel(L"Resources/Sword.cmo");
    m_model_weapon.SetPos(m_pos);
    m_model_weapon.SetAngle(m_angle);
    m_model_weapon.SetParent(&m_tmp_player);

    //�����蔻��̐ݒ�
    m_sword_collision.Initialize();
    m_sword_collision.SetParent(&m_model_weapon);
    m_sword_collision.SetOffset(Vector3(0, 4.0f, 0));
    m_sword_collision.SetLocalRadius(3.0f);
    m_sword_collision.Update();
}

/// <summary>
/// ���̍X�V
/// </summary>
void Sword::Update()
{
    //�v���C���[���̍X�V
    m_tmp_player.SetPos(m_player_pos);
    m_tmp_player.SetAngle(m_player_angle);
    m_tmp_player.Update();

    //�����f���̍X�V
    m_model_weapon.SetPos(m_pos);
    m_model_weapon.SetAngle(m_angle);
    m_model_weapon.SetBoneWorld(m_bone_info);
    m_model_weapon.Update();

    //�Փ˔���̍X�V
    m_sword_collision.Update();
}

/// <summary>
/// ���̕`��
/// </summary>
void Sword::Render()
{
    //����̍X�V
    m_model_weapon.Render();

    //�����蔻��̕`��
    m_sword_collision.Render();
}