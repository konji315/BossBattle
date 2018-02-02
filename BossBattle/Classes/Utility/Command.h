//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:ƒRƒ}ƒ“ƒhƒNƒ‰ƒX
//
//day:2017/07/13
//
//acotr:Koshida Kenji
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
#pragma once
#include "..\Character\Player\Player.h"

class Command
{
public:
    virtual ~Command() {};
    virtual void execute(Player* player) = 0;
};

/// <summary>
/// ãUŒ‚ƒRƒ}ƒ“ƒh
/// </summary>
class LightAttackCommand : public Command
{
public:
    void execute(Player* player)override { player->SetLightAttackInputState(true); };
};

/// <summary>
/// ƒWƒƒƒ“ƒvƒRƒ}ƒ“ƒh
/// </summary>
class JumpCommand : public Command
{
public:
    void execute(Player* player)override { player->StartJump(); };
};

/// <summary>
/// ‰ñ”ğƒRƒ}ƒ“ƒh
/// </summary>
class EvadeCommand : public Command
{
public:
    void execute(Player* player)override { player->SetEvadeInputState(true); }
};

/// <summary>
/// ‘–‚èƒRƒ}ƒ“ƒh
/// </summary>
class RunCommand : public Command
{
public:
    void execute(Player* player)override { player->Run(true); }
};

/// <summary>
/// ƒRƒ}ƒ“ƒh–³‚µ
/// </summary>
class NonCommand : public Command
{
public:
    void execute(Player* player)override {};
};
