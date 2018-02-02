//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:�R�}���h�N���X
//
//day:2017/07/13
//
//acotr:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
#pragma once
#include "..\Character\Player\Player.h"

class Command
{
public:
    virtual ~Command() {};
    virtual void execute(Player* player) = 0;
};

/// <summary>
/// ��U���R�}���h
/// </summary>
class LightAttackCommand : public Command
{
public:
    void execute(Player* player)override { player->SetLightAttackInputState(true); };
};

/// <summary>
/// �W�����v�R�}���h
/// </summary>
class JumpCommand : public Command
{
public:
    void execute(Player* player)override { player->StartJump(); };
};

/// <summary>
/// ����R�}���h
/// </summary>
class EvadeCommand : public Command
{
public:
    void execute(Player* player)override { player->SetEvadeInputState(true); }
};

/// <summary>
/// ����R�}���h
/// </summary>
class RunCommand : public Command
{
public:
    void execute(Player* player)override { player->Run(true); }
};

/// <summary>
/// �R�}���h����
/// </summary>
class NonCommand : public Command
{
public:
    void execute(Player* player)override {};
};
