//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:“G‚ÌŠî’êƒNƒ‰ƒX
//
//day:2017/07/10
//
//acotr:Koshida Kenji
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/

#pragma once
#include "..\Character.h"
#include "..\Player\Player.h"

/// <summary>
/// –Ú•WŠp“x‚Ü‚Å‚Ì·•ª‚ÌŒvZ
/// </summary>
/// <param name="angle0">Œ»İ‚ÌŠp“x</param>
/// <param name="angle1">–Ú•W‚ÌŠp“x</param>
/// <returns>Šp“x‚Ì·•ª</returns>
static float GetShortAngleRad(float angle0, float angle1)
{
    float angle_sub;

    angle_sub = angle1 - angle0;	// Šp“x‚Ì·

    // ·‚ª180“x‚æ‚è‘å‚«‚©‚Á‚½ê‡A‹t‰ñ“]‚Ì•û‚ª‹ß‚¢‚Ì‚ÅAƒ}ƒCƒiƒX‚É•ÏŠ·
    // ÅI“I‚É-180`+180“x‚Ì”ÍˆÍ‚ÉB
    if (angle_sub > DirectX::XM_PI)
        angle_sub -= DirectX::XM_2PI;
    else if (angle_sub < -DirectX::XM_PI)
        angle_sub += DirectX::XM_2PI;

    return angle_sub;
}

class Enemy : public Character
{
public:
    //“G‚Ìó‘Ô‘JˆÚ
    class EnemyState
    {
    public:
        virtual ~EnemyState() {};
        //‰Šú‰»
        virtual void Initialize(Enemy* enemy) = 0;
        //XV
        virtual EnemyState* Update(Enemy* enemy) { return nullptr; };
    };
public:
    //ƒRƒ“ƒXƒgƒ‰ƒNƒ^
    Enemy()
    {
        m_enemy_state = nullptr;
        m_player = nullptr;
        m_camera = nullptr;
    };
    virtual ~Enemy() {};
    //ƒvƒŒƒCƒ„[î•ñ‚Ìİ’è
    void SetPlayerInfo(Player* player) { m_player = player; }
    //ƒJƒƒ‰î•ñ‚Ìİ’è
    void SetCamera(Camera* camera) { m_camera = camera; }
    //“Gó‘Ô‚Ìİ’è
    void SetState(EnemyState* state)
    {
        if (m_enemy_state != nullptr)
        {
            delete m_enemy_state;
            m_enemy_state = state;
            m_enemy_state->Initialize(this);
        }
    }

    //ƒXƒ^ƒ“’lŒvZ
    void CalcStunDamage(int damage) { m_stun += damage; }
    //ƒXƒ^ƒ“’l‰Šú‰»
    void InitStun() { m_stun = 0; }

    //ƒvƒŒƒCƒ„[î•ñæ“¾
    const Player* GetPlayerInfo() { return m_player; }
    //ƒJƒƒ‰î•ñæ“¾
    Camera* GetCamera() { return m_camera; }
    //ƒXƒ^ƒ“’l‚ª—­‚Ü‚Á‚½‚©‚Ç‚¤‚©
    virtual bool IsStun() = 0;

protected:
    EnemyState* m_enemy_state;	//ó‘Ô‘JˆÚ
    Player*     m_player;		//ƒvƒŒƒCƒ„[î•ñ
    int m_stun;					//ƒXƒ^ƒ“’l
    Camera* m_camera;			//ƒJƒƒ‰î•ñ
};

