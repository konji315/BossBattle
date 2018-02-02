#include "Skeleton.h"
#include "..\..\Utility\Singleton.h"
#include "..\..\Manager\AudioManager.h"
#include "..\..\Manager\ParticleEffectManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

const int Skeleton::MAX_HP = 100;       //最大体力
const int Skeleton::MAX_STUN = 50;      //最大スタン値
const int Skeleton::ATTACK_PATTERN = 2; //攻撃パターン
const int Skeleton::DAMAGE = 15;        //ダメージ値

/// <summary>
/// コンストラクタ
/// </summary>
Skeleton::Skeleton()
{
    //m_stun = Skeleton::MAX_STUN;
    m_hp = Skeleton::MAX_HP;
    m_damage = Skeleton::DAMAGE;
    m_damage_color_timer = 0;

    m_dead = false;

    //スケルトンモデルの読み込み
    m_model.LoadModel(L"Resources/skeleton.cmo");

    //UIの設定
    m_status_canvas.SetTexture("Boss_stun", L"Resources/Sprites/enemy_stun_bar.png");
    m_status_canvas.SetTexturePos("Boss_stun", Vector2(778, 121));
    m_status_canvas.SetTexture("Boss_HP", L"Resources/Sprites/boss_hp_frame.png");
    m_status_canvas.SetTexturePos("Boss_HP", Vector2(512, 100));
    m_status_canvas.SetTexture("Boss_HP_bar", L"Resources/Sprites/enemy_HP_bar.png");
    m_status_canvas.SetTexturePos("Boss_HP_bar", Vector2(511, 79));

    //手の当たり判定
    for (int i = 0; i < HAND_NUM; i++)
    {
        m_hand_collision[i].Initialize();
        m_hand_collision[i].SetParent(&m_model);
        m_hand_collision[i].SetLocalRadius(5.0f);
    }

    //角の当たり判定
    for (int i = 0; i < HORN_NUM; i++)
    {
        m_horn_collision[i].Initialize();
        m_horn_collision[i].SetParent(&m_model);
        m_horn_collision[i].SetOffset(Vector3(0, 11, 0));
        m_horn_collision[i].SetLocalRadius(6.0f);
    }

    //モデルの色の初期化
    m_model.ResetColor();

    //初期化
    Initialize();

    //初期状態の設定
    m_enemy_state = new Skeleton::IdleState();
    m_enemy_state->Initialize(this);
}

/// <summary>
/// デストラクタ
/// </summary>
Skeleton::~Skeleton()
{
    //ステートの削除
    if (m_enemy_state != nullptr)
        delete m_enemy_state;
}

/// <summary>
/// 初期化
/// </summary>
void Skeleton::Initialize()
{
    m_anim_name = L"Idle";
    InitStun();
}

/// <summary>
/// 更新
/// </summary>
void Skeleton::Update()
{
    //死んだら更新終了
    if (m_dead)
        return;

    //状態遷移
    EnemyState* state = m_enemy_state->Update(this);
    if (state != nullptr)
    {
        delete m_enemy_state;
        m_enemy_state = state;
        m_enemy_state->Initialize(this);
    }

    //位置情報の設定
    m_model.SetPos(m_pos);
    m_model.SetAngle(m_angle);
    m_model.Update();
    m_model.UpdateAnimation(m_anim_name);

    //手のボーン情報セット
    m_hand_collision[HAND::RIGHT_TOP].SetBoneWorld(m_model.GetBoneWorld(22));
    m_hand_collision[HAND::RIGHT_BOTTOM].SetBoneWorld(m_model.GetBoneWorld(12));
    m_hand_collision[HAND::LEFT_TOP].SetBoneWorld(m_model.GetBoneWorld(19));
    m_hand_collision[HAND::LEFT_BOTTOM].SetBoneWorld(m_model.GetBoneWorld(15));

    //角のボーン情報セット
    m_horn_collision[HORN::RIGHT].SetBoneWorld(m_model.GetBoneWorld(4));
    m_horn_collision[HORN::LEFT].SetBoneWorld(m_model.GetBoneWorld(8));

    //手の当たり判定の更新
    for (int i = 0; i < HAND_NUM; i++)
    {
        m_hand_collision[i].Update();
    }
    //角の当たり判定
    for (int i = 0; i < HORN_NUM; i++)
    {
        m_horn_collision[i].Update();
    }

    //体力がある間
    if (m_hp > 0)
    {
        //体力バーの設定
        UINT width = m_status_canvas.GetDesc("Boss_HP_bar").Width;
        RECT rect = m_status_canvas.GetRect("Boss_HP_bar");
        rect.right = width * m_hp / Skeleton::MAX_HP;

        m_status_canvas.SetRect("Boss_HP_bar", rect);

        //スタンバーの設定
        width = m_status_canvas.GetDesc("Boss_stun").Width;
        rect = m_status_canvas.GetRect("Boss_stun");
        //m_stun = min(m_stun, MAX_STUN);
        rect.right = LONG(width * (1 - (float)m_stun / Skeleton::MAX_STUN));

        m_status_canvas.SetRect("Boss_stun", rect);
    }

    //ダメージ色になったとき
    if (m_damage_color_timer != 0)
    {
        m_damage_color_timer -= 1 / 60.0f;
        if (m_damage_color_timer <= 0)
            m_model.ResetColor();
    }
}

