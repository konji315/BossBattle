//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:����̊��N���X
//
//day:2017/07/9
//
//acotr:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/

#pragma once
#include <d3d11.h>
#include <SimpleMath.h>

#include "..\Model\Obj3d.h"
#include "..\Collider\CollisionNode.h"

class Weapon
{
public:

    Weapon() {};
    virtual ~Weapon() {};

    //������
    virtual void Initialize() = 0;

    //�X�V
    virtual void Update() = 0;
    //�`��
    virtual void Render() = 0;

    //�v���C���[�̃Z�b�g
    virtual void SetPlayerPos(const DirectX::SimpleMath::Vector3& pos) { m_player_pos = pos; };
    virtual void SetPlayerAngle(const DirectX::SimpleMath::Vector3& angle) { m_player_angle = angle; };

    //�{�[�����̃Z�b�g
    virtual void SetBoneWorld(const DirectX::SimpleMath::Matrix& boneinfo) { m_bone_info = boneinfo; }

    /// <summary>
    /// �_���[�W�̎擾
    /// </summary>
    int GetLightAttack1Damage() const { return m_lightattack1_damage; }
    int GetLightAttack2Damage() const { return m_lightattack2_damage; }
    int GetLightAttack3Damage() const { return m_lightattack3_damage; }
    int GetJumpAttack1Damage() const { return m_jumpattack1_damage; }
    int GetJumpAttack2Damage() const { return m_jumpattack2_damage; }
    int GetJumpAttack3Damage() const { return m_jumpattack3_damage; }

    //����̎擾
    const SphereNode& GetCollisonSphere()const { return m_sword_collision; }

protected:
    Obj3d m_model_weapon;                           //����̃��f��
    Obj3d m_tmp_player;                             //�v���C���[���

    SphereNode m_sword_collision;			        //���̓����蔻��

    int m_lightattack1_damage;				        //��U��1�̗^�_���[�W
    int m_lightattack2_damage;				        //��U��2�̗^�_���[�W
    int m_lightattack3_damage;				        //��U��3�̗^�_���[�W
    int m_jumpattack1_damage;				        //�W�����v�U��1�̗^�_���[�W
    int m_jumpattack2_damage;				        //�W�����v�U��2�̗^�_���[�W
    int m_jumpattack3_damage;				        //�W�����v�U��3�̗^�_���[�W

    DirectX::SimpleMath::Matrix m_bone_info;        //�{�[�����

    DirectX::SimpleMath::Vector3 m_pos;             //�ʒu���
    DirectX::SimpleMath::Vector3 m_angle;           //�p�x

    DirectX::SimpleMath::Vector3 m_player_pos;      //�v���C���[�̈ʒu
    DirectX::SimpleMath::Vector3 m_player_angle;    //�v���C���[�̊p�x
};