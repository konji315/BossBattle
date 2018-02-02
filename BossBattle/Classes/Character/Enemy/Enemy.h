//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:�G�̊��N���X
//
//day:2017/07/10
//
//acotr:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/

#pragma once
#include "..\Character.h"
#include "..\Player\Player.h"

/// <summary>
/// �ڕW�p�x�܂ł̍����̌v�Z
/// </summary>
/// <param name="angle0">���݂̊p�x</param>
/// <param name="angle1">�ڕW�̊p�x</param>
/// <returns>�p�x�̍���</returns>
static float GetShortAngleRad(float angle0, float angle1)
{
    float angle_sub;

    angle_sub = angle1 - angle0;	// �p�x�̍�

    // ����180�x���傫�������ꍇ�A�t��]�̕����߂��̂ŁA�}�C�i�X�ɕϊ�
    // �ŏI�I��-180�`+180�x�͈̔͂ɁB
    if (angle_sub > DirectX::XM_PI)
        angle_sub -= DirectX::XM_2PI;
    else if (angle_sub < -DirectX::XM_PI)
        angle_sub += DirectX::XM_2PI;

    return angle_sub;
}

class Enemy : public Character
{
public:
    //�G�̏�ԑJ��
    class EnemyState
    {
    public:
        virtual ~EnemyState() {};
        //������
        virtual void Initialize(Enemy* enemy) = 0;
        //�X�V
        virtual EnemyState* Update(Enemy* enemy) { return nullptr; };
    };
public:
    //�R���X�g���N�^
    Enemy()
    {
        m_enemy_state = nullptr;
        m_player = nullptr;
        m_camera = nullptr;
    };
    virtual ~Enemy() {};
    //�v���C���[���̐ݒ�
    void SetPlayerInfo(Player* player) { m_player = player; }
    //�J�������̐ݒ�
    void SetCamera(Camera* camera) { m_camera = camera; }
    //�G��Ԃ̐ݒ�
    void SetState(EnemyState* state)
    {
        if (m_enemy_state != nullptr)
        {
            delete m_enemy_state;
            m_enemy_state = state;
            m_enemy_state->Initialize(this);
        }
    }

    //�X�^���l�v�Z
    void CalcStunDamage(int damage) { m_stun += damage; }
    //�X�^���l������
    void InitStun() { m_stun = 0; }

    //�v���C���[���擾
    const Player* GetPlayerInfo() { return m_player; }
    //�J�������擾
    Camera* GetCamera() { return m_camera; }
    //�X�^���l�����܂������ǂ���
    virtual bool IsStun() = 0;

protected:
    EnemyState* m_enemy_state;	//��ԑJ��
    Player*     m_player;		//�v���C���[���
    int m_stun;					//�X�^���l
    Camera* m_camera;			//�J�������
};