/// <summary>
/// 描画
/// </summary>
void Skeleton::Render()
{
    m_model.Render();

    //手の当たり判定の描画
    for (int i = 0; i < HAND_NUM; i++)
    {
        m_hand_collision[i].Render();
    }
    //角の当たり判定
    for (int i = 0; i < HORN_NUM; i++)
    {
        m_horn_collision[i].Render();
    }

    //体力が尽きたらUI非表示
    if (m_hp > 0)
    {
        //UIの表示
        m_status_canvas.Render();
    }
}

#pragma region SkeletonState

/// <summary>
/// スケルトンデモ状態初期化
/// </summary>
void Skeleton::DemoState::Initialize(Enemy * enemy)
{
    enemy->ChangeAnimation(L"Demo");
}

/// <summary>
/// スケルトンデモ状態更新
/// </summary>
Skeleton::EnemyState * Skeleton::DemoState::Update(Enemy * enemy)
{
    return nullptr;
}

/// <summary>
/// スケルトン待機状態初期化
/// </summary>
void Skeleton::IdleState::Initialize(Enemy* enemy)
{
    enemy->ChangeAnimation(L"Idle");    //待機アニメーション再生
    enemy->SetCollisionState(false);    //当たり判定OFF
    enemy->SetAttackState(false);       //攻撃判定OFF
}

/// <summary>
/// スケルトン待機状態更新
/// </summary>
Skeleton::EnemyState* Skeleton::IdleState::Update(Enemy* enemy)
{
    const Player* player = enemy->GetPlayerInfo();

    //プレイヤーがいるなら
    if (player)
    {
        Vector3 player_pos = player->GetPos();
        //徐々にプレイヤーの方向に向く
        float enemy_angle = enemy->GetAngle().y;
        float radian = -atan2(player_pos.z, player_pos.x);
        float angle = GetShortAngleRad(enemy_angle, radian + XMConvertToRadians(90));
        enemy_angle += angle * 0.01f;

        enemy->SetAngle(Vector3(0, enemy_angle, 0));
    }


    //待機終了したら攻撃
    if (!enemy->IsAnim(L"Idle"))
    {
        EnemyState* state;

        int pattern = rand() % Skeleton::ATTACK_PATTERN;

        switch (pattern)
        {
        case 0:
            state = new Skeleton::Attack1State();
            break;
        case 1:
            state = new Skeleton::Attack2State();
            break;
        default:
            break;
        }
        return state;
    }
    return nullptr;
}

/// <summary>
/// 攻撃1の初期化
/// </summary>
void Skeleton::Attack1State::Initialize(Enemy * enemy)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();
    am.PlaySounds(CRI_PLAYSOUNDS_BOSS_VOICE2);  //ボス音声2再生

    enemy->ChangeAnimation(L"Attack1"); //攻撃1アニメーション再生
    enemy->SetCollisionState(true);		//当たり判定ON
    enemy->SetAttackState(true);		//攻撃判定ON
}

