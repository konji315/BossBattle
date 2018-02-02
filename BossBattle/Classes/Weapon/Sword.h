//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:Œ•‚Ì•ŠíƒNƒ‰ƒX
//
//day:2017/11/14
//
//actor:Koshida Kenji
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/

#pragma once
#include "Weapon.h"
#include "..\Collider\CollisionNode.h"

class Sword : public Weapon
{
public:
    static const int LIGHT_ATTACK1_DAMAGE;  //ãUŒ‚1‚Ìƒ_ƒ[ƒW
    static const int LIGHT_ATTACK2_DAMAGE;  //ãUŒ‚2‚Ìƒ_ƒ[ƒW
    static const int LIGHT_ATTACK3_DAMAGE;  //ãUŒ‚3‚Ìƒ_ƒ[ƒW

    static const int JUMP_ATTACK1_DAMAGE;   //ƒWƒƒƒ“ƒvUŒ‚1‚Ìƒ_ƒ[ƒW
    static const int JUMP_ATTACK2_DAMAGE;   //ƒWƒƒƒ“ƒvUŒ‚2‚Ìƒ_ƒ[ƒW
    static const int JUMP_ATTACK3_DAMAGE;   //ƒWƒƒƒ“ƒvUŒ‚3‚Ìƒ_ƒ[ƒW

public:
    Sword();
    virtual ~Sword(){}

    //‰Šú‰»
    void Initialize()override{}
    //XV
    void Update()override;
    //•`‰æ
    void Render()override;
};

