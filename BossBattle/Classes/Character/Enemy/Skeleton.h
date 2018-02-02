//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:�X�P���g���N���X
//
//day:2017/11/25
//
//acotr:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
#pragma once
#include "Enemy.h"
#include "..\..\Camera\BossCamera.h"

class Skeleton : public Enemy
{
public:
    static const int MAX_HP;		    //�ő�̗�
    static const int MAX_STUN;		    //�ő�X�^���l
    static const int ATTACK_PATTERN;    //�U���p�^�[����
    static const int DAMAGE;		    //�v���C���[�ɗ^����_���[�W

    enum HAND			//��
    {
        RIGHT_TOP,		//�E��̎�
        RIGHT_BOTTOM,	//�E���̎�
        LEFT_TOP,		//����̎�
        LEFT_BOTTOM,	//�����̎�
        HAND_NUM
    };

    enum HORN			//�p
    {
        RIGHT,			//�E�p
        LEFT,			//���p
        HORN_NUM
    };

public:
    //�f��
    class DemoState : public EnemyState
    {
    public:
        DemoState() {}
        void Initialize(Enemy* enemy)override;
        EnemyState* Update(Enemy* enemy)override;
    };
    //�ҋ@
    class IdleState : public EnemyState
    {
    public:
        IdleState() {}
        void Initialize(Enemy* enemy)override;
        EnemyState* Update(Enemy* enemy)override;
    };

    //�U��1
    class Attack1State : public EnemyState
    {
    public:
        Attack1State() { m_timer = 0; }
        void Initialize(Enemy* enemy)override;
        EnemyState* Update(Enemy* enemy)override;
    private:
        int m_timer;		//�^�C�}�[
    };

    //�U��2
    class Attack2State : public EnemyState
    {
    public:
        Attack2State() { m_timer = 0; }
        void Initialize(Enemy* enemy)override;
        EnemyState* Update(Enemy* enemy)override;
    private:
        int m_timer;		//�^�C�}�[
    };

    //�X�^�����
    class StunState : public EnemyState
    {
    public:
        StunState() {}
        void Initialize(Enemy* enemy)override;
        EnemyState* Update(Enemy* enemy)override;
    };

    //���S
    class DeadState : public EnemyState
    {
    public:
        DeadState() { m_timer = 0; }
        void Initialize(Enemy* enemy)override;
        EnemyState* Update(Enemy* enemy)override;
    private:
        int m_timer;		//�^�C�}�[
    };
public:
    Skeleton();
    ~Skeleton();

    //������
    void Initialize()override;
    //�X�V
    void Update()override;
    //�`��
    void Render()override;

    //�ړ�
    void Move(DirectX::SimpleMath::Vector3 movev)override {}

    //�X�^���������ǂ���
    bool IsStun()override
    {
        if (m_stun >= MAX_STUN)
            return true;

        return false;
    };

    //�p�̓����蔻��̎擾
    const SphereNode& GetHornCollision(int id) { return m_horn_collision[id]; }
    //��̓����蔻��̎擾
    const SphereNode& GetHandCollision(int id) { return m_hand_collision[id]; }
private:
    SphereNode m_horn_collision[HORN_NUM];	//�p�̓����蔻��(2�{��)
    SphereNode m_hand_collision[HAND_NUM];  //��̓����蔻��(4�{��)
};