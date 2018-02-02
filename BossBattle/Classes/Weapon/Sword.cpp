#include "Sword.h"
#include "..\Utility\Singleton.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

const int Sword::LIGHT_ATTACK1_DAMAGE = 3;      //弱攻撃1のダメージ
const int Sword::LIGHT_ATTACK2_DAMAGE = 5;      //弱攻撃2のダメージ
const int Sword::LIGHT_ATTACK3_DAMAGE = 7;      //弱攻撃3のダメージ

const int Sword::JUMP_ATTACK1_DAMAGE = 5;       //ジャンプ攻撃1のダメージ
const int Sword::JUMP_ATTACK2_DAMAGE = 8;       //ジャンプ攻撃2のダメージ
const int Sword::JUMP_ATTACK3_DAMAGE = 15;      //ジャンプ攻撃3のダメージ

/// <summary>
/// 剣のコンストラクタ
/// </summary>
Sword::Sword()
{
    m_pos = Vector3(0, 1.0f, 0);
    m_angle = Vector3(XMConvertToRadians(160), 0, XMConvertToRadians(90));

    /// <summary>
    /// 与ダメージの設定
    /// </summary>
    m_lightattack1_damage = LIGHT_ATTACK1_DAMAGE;
    m_lightattack2_damage = LIGHT_ATTACK2_DAMAGE;
    m_lightattack3_damage = LIGHT_ATTACK3_DAMAGE;
    m_jumpattack1_damage = JUMP_ATTACK1_DAMAGE;
    m_jumpattack2_damage = JUMP_ATTACK2_DAMAGE;
    m_jumpattack3_damage = JUMP_ATTACK3_DAMAGE;

    //モデルの設定
    m_model_weapon.LoadModel(L"Resources/Sword.cmo");
    m_model_weapon.SetPos(m_pos);
    m_model_weapon.SetAngle(m_angle);
    m_model_weapon.SetParent(&m_tmp_player);

    //当たり判定の設定
    m_sword_collision.Initialize();
    m_sword_collision.SetParent(&m_model_weapon);
    m_sword_collision.SetOffset(Vector3(0, 4.0f, 0));
    m_sword_collision.SetLocalRadius(3.0f);
    m_sword_collision.Update();
}

/// <summary>
/// 剣の更新
/// </summary>
void Sword::Update()
{
    //プレイヤー情報の更新
    m_tmp_player.SetPos(m_player_pos);
    m_tmp_player.SetAngle(m_player_angle);
    m_tmp_player.Update();

    //剣モデルの更新
    m_model_weapon.SetPos(m_pos);
    m_model_weapon.SetAngle(m_angle);
    m_model_weapon.SetBoneWorld(m_bone_info);
    m_model_weapon.Update();

    //衝突判定の更新
    m_sword_collision.Update();
}

/// <summary>
/// 剣の描画
/// </summary>
void Sword::Render()
{
    //武器の更新
    m_model_weapon.Render();

    //当たり判定の描画
    m_sword_collision.Render();
}