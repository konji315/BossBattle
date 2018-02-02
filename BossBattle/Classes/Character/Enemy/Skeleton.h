//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:ƒXƒPƒ‹ƒgƒ“ƒNƒ‰ƒX
//
//day:2017/11/25
//
//acotr:Koshida Kenji
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
#pragma once
#include "Enemy.h"
#include "..\..\Camera\BossCamera.h"

class Skeleton : public Enemy
{
public:
    static const int MAX_HP;		    //Å‘å‘Ì—Í
    static const int MAX_STUN;		    //Å‘åƒXƒ^ƒ“’l
    static const int ATTACK_PATTERN;    //UŒ‚ƒpƒ^[ƒ“”
    static const int DAMAGE;		    //ƒvƒŒƒCƒ„[‚É—^‚¦‚éƒ_ƒ[ƒW

    enum HAND			//Žè
    {
        RIGHT_TOP,		//‰Eã‚ÌŽè
        RIGHT_BOTTOM,	//‰E‰º‚ÌŽè
        LEFT_TOP,		//¶ã‚ÌŽè
        LEFT_BOTTOM,	//¶‰º‚ÌŽè
        HAND_NUM
    };

    enum HORN			//Šp
    {
        RIGHT,			//‰EŠp
        LEFT,			//¶Šp
        HORN_NUM
    };

public:
    //ƒfƒ‚
    class DemoState : public EnemyState
    {
    public:
        DemoState() {}
        void Initialize(Enemy* enemy)override;
        EnemyState* Update(Enemy* enemy)override;
    };
    //‘Ò‹@
    class IdleState : public EnemyState
    {
    public:
        IdleState() {}
        void Initialize(Enemy* enemy)override;
        EnemyState* Update(Enemy* enemy)override;
    };

    //UŒ‚1
    class Attack1State : public EnemyState
    {
    public:
        Attack1State() { m_timer = 0; }
        void Initialize(Enemy* enemy)override;
        EnemyState* Update(Enemy* enemy)override;
    private:
        int m_timer;		//ƒ^ƒCƒ}[
    };

    //UŒ‚2
    class Attack2State : public EnemyState
    {
    public:
        Attack2State() { m_timer = 0; }
        void Initialize(Enemy* enemy)override;
        EnemyState* Update(Enemy* enemy)override;
    private:
        int m_timer;		//ƒ^ƒCƒ}[
    };

    //ƒXƒ^ƒ“ó‘Ô
    class StunState : public EnemyState
    {
    public:
        StunState() {}
        void Initialize(Enemy* enemy)override;
        EnemyState* Update(Enemy* enemy)override;
    };

    //Ž€–S
    class DeadState : public EnemyState
    {
    public:
        DeadState() { m_timer = 0; }
        void Initialize(Enemy* enemy)override;
        EnemyState* Update(Enemy* enemy)override;
    private:
        int m_timer;		//ƒ^ƒCƒ}[
    };
public:
    Skeleton();
    ~Skeleton();

    //‰Šú‰»
    void Initialize()override;
    //XV
    void Update()override;
    //•`‰æ
    void Render()override;

    //ˆÚ“®
    void Move(DirectX::SimpleMath::Vector3 movev)override {}

    //ƒXƒ^ƒ“‚µ‚½‚©‚Ç‚¤‚©
    bool IsStun()override
    {
        if (m_stun >= MAX_STUN)
            return true;

        return false;
    };

    //Šp‚Ì“–‚½‚è”»’è‚ÌŽæ“¾
    const SphereNode& GetHornCollision(int id) { return m_horn_collision[id]; }
    //Žè‚Ì“–‚½‚è”»’è‚ÌŽæ“¾
    const SphereNode& GetHandCollision(int id) { return m_hand_collision[id]; }
private:
    SphereNode m_horn_collision[HORN_NUM];	//Šp‚Ì“–‚½‚è”»’è(2–{•ª)
    SphereNode m_hand_collision[HAND_NUM];  //Žè‚Ì“–‚½‚è”»’è(4–{•ª)
};