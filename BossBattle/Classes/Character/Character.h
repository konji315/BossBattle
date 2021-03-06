//〜/〜/〜//〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/
//Content:キャラクターの基底クラス
//
//day:2017/07/10
//
//acotr:Koshida Kenji
//〜/〜/〜//〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/

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

    //場所の設定
    void SetPos(const DirectX::SimpleMath::Vector3& pos) { m_pos = pos; }
    //角度の設定
    void SetAngle(const DirectX::SimpleMath::Vector3& angle) { m_angle = angle; }
    //ダメージ計算
    void CalcDamage(const int damage) { m_hp -= damage; }

    //場所の取得
    const DirectX::SimpleMath::Vector3& GetPos() const { return m_pos; }
    //角度の取得
    const DirectX::SimpleMath::Vector3& GetAngle() const { return m_angle; }
    //移動速度の取得
    const float GetMoveSpeed() const { return m_move_speed; }
    //HPがあるかどうか
    bool ExistsHP()
    {
        if (m_hp > 0)
            return true;

        return false;
    }
    //移動方向ベクトルの取得
    const DirectX::SimpleMath::Vector2 GetMoveVec()
    {
        DirectX::SimpleMath::Vector2 vec(0, 1);
        DirectX::SimpleMath::Vector2 move_vec;
        move_vec.x = vec.x * cosf(m_angle.y) - vec.y * sinf(m_angle.y);
        move_vec.y = vec.x * sinf(m_angle.y) + vec.y * cosf(m_angle.y);
        move_vec.Normalize();

        return move_vec;
    }
    //衝突判定の有無の取得
    const bool GetCollisionState() { return m_is_collision; }
    //攻撃判定状態の取得
    const bool GetAttackState() { return m_is_attack; }
    //判定の取得
    const SphereNode& GetCollisonSphere() { return m_collision_sphere; }
    //与ダメの取得
    int GetDamage() const { return m_damage; }

    //アニメーション状態の取得
    bool IsAnim(const std::wstring& clip_name) { return m_model.IsAnim(clip_name); }

    //死亡
    void Dead() { m_dead = true; }
    //倒されたかどうか
    bool IsDead() { return m_dead; }

    //モーション変更
    void ChangeAnimation(wchar_t* clipname)
    {
        m_model.ResetAnimation();
        m_anim_name = clipname;
    }

    //当たり判定状態のセット(ダメージを受ける側)
    void SetCollisionState(bool state) { m_is_collision = state; }
    //攻撃判定のセット(ダメージを与える側)
    void SetAttackState(bool state) { m_is_attack = state; }

    //ダメージ時のモデルの色変化
    void HitDamageColor(const DirectX::XMVECTORF32& color)
    {
        m_model.ChangeModelColor(color);	//色を変える

        m_damage_color_timer = 0.2f;		//タイマーセット
    }
protected:
    Obj3d m_model;							//モデル

    DirectX::SimpleMath::Vector3 m_pos;		//座標

    DirectX::SimpleMath::Vector3 m_angle;	//角度

    float m_move_speed;						//移動速度

    int m_hp;								//体力
    int m_damage;							//相手に与えるダメージ

    SphereNode m_collision_sphere;			//当たり判定

    Canvas m_status_canvas;					//ステータスキャンバス

    bool m_dead;							//死亡フラグ

    wchar_t* m_anim_name;					//現在再生しているアニメーション名

    bool m_is_collision;					//衝突するかどうか

    bool m_is_attack;						//攻撃中かどうか

    float m_damage_color_timer;				//モデルの色変化タイマー
};