/// <summary>
/// 攻撃1の更新
/// </summary>
Skeleton::EnemyState * Skeleton::Attack1State::Update(Enemy * enemy)
{
    m_timer++;

    //左手攻撃開始時
    if (m_timer == 558)
        enemy->SetAttackState(true);    //当たり判定ON

    //手が地面についたタイミングで地震
    if (m_timer == 156 || m_timer == 590)
    {
        AudioManager& am = singleton<AudioManager>::GetInstance();
        am.PlaySounds(CRI_PLAYSOUNDS_EARTHQUAKE);

        enemy->SetAttackState(false);   //攻撃判定OFF
        enemy->GetCamera()->ShakeCamera(10000);
    }

    //スタン値が溜まったらスタン状態へ
    if (enemy->IsStun())
        return new Skeleton::StunState();

    //攻撃が終了したら待機状態へ
    if (!enemy->IsAnim(L"Attack1"))
        return new Skeleton::IdleState();

    return nullptr;
}

/// <summary>
/// 攻撃2の初期化
/// </summary>
void Skeleton::Attack2State::Initialize(Enemy * enemy)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();
    am.PlaySounds(CRI_PLAYSOUNDS_BOSS_VOICE2);  //ボス音声2再生

    enemy->ChangeAnimation(L"Attack2"); //攻撃2アニメーション再生
    enemy->SetCollisionState(true);		//当たり判定ON
    enemy->SetAttackState(false);       //攻撃判定OFF
}

/// <summary>
/// 攻撃2の更新
/// </summary>
Skeleton::EnemyState * Skeleton::Attack2State::Update(Enemy * enemy)
{
    m_timer++;
    //薙ぎ払いによる振動
    if ((m_timer >= 162 && m_timer <= 228) || (m_timer >= 400 && m_timer <= 470))
    {
        //攻撃開始
        if (m_timer == 162 || m_timer == 400)
        {
            enemy->SetAttackState(true);
            AudioManager& am = singleton<AudioManager>::GetInstance();
            am.PlaySounds(CRI_PLAYSOUNDS_EARTHQUAKE, 0.8f);
        }
        //攻撃終了
        if (m_timer == 228 || m_timer == 470)
        {
            enemy->SetAttackState(false);
        }
        enemy->GetCamera()->ShakeCamera(2000);
    }

    //スタン値が溜まったらスタン状態へ
    if (enemy->IsStun())
        return new Skeleton::StunState();

    //攻撃が終了したら待機状態へ
    if (!enemy->IsAnim(L"Attack2"))
        return new Skeleton::IdleState();

    return nullptr;
}

/// <summary>
/// スケルトン死亡状態初期化
/// </summary>
void Skeleton::DeadState::Initialize(Enemy * enemy)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();
    am.PlaySounds(CRI_PLAYSOUNDS_BOSS_VOICE1);      //ボス音声1再生

    enemy->ChangeAnimation(L"Dead");    //死亡アニメーション再生
    enemy->SetCollisionState(false);    //当たり判定OFF
    enemy->SetAttackState(false);       //攻撃判定OFF
}

/// <summary>
/// スケルトン死亡状態更新
/// </summary>
Skeleton::EnemyState * Skeleton::DeadState::Update(Enemy * enemy)
{
    m_timer++;

    //爆発エフェクト生成
    if (m_timer % 30 == 0 && m_timer < 300)
    {
        ParticleEffectManager& pem = singleton<ParticleEffectManager>::GetInstance();

        Vector3 rnd_pos((float)(rand() % 30 - 15), (float)(rand() % 20), (float)(rand() % 30 - 15));

        pem.Play("explosion1", enemy->GetPos() + rnd_pos, 3);

    }

    //死亡モーションが終了したら死亡
    if (!enemy->IsAnim(L"Dead"))
        enemy->Dead();

    return nullptr;
}

/// <summary>
/// スケルトンスタン状態初期化
/// </summary>
void Skeleton::StunState::Initialize(Enemy * enemy)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();
    am.PlaySounds(CRI_PLAYSOUNDS_BOSS_VOICE2);  //ボス音声2再生

    enemy->ChangeAnimation(L"Stun");        //スタンアニメーション再生
    enemy->SetCollisionState(true);         //当たり判定ON
    enemy->SetAttackState(false);           //攻撃判定OFF
}

/// <summary>
/// スケルトンスタン状態更新
/// </summary>
Skeleton::EnemyState * Skeleton::StunState::Update(Enemy * enemy)
{
    //体力が尽きたら死亡状態へ
    if (!enemy->ExistsHP())
        return new Skeleton::DeadState();

    //スタンモーションが終了したら待機状態へ
    if (!enemy->IsAnim(L"Stun"))
    {
        enemy->InitStun();	//スタン値初期化
        return new Skeleton::IdleState();
    }
    return nullptr;
}

#pragma endregion
