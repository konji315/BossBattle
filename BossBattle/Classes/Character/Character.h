//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:�L�����N�^�[�̊��N���X
//
//day:2017/07/10
//
//acotr:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/

#pragma once
#include <d3d11.h>
#include <SimpleMath.h>

#include "..\Collider\CollisionNode.h"
#include "..\Texture\Canvas.h"

class Character
{
public:
    Character() {}
    virtual ~Character() {}

    virtual void Initialize() = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;
    virtual void Move(DirectX::SimpleMath::Vector3 movev) = 0;

    //�ꏊ�̐ݒ�
    void SetPos(const DirectX::SimpleMath::Vector3& pos) { m_pos = pos; }
    //�p�x�̐ݒ�
    void SetAngle(const DirectX::SimpleMath::Vector3& angle) { m_angle = angle; }
    //�_���[�W�v�Z
    void CalcDamage(const int damage) { m_hp -= damage; }

    //�ꏊ�̎擾
    const DirectX::SimpleMath::Vector3& GetPos() const { return m_pos; }
    //�p�x�̎擾
    const DirectX::SimpleMath::Vector3& GetAngle() const { return m_angle; }
    //�ړ����x�̎擾
    const float GetMoveSpeed() const { return m_move_speed; }
    //HP�����邩�ǂ���
    bool ExistsHP()
    {
        if (m_hp > 0)
            return true;

        return false;
    }
    //�ړ������x�N�g���̎擾
    const DirectX::SimpleMath::Vector2 GetMoveVec()
    {
        DirectX::SimpleMath::Vector2 vec(0, 1);
        DirectX::SimpleMath::Vector2 move_vec;
        move_vec.x = vec.x * cosf(m_angle.y) - vec.y * sinf(m_angle.y);
        move_vec.y = vec.x * sinf(m_angle.y) + vec.y * cosf(m_angle.y);
        move_vec.Normalize();

        return move_vec;
    }
    //�Փ˔���̗L���̎擾
    const bool GetCollisionState() { return m_is_collision; }
    //�U�������Ԃ̎擾
    const bool GetAttackState() { return m_is_attack; }
    //����̎擾
    const SphereNode& GetCollisonSphere() { return m_collision_sphere; }
    //�^�_���̎擾
    int GetDamage() const { return m_damage; }

    //�A�j���[�V������Ԃ̎擾
    bool IsAnim(const std::wstring& clip_name) { return m_model.IsAnim(clip_name); }

    //���S
    void Dead() { m_dead = true; }
    //�|���ꂽ���ǂ���
    bool IsDead() { return m_dead; }

    //���[�V�����ύX
    void ChangeAnimation(wchar_t* clipname)
    {
        m_model.ResetAnimation();
        m_anim_name = clipname;
    }

    //�����蔻���Ԃ̃Z�b�g(�_���[�W���󂯂鑤)
    void SetCollisionState(bool state) { m_is_collision = state; }
    //�U������̃Z�b�g(�_���[�W��^���鑤)
    void SetAttackState(bool state) { m_is_attack = state; }

    //�_���[�W���̃��f���̐F�ω�
    void HitDamageColor(const DirectX::XMVECTORF32& color)
    {
        m_model.ChangeModelColor(color);	//�F��ς���

        m_damage_color_timer = 0.2f;		//�^�C�}�[�Z�b�g
    }
protected:
    Obj3d m_model;							//���f��

    DirectX::SimpleMath::Vector3 m_pos;		//���W

    DirectX::SimpleMath::Vector3 m_angle;	//�p�x

    float m_move_speed;						//�ړ����x

    int m_hp;								//�̗�
    int m_damage;							//����ɗ^����_���[�W

    SphereNode m_collision_sphere;			//�����蔻��

    Canvas m_status_canvas;					//�X�e�[�^�X�L�����o�X

    bool m_dead;							//���S�t���O

    wchar_t* m_anim_name;					//���ݍĐ����Ă���A�j���[�V������

    bool m_is_collision;					//�Փ˂��邩�ǂ���

    bool m_is_attack;						//�U�������ǂ���

    float m_damage_color_timer;				//���f���̐F�ω��^�C�}�[
};

