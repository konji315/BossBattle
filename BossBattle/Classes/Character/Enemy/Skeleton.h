//〜/〜/〜//〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/
//Content:スケルトンクラス
//
//day:2017/11/25
//
//acotr:Koshida Kenji
//〜/〜/〜//〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/
#pragma once
#include "Enemy.h"
#include "..\..\Camera\BossCamera.h"

class Skeleton : public Enemy
{
public:
    static const int MAX_HP;		    //最大体力
    static const int MAX_STUN;		    //最大スタン値
    static const int ATTACK_PATTERN;    //攻撃パターン数
    static const int DAMAGE;		    //プレイヤーに与えるダメージ

    enum HAND			//手
    {
        RIGHT_TOP,		//右上の手
        RIGHT_BOTTOM,	//右下の手
        LEFT_TOP,		//左上の手
        LEFT_BOTTOM,	//左下の手
        HAND_NUM
    };

    enum HORN			//角
    {
        RIGHT,			//右角
        LEFT,			//左角
        HORN_NUM
    };

public:
    //デモ
    class DemoState : public EnemyState
    {
    public:
        DemoState() {}
        void Initialize(Enemy* enemy)override;
        EnemyState* Update(Enemy* enemy)override;
    };
    //待機
    class IdleState : public EnemyState
    {
    public:
        IdleState() {}
        void Initialize(Enemy* enemy)override;
        EnemyState* Update(Enemy* enemy)override;
    };

    //攻撃1
    class Attack1State : public EnemyState
    {
    public:
        Attack1State() { m_timer = 0; }
        void Initialize(Enemy* enemy)override;
        EnemyState* Update(Enemy* enemy)override;
    private:
        int m_timer;		//タイマー
    };

    //攻撃2
    class Attack2State : public EnemyState
    {
    public:
        Attack2State() { m_timer = 0; }
        void Initialize(Enemy* enemy)override;
        EnemyState* Update(Enemy* enemy)override;
    private:
        int m_timer;		//タイマー
    };

    //スタン状態
    class StunState : public EnemyState
    {
    public:
        StunState() {}
        void Initialize(Enemy* enemy)override;
        EnemyState* Update(Enemy* enemy)override;
    };

    //死亡
    class DeadState : public EnemyState
    {
    public:
        DeadState() { m_timer = 0; }
        void Initialize(Enemy* enemy)override;
        EnemyState* Update(Enemy* enemy)override;
    private:
        int m_timer;		//タイマー
    };
public:
    Skeleton();
    ~Skeleton();

    //初期化
    void Initialize()override;
    //更新
    void Update()override;
    //描画
    void Render()override;

    //移動
    void Move(DirectX::SimpleMath::Vector3 movev)override {}

    //スタンしたかどうか
    bool IsStun()override
    {
        if (m_stun >= MAX_STUN)
            return true;

        return false;
    };

    //角の当たり判定の取得
    const SphereNode& GetHornCollision(int id) { return m_horn_collision[id]; }
    //手の当たり判定の取得
    const SphereNode& GetHandCollision(int id) { return m_hand_collision[id]; }
private:
    SphereNode m_horn_collision[HORN_NUM];	//角の当たり判定(2本分)
    SphereNode m_hand_collision[HAND_NUM];  //手の当たり判定(4本分)
};