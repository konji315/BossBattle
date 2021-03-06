//〜/〜/〜//〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/
//Content:敵の基底クラス
//
//day:2017/07/10
//
//acotr:Koshida Kenji
//〜/〜/〜//〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/

#pragma once
#include "..\Character.h"
#include "..\Player\Player.h"

/// <summary>
/// 目標角度までの差分の計算
/// </summary>
/// <param name="angle0">現在の角度</param>
/// <param name="angle1">目標の角度</param>
/// <returns>角度の差分</returns>
static float GetShortAngleRad(float angle0, float angle1)
{
    float angle_sub;

    angle_sub = angle1 - angle0;	// 角度の差

    // 差が180度より大きかった場合、逆回転の方が近いので、マイナスに変換
    // 最終的に-180〜+180度の範囲に。
    if (angle_sub > DirectX::XM_PI)
        angle_sub -= DirectX::XM_2PI;
    else if (angle_sub < -DirectX::XM_PI)
        angle_sub += DirectX::XM_2PI;

    return angle_sub;
}

class Enemy : public Character
{
public:
    //敵の状態遷移
    class EnemyState
    {
    public:
        virtual ~EnemyState() {};
        //初期化
        virtual void Initialize(Enemy* enemy) = 0;
        //更新
        virtual EnemyState* Update(Enemy* enemy) { return nullptr; };
    };
public:
    //コンストラクタ
    Enemy()
    {
        m_enemy_state = nullptr;
        m_player = nullptr;
        m_camera = nullptr;
    };
    virtual ~Enemy() {};
    //プレイヤー情報の設定
    void SetPlayerInfo(Player* player) { m_player = player; }
    //カメラ情報の設定
    void SetCamera(Camera* camera) { m_camera = camera; }
    //敵状態の設定
    void SetState(EnemyState* state)
    {
        if (m_enemy_state != nullptr)
        {
            delete m_enemy_state;
            m_enemy_state = state;
            m_enemy_state->Initialize(this);
        }
    }

    //スタン値計算
    void CalcStunDamage(int damage) { m_stun += damage; }
    //スタン値初期化
    void InitStun() { m_stun = 0; }

    //プレイヤー情報取得
    const Player* GetPlayerInfo() { return m_player; }
    //カメラ情報取得
    Camera* GetCamera() { return m_camera; }
    //スタン値が溜まったかどうか
    virtual bool IsStun() = 0;

protected:
    EnemyState* m_enemy_state;	//状態遷移
    Player*     m_player;		//プレイヤー情報
    int m_stun;					//スタン値
    Camera* m_camera;			//カメラ情報
};

