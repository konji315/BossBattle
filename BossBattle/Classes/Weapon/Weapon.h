//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:•Ší‚ÌŠî’êƒNƒ‰ƒX
//
//day:2017/07/9
//
//acotr:Koshida Kenji
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/

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

    //‰Šú‰»
    virtual void Initialize() = 0;

    //XV
    virtual void Update() = 0;
    //•`‰æ
    virtual void Render() = 0;

    //ƒvƒŒƒCƒ„[‚ÌƒZƒbƒg
    virtual void SetPlayerPos(const DirectX::SimpleMath::Vector3& pos) { m_player_pos = pos; };
    virtual void SetPlayerAngle(const DirectX::SimpleMath::Vector3& angle) { m_player_angle = angle; };

    //ƒ{[ƒ“î•ñ‚ÌƒZƒbƒg
    virtual void SetBoneWorld(const DirectX::SimpleMath::Matrix& boneinfo) { m_bone_info = boneinfo; }

    /// <summary>
    /// ƒ_ƒ[ƒW‚Ìæ“¾
    /// </summary>
    int GetLightAttack1Damage() const { return m_lightattack1_damage; }
    int GetLightAttack2Damage() const { return m_lightattack2_damage; }
    int GetLightAttack3Damage() const { return m_lightattack3_damage; }
    int GetJumpAttack1Damage() const { return m_jumpattack1_damage; }
    int GetJumpAttack2Damage() const { return m_jumpattack2_damage; }
    int GetJumpAttack3Damage() const { return m_jumpattack3_damage; }

    //”»’è‚Ìæ“¾
    const SphereNode& GetCollisonSphere()const { return m_sword_collision; }

protected:
    Obj3d m_model_weapon;                           //•Ší‚Ìƒ‚ƒfƒ‹
    Obj3d m_tmp_player;                             //ƒvƒŒƒCƒ„[î•ñ

    SphereNode m_sword_collision;			        //Œ•‚Ì“–‚½‚è”»’è

    int m_lightattack1_damage;				        //ãUŒ‚1‚Ì—^ƒ_ƒ[ƒW
    int m_lightattack2_damage;				        //ãUŒ‚2‚Ì—^ƒ_ƒ[ƒW
    int m_lightattack3_damage;				        //ãUŒ‚3‚Ì—^ƒ_ƒ[ƒW
    int m_jumpattack1_damage;				        //ƒWƒƒƒ“ƒvUŒ‚1‚Ì—^ƒ_ƒ[ƒW
    int m_jumpattack2_damage;				        //ƒWƒƒƒ“ƒvUŒ‚2‚Ì—^ƒ_ƒ[ƒW
    int m_jumpattack3_damage;				        //ƒWƒƒƒ“ƒvUŒ‚3‚Ì—^ƒ_ƒ[ƒW

    DirectX::SimpleMath::Matrix m_bone_info;        //ƒ{[ƒ“î•ñ

    DirectX::SimpleMath::Vector3 m_pos;             //ˆÊ’uî•ñ
    DirectX::SimpleMath::Vector3 m_angle;           //Šp“x

    DirectX::SimpleMath::Vector3 m_player_pos;      //ƒvƒŒƒCƒ„[‚ÌˆÊ’u
    DirectX::SimpleMath::Vector3 m_player_angle;    //ƒvƒŒƒCƒ„[‚ÌŠp“x
};