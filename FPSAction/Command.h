//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:ƒRƒ}ƒ“ƒhƒNƒ‰ƒX
//
//day:2017/07/13
//
//acotr:Koshida Kenji
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
#pragma once
#include "Classes\Player\Player.h"

class Command
{
public:
	virtual ~Command() {};
	virtual void execute(Player* player) = 0;
};

class LightAttackCommand : public Command
{
public:
	void execute(Player* player)override { player->StartLightAttack(); };
};

class HeavyAttackCommand : public Command
{
public :
	void execute(Player* player)override { player->StartHeavyAttack(); };
};

class JumpCommand : public Command
{
public:
	void execute(Player* player)override { player->StartJump(); };
};

class NonCommand : public Command
{
public:
	void execute(Player* player)override {};
};