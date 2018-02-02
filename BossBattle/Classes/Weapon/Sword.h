//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:���̕���N���X
//
//day:2017/11/14
//
//actor:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/

#pragma once
#include "Weapon.h"
#include "..\Collider\CollisionNode.h"

class Sword : public Weapon
{
public:
    static const int LIGHT_ATTACK1_DAMAGE;  //��U��1�̃_���[�W
    static const int LIGHT_ATTACK2_DAMAGE;  //��U��2�̃_���[�W
    static const int LIGHT_ATTACK3_DAMAGE;  //��U��3�̃_���[�W

    static const int JUMP_ATTACK1_DAMAGE;   //�W�����v�U��1�̃_���[�W
    static const int JUMP_ATTACK2_DAMAGE;   //�W�����v�U��2�̃_���[�W
    static const int JUMP_ATTACK3_DAMAGE;   //�W�����v�U��3�̃_���[�W

public:
    Sword();
    virtual ~Sword(){}

    //������
    void Initialize()override{}
    //�X�V
    void Update()override;
    //�`��
    void Render()override;
};

