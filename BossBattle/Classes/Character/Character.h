//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:ƒLƒƒƒ‰ƒNƒ^[‚ÌŠî’êƒNƒ‰ƒX
//
//day:2017/07/10
//
//acotr:Koshida Kenji
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/

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

    //êŠ‚Ìİ’è
    void SetPos(const DirectX::SimpleMath::Vector3& pos) { m_pos = pos; }
    //Šp“x‚Ìİ’è
    void SetAngle(const DirectX::SimpleMath::Vector3& angle) { m_angle = angle; }
    //ƒ_ƒ[ƒWŒvZ
    void CalcDamage(const int damage) { m_hp -= damage; }

    //êŠ‚Ìæ“¾
    const DirectX::SimpleMath::Vector3& GetPos() const { return m_pos; }
    //Šp“x‚Ìæ“¾
    const DirectX::SimpleMath::Vector3& GetAngle() const { return m_angle; }
    //ˆÚ“®‘¬“x‚Ìæ“¾
    const float GetMoveSpeed() const { return m_move_speed; }
    //HP‚ª‚ ‚é‚©‚Ç‚¤‚©
    bool ExistsHP()
    {
        if (m_hp > 0)
            return true;

        return false;
    }
    //ˆÚ“®•ûŒüƒxƒNƒgƒ‹‚Ìæ“¾
    const DirectX::SimpleMath::Vector2 GetMoveVec()
    {
        DirectX::SimpleMath::Vector2 vec(0, 1);
        DirectX::SimpleMath::Vector2 move_vec;
        move_vec.x = vec.x * cosf(m_angle.y) - vec.y * sinf(m_angle.y);
        move_vec.y = vec.x * sinf(m_angle.y) + vec.y * cosf(m_angle.y);
        move_vec.Normalize();

        return move_vec;
    }
    //Õ“Ë”»’è‚Ì—L–³‚Ìæ“¾
    const bool GetCollisionState() { return m_is_collision; }
    //UŒ‚”»’èó‘Ô‚Ìæ“¾
    const bool GetAttackState() { return m_is_attack; }
    //”»’è‚Ìæ“¾
    const SphereNode& GetCollisonSphere() { return m_collision_sphere; }
    //—^ƒ_ƒ‚Ìæ“¾
    int GetDamage() const { return m_damage; }

    //ƒAƒjƒ[ƒVƒ‡ƒ“ó‘Ô‚Ìæ“¾
    bool IsAnim(const std::wstring& clip_name) { return m_model.IsAnim(clip_name); }

    //€–S
    void Dead() { m_dead = true; }
    //“|‚³‚ê‚½‚©‚Ç‚¤‚©
    bool IsDead() { return m_dead; }

    //ƒ‚[ƒVƒ‡ƒ“•ÏX
    void ChangeAnimation(wchar_t* clipname)
    {
        m_model.ResetAnimation();
        m_anim_name = clipname;
    }

    //“–‚½‚è”»’èó‘Ô‚ÌƒZƒbƒg(ƒ_ƒ[ƒW‚ğó‚¯‚é‘¤)
    void SetCollisionState(bool state) { m_is_collision = state; }
    //UŒ‚”»’è‚ÌƒZƒbƒg(ƒ_ƒ[ƒW‚ğ—^‚¦‚é‘¤)
    void SetAttackState(bool state) { m_is_attack = state; }

    //ƒ_ƒ[ƒW‚Ìƒ‚ƒfƒ‹‚ÌF•Ï‰»
    void HitDamageColor(const DirectX::XMVECTORF32& color)
    {
        m_model.ChangeModelColor(color);	//F‚ğ•Ï‚¦‚é

        m_damage_color_timer = 0.2f;		//ƒ^ƒCƒ}[ƒZƒbƒg
    }
protected:
    Obj3d m_model;							//ƒ‚ƒfƒ‹

    DirectX::SimpleMath::Vector3 m_pos;		//À•W

    DirectX::SimpleMath::Vector3 m_angle;	//Šp“x

    float m_move_speed;						//ˆÚ“®‘¬“x

    int m_hp;								//‘Ì—Í
    int m_damage;							//‘Šè‚É—^‚¦‚éƒ_ƒ[ƒW

    SphereNode m_collision_sphere;			//“–‚½‚è”»’è

    Canvas m_status_canvas;					//ƒXƒe[ƒ^ƒXƒLƒƒƒ“ƒoƒX

    bool m_dead;							//€–Sƒtƒ‰ƒO

    wchar_t* m_anim_name;					//Œ»İÄ¶‚µ‚Ä‚¢‚éƒAƒjƒ[ƒVƒ‡ƒ“–¼

    bool m_is_collision;					//Õ“Ë‚·‚é‚©‚Ç‚¤‚©

    bool m_is_attack;						//UŒ‚’†‚©‚Ç‚¤‚©

    float m_damage_color_timer;				//ƒ‚ƒfƒ‹‚ÌF•Ï‰»ƒ^ƒCƒ}[
};

