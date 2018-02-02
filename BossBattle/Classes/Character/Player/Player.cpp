#include "Player.h"
#include "..\..\Common\DXTKResouces.h"
#include "..\..\Weapon\Sword.h"
#include "..\..\Manager\AudioManager.h"
#include "..\..\Manager\ParticleEffectManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

const float Player::WALK_SPEED = 0.3f;              //歩く速度
const float Player::RUN_SPEED = 0.8f;               //走る速度
const float Player::ATTACK_SPEED = 0.5f;            //攻撃時の速度
const float Player::EVADE_SPEED = 0.8f;             //回避時の速度
const float Player::GRAVITY_ACC = 0.03f;            //重力加速度
const float Player::JUMP_SPEED_FIRST = 0.7f;        //ジャンプ初速度
const float Player::JUMP_SPEED_MAX = 3.0f;          //最大ジャンプ速度
const float Player::DAMAGE_EFFECT_TIMER = 0.3f;     //ダメージエフェクトのタイマー
const int Player::MAX_HP = 50;                      //最大体力

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player()
{
    DXTK::DXTKResources& dxtk = singleton<DXTK::DXTKResources>::GetInstance();

    m_hp = Player::MAX_HP;
    m_horizontal.x = 0;
    m_horizontal.z = 0;
    m_is_jump = false;
    m_is_run = false;
    m_move_speed = 0;
    m_damage_effect_timer = DAMAGE_EFFECT_TIMER;

    m_is_lightattack = false;
    m_is_heavyattack = false;
    m_is_evade = false;

    m_dead = false;

    //プレイヤーモデルの読み込み
    m_model.LoadModel(L"Resources/player.cmo");

    //当たり判定の設定
    m_collision_sphere.Initialize();
    m_collision_sphere.SetParent(&m_model);
    m_collision_sphere.SetOffset(Vector3(0, 1.0f, 0));
    m_collision_sphere.SetLocalRadius(3.0f);
    m_collision_sphere.Update();

    //状態の設定
    m_state = new StandingState();
    m_state->Initialize(*this);

    //武器生成
    m_weapon = new Sword();
    m_weapon->Initialize();

    //当たり判定ON
    SetCollisionState(true);

    //UI設定
    m_status_canvas.SetTexture("HP", L"Resources/Sprites/player_hp.png");
    m_status_canvas.SetTexturePos("HP", Vector2(820, 650));

    m_status_canvas.SetTexture("HP_bar", L"Resources/Sprites/player_hp_bar.png");
    m_status_canvas.SetTexturePos("HP_bar", Vector2(820, 650));

    m_status_canvas.SetTexture("damage_effect", L"Resources/Sprites/damage_effect.png");
    m_status_canvas.SetTexturePos("damage_effect", Vector2(512, 384));
    m_status_canvas.SetVisible("damage_effect", false);
}

Player::~Player()
{
    //武器削除
    if (m_weapon != nullptr)
        delete m_weapon;
    //ステート削除
    if (m_state != nullptr)
        delete m_state;
}

/// <summary>
/// プレイヤーの更新
/// </summary>
void Player::Update()
{
    PlayerState* state = m_state->Update(*this);

    if (state != nullptr)
    {
        delete m_state;

        //死亡か
        if (!ExistsHP())
            m_state = new DeadState();
        else
            m_state = state;

        m_state->Initialize(*this);
    }

    //ジャンプ中
    if (m_is_jump || ExistsHP())
    {
        m_velocity.y -= GRAVITY_ACC;
        if (m_velocity.y < -JUMP_SPEED_MAX)
            m_velocity.y = -JUMP_SPEED_MAX;
    }

    //生きている間は座標更新
    if (!IsDead())
    {
        //重力
        m_pos += m_velocity;

        //モデルの更新
        m_model.SetPos(m_pos);
        m_model.SetAngle(m_angle);
        m_model.UpdateAnimation(m_anim_name);
        m_model.Update();

        //武器の更新
        m_weapon->SetPlayerPos(m_pos);
        m_weapon->SetPlayerAngle(m_angle);
        m_weapon->SetBoneWorld(m_model.GetBoneWorld(7));
        m_weapon->Update();

        //当たり判定の更新
        m_collision_sphere.Update();
    }

    //ダメージエフェクトの非表示
    if (m_status_canvas.GetVisible("damage_effect"))
    {
        m_damage_effect_timer -= 1 / 60.0f;
        if (m_damage_effect_timer < 0)
        {
            m_status_canvas.SetVisible("damage_effect", false);
            m_damage_effect_timer = DAMAGE_EFFECT_TIMER;
        }
    }

    if (m_hp <= 0)
        m_hp = 0;

    if (m_hp >= 0)
    {
        UINT width = m_status_canvas.GetDesc("HP_bar").Width;
        RECT rect = m_status_canvas.GetRect("HP_bar");
        rect.right = width * m_hp / Player::MAX_HP;
        m_status_canvas.SetRect("HP_bar", rect);

        m_hp = min(m_hp, MAX_HP);
    }
}

/// <summary>
/// プレイヤーの描画
/// </summary>
void Player::Render()
{
    //武器の描画
    m_weapon->Render();
    m_collision_sphere.Render();

    //プレイヤーモデルの描画
    m_model.Render();

    //UIの描画
    m_status_canvas.Render();
}

/// <summary>
/// 武器のセット
/// </summary>
/// <param name="weapon">武器</param>
void Player::SetWeapon(Weapon * weapon)
{
    if (m_weapon != nullptr)
    {
        delete m_weapon;
        m_weapon = nullptr;
    }

    m_weapon = weapon;
}

/// <summary>
/// 移動中かどうか
/// </summary>
/// <returns>移動中か</returns>
bool Player::IsMove()
{
    if (std::abs(m_horizontal.x) > 0 || std::abs(m_horizontal.z) > 0)
        return true;
    return false;
}

/// <summary>
///
/// プレイヤーの移動
/// </summary>
/// <param name="movev">移動方向</param>
void Player::Move(DirectX::SimpleMath::Vector3 movev)
{
    //入力が小さい場合はスキップ
    if (std::abs(m_horizontal.x) <= 0.6f && std::abs(m_horizontal.z) <= 0.6f)
        return;

    Matrix transmat = Matrix::CreateTranslation(m_pos);
    Matrix rotmatY = Matrix::CreateRotationY(m_camera_rotate.y - XMConvertToRadians(-90));		//ボス戦専用

    Matrix world = rotmatY * transmat;

    //移動方向ベクトル
    movev = SimpleMath::Vector3::TransformNormal(movev, world);

    ///入力の角度算出(ボス戦専用)
    if (m_horizontal.x != 0 || m_horizontal.z != 0)
    {
        Vector2 y(0, 1);
        Vector2 in(m_horizontal.z, m_horizontal.x);

        float cos_sita = y.Dot(in);

        float sita = acosf(cos_sita);

        //絶対値
        if (m_horizontal.z < 0)
            sita = -sita;

        SetAngle(Vector3(0, m_camera_rotate.y + sita, 0));
    }
    //プレイヤーの座標を移動
    SetPos(m_pos + movev);
}

/// <summary>
/// プレイヤーの回転
/// </summary>
/// <param name="anglev"></param>
void Player::Rotation(const DirectX::SimpleMath::Vector3 & anglev)
{
    SetAngle(m_angle + anglev);
}

//カメラ角度の回転
void Player::CameraRotatin(const DirectX::SimpleMath::Vector3 & rotate)
{
    SetCameraRotate(m_camera_rotate + rotate);
}

/// <summary>
/// ジャンプスタート
/// </summary>
void Player::StartJump()
{
    if (!m_is_jump)
    {
        m_velocity.y = JUMP_SPEED_FIRST;
        m_is_jump = true;
    }
}

/// <summary>
/// ジャンプ終了
/// </summary>
void Player::StopJump()
{
    m_is_jump = false;
    m_velocity = Vector3::Zero;
}

/// <summary>
/// 落下開始
/// </summary>
void Player::StartFall()
{
    if (!m_is_jump)
    {
        m_is_jump = true;
        m_velocity = Vector3::Zero;
    }
}

/// <summary>
/// 走る
/// </summary>
/// <param name="is_run">状態</param>
void Player::Run(bool is_run)
{
    m_is_run = is_run;
    if (m_is_run)
        m_move_speed = RUN_SPEED;
    else
        m_move_speed = WALK_SPEED;
}

/// <summary>
/// ヒットエフェクト発生
/// </summary>
void Player::HitEffect(const DirectX::SimpleMath::Vector3 & pos)
{
    ParticleEffectManager& pem = singleton<ParticleEffectManager>::GetInstance();

    pem.Play("halo", pos, 1);
    pem.Play("slash", pos, 1);
}

/// <summary>
/// 足跡エフェクト
/// </summary>
void Player::Footsteps()
{
    ParticleEffectManager& pem = singleton<ParticleEffectManager>::GetInstance();

    //足の位置に調整
    pem.Play("footsteps", m_pos - Vector3(0, 2, 0), 1);
}

#pragma region PlayerState

/// <summary>
/// 立ち状態の初期化
/// </summary>
/// <param name="player">プレイヤー</param>
void StandingState::Initialize(Player & player)
{
    player.ChangeAnimation(L"Idle");	    //待機モーション再生
    player.SetLightAttackInputState(false);	//弱攻撃終了
    player.SetEvadeInputState(false);		//回避判定OFF
    player.SetAttackState(false);		    //攻撃判定OFF
}

/// <summary>
/// 立ち状態の更新
/// </summary>
/// <param name="player">プレイヤー</param>
/// <returns>次のステート</returns>
PlayerState * StandingState::Update(Player & player)
{
    //ジャンプ開始
    if (player.GetJumpState())
    {
        AudioManager& am = singleton<AudioManager>::GetInstance();
        am.PlaySounds(CRI_PLAYSOUNDS_JUMP1, 0.6f);

        player.Footsteps();
        return new JumpingState();
    }
    //移動開始
    if (player.GetHorizontal().x || player.GetHorizontal().z)
        return new WalkingState();

    //弱攻撃開始
    if (player.GetLightAttackInputState())
        return new LightAttack1State();

    //回避開始
    if (player.GetEvadeInputState())
        return new EvadeState();

    return nullptr;
}

/// <summary>
/// 歩き状態の初期化
/// </summary>
/// <param name="player">プレイヤー</param>
void WalkingState::Initialize(Player & player)
{
    player.ChangeAnimation(L"Walk");	//歩行アニメーション再生
    player.SetAttackState(false);		//攻撃判定OFF
}

/// <summary>
/// 歩き状態の更新
/// </summary>
/// <param name="player">プレイヤー</param>
/// <returns>次のステート</returns>
PlayerState * WalkingState::Update(Player & player)
{
    //足音
    m_timer += 1 / 60.0f;
    if (m_timer >= 0.4f)
    {
        AudioManager& am = singleton<AudioManager>::GetInstance();
        am.PlaySounds(CRI_PLAYSOUNDS_LANDING1, 0.4f);
        m_timer = 0;
    }

    //入力方向への移動量の生成
    Vector3 moveV(player.GetHorizontal().x * player.GetMoveSpeed(), 0.0f, -player.GetHorizontal().z * player.GetMoveSpeed());
    player.Move(moveV);

    //Lが押されたら走り状態
    if (player.GetRunState())
        return new RunningState();

    //ジャンプ開始
    if (player.GetJumpState())
    {
        AudioManager& am = singleton<AudioManager>::GetInstance();
        am.PlaySounds(CRI_PLAYSOUNDS_JUMP1, 0.6f);

        player.Footsteps();
        return new JumpingState();
    }
    //移動終了時
    if (!player.IsMove())
        return new StandingState();

    //弱攻撃開始
    if (player.GetLightAttackInputState())
        return new LightAttack1State();

    //回避開始
    if (player.GetEvadeInputState())
        return new EvadeState();

    return nullptr;
}

/// <summary>
/// ジャンプ状態の初期化
/// </summary>
/// <param name="player">プレイヤー</param>
void JumpingState::Initialize(Player & player)
{
    player.ChangeAnimation(L"Jump");		//ジャンプアニメーション再生
    player.SetEvadeInputState(false);		//回避終了
    player.SetAttackState(false);			//攻撃判定OFF
}

/// <summary>
/// ジャンプ状態の更新
/// </summary>
/// <param name="player">プレイヤー</param>
/// <returns>次のステート</returns>
PlayerState * JumpingState::Update(Player & player)
{
    //入力方向への移動量の生成
    Vector3 moveV(player.GetHorizontal().x * player.GetMoveSpeed(), 0.0f, -player.GetHorizontal().z * player.GetMoveSpeed());

    //空中移動
    player.Move(moveV);

    //着地
    if (!player.GetJumpState())
    {
        AudioManager& am = singleton<AudioManager>::GetInstance();
        am.PlaySounds(CRI_PLAYSOUNDS_LANDING1, 0.8f);

        return new StandingState();
    }
    //ジャンプ攻撃
    if (player.GetLightAttackInputState())
        return new JumpAttack1State();

    //回避開始
    if (player.GetEvadeInputState())
        return new EvadeState();

    return nullptr;
}

/// <summary>
/// 走り状態の初期化
/// </summary>
/// <param name="player">プレイヤー</param>
void RunningState::Initialize(Player & player)
{
    player.ChangeAnimation(L"Run");     //走りアニメーション再生
    player.SetAttackState(false);		//攻撃判定OFF
}

/// <summary>
/// 走り状態の更新
/// </summary>
/// <param name="player">プレイヤー</param>
/// <returns>ステートの変化</returns>
PlayerState * RunningState::Update(Player & player)
{
    //足音
    m_timer += 1 / 60.0f;
    if (m_timer >= 0.25f)
    {
        AudioManager& am = singleton<AudioManager>::GetInstance();
        am.PlaySounds(CRI_PLAYSOUNDS_LANDING1, 0.8f);
        player.Footsteps();
        m_timer = 0;
    }

    //入力方向への移動量の生成
    Vector3 moveV(player.GetHorizontal().x * player.GetMoveSpeed(), 0.0f, -player.GetHorizontal().z * player.GetMoveSpeed());
    player.Move(moveV);

    //Lが離されたら歩き
    if (!player.GetRunState())
        return new WalkingState();

    //ジャンプ開始
    if (player.GetJumpState())
    {
        AudioManager& am = singleton<AudioManager>::GetInstance();
        am.PlaySounds(CRI_PLAYSOUNDS_JUMP1, 0.6f);

        player.Footsteps();
        return new JumpingState();
    }
    //移動終了時
    if (!player.IsMove())
        return new StandingState();

    //弱攻撃開始
    if (player.GetLightAttackInputState())
        return new LightAttack1State();

    //回避開始
    if (player.GetEvadeInputState())
        return new EvadeState();

    return nullptr;
}


/// <summary>
///	弱攻撃1状態の初期化
/// </summary>
/// <param name="player">プレイヤー</param>
void LightAttack1State::Initialize(Player & player)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();
    am.PlaySounds(CRI_PLAYSOUNDS_SWORD_GESTURE1);

    player.ChangeAnimation(L"Attack1");     //攻撃1アニメーション再生
    player.SetLightAttackInputState(false);	//弱攻撃終了
    player.SetAttackState(true);			//攻撃判定ON

    int weapon_damage = player.GetWeaponInfo()->GetLightAttack1Damage();
    player.SetDamage(weapon_damage);    	//弱攻撃1のダメージを設定

    //視点方向の取得
    m_move_vec = player.GetMoveVec();

    m_attack_speed = Player::ATTACK_SPEED;
}

/// <summary>
/// 弱攻撃1状態の更新
/// </summary>
/// <param name="player">プレイヤー</param>
PlayerState * LightAttack1State::Update(Player & player)
{
    //視点方向への移動量の生成
    Vector3 moveV(m_move_vec.x * m_attack_speed, 0.0f, -m_move_vec.y * m_attack_speed);
    player.SetPos(player.GetPos() + moveV);

    //ブレーキ
    m_attack_speed *= 0.9f;

    //アニメーション時間が終了したら待機状態へ
    if (!player.IsAnim(L"Attack1"))
    {
        //コンボ入力
        if (player.GetLightAttackInputState())
            return new LightAttack2State();

        return new StandingState();
    }

    //ジャンプ
    if (player.GetJumpState())
    {
        player.Footsteps();

        return new JumpingState();
    }
    //回避開始
    if (player.GetEvadeInputState())
        return new EvadeState();

    return nullptr;
}

/// <summary>
/// 弱攻撃2の初期化
/// </summary>
/// <param name="player">プレイヤー</param>
void LightAttack2State::Initialize(Player & player)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();
    am.PlaySounds(CRI_PLAYSOUNDS_SWORD_GESTURE1);

    player.ChangeAnimation(L"Attack2");     //攻撃2アニメーション再生
    player.SetLightAttackInputState(false); //弱攻撃終了
    player.SetAttackState(true);		    //攻撃判定ON

    int weapon_damage = player.GetWeaponInfo()->GetLightAttack2Damage();
    player.SetDamage(weapon_damage);	    //弱攻撃2のダメージを設定

    //視点方向の取得
    m_move_vec = player.GetMoveVec();

    m_attack_speed = Player::ATTACK_SPEED;
}

/// <summary>
/// 弱攻撃2の更新
/// </summary>
/// <param name="player">プレイヤー</param>
PlayerState * LightAttack2State::Update(Player & player)
{
    //視点方向への移動量の生成
    Vector3 moveV(m_move_vec.x * m_attack_speed, 0.0f, -m_move_vec.y * m_attack_speed);
    player.SetPos(player.GetPos() + moveV);

    //ブレーキ
    m_attack_speed *= 0.9f;

    //アニメーション時間が終了したら待機状態へ
    if (!player.IsAnim(L"Attack2"))
    {
        //コンボ入力
        if (player.GetLightAttackInputState())
            return new LightAttack3State();

        return new StandingState();
    }

    //ジャンプ開始
    if (player.GetJumpState())
    {
        player.Footsteps();
        return new JumpingState();
    }
    //回避開始
    if (player.GetEvadeInputState())
        return new EvadeState();

    return nullptr;
}

/// <summary>
/// 弱攻撃3の初期化
/// </summary>
/// <param name="player">プレイヤー</param>
void LightAttack3State::Initialize(Player & player)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();
    am.PlaySounds(CRI_PLAYSOUNDS_SWORD_GESTURE1);

    player.ChangeAnimation(L"Attack3");     //攻撃3アニメーション再生
    player.SetLightAttackInputState(false); //弱攻撃終了
    player.SetAttackState(true);		    //攻撃判定ON

    int weapon_damage = player.GetWeaponInfo()->GetLightAttack3Damage();
    player.SetDamage(weapon_damage);	    //弱攻撃3のダメージを設定

    //視点方向の取得
    m_move_vec = player.GetMoveVec();

    m_attack_speed = Player::ATTACK_SPEED;
}

/// <summary>
/// 弱攻撃3の更新
/// </summary>
/// <param name="player">プレイヤー</param>
PlayerState * LightAttack3State::Update(Player & player)
{
    //視点方向への移動量の生成
    Vector3 moveV(m_move_vec.x * m_attack_speed, 0.0f, -m_move_vec.y * m_attack_speed);
    player.SetPos(player.GetPos() + moveV);

    //ブレーキ
    m_attack_speed *= 0.9f;

    //アニメーション時間が終了したら待機状態へ
    if (!player.IsAnim(L"Attack3"))
        return new StandingState();

    //ジャンプ開始
    if (player.GetJumpState())
    {
        player.Footsteps();
        return new JumpingState();
    }
    //回避開始
    if (player.GetEvadeInputState())
        return new EvadeState();

    return nullptr;
}

/// <summary>
/// ジャンプ攻撃1の初期化
/// </summary>
void JumpAttack1State::Initialize(Player & player)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();
    am.PlaySounds(CRI_PLAYSOUNDS_SWORD_GESTURE1);

    player.ChangeAnimation(L"JumpAttack1");     //ジャンプ攻撃1アニメーション再生
    player.SetLightAttackInputState(false);     //弱攻撃終了
    player.SetAttackState(true);		        //攻撃判定ON

    int weapon_damage = player.GetWeaponInfo()->GetJumpAttack1Damage();
    player.SetDamage(weapon_damage);	        //ジャンプ攻撃1のダメージを設定

    //ちょっと上昇
    player.SetVelocity(0.5f);
}

/// <summary>
/// ジャンプ攻撃1の更新
/// </summary>
PlayerState * JumpAttack1State::Update(Player & player)
{
    //コンボ入力が無かったらそのまま落下
    if (!player.IsAnim(L"JumpAttack1"))
    {
        //空中コンボ入力
        if (player.GetLightAttackInputState())
            return new JumpAttack2State();

        return new JumpingState();
    }
    return nullptr;
}

/// <summary>
/// ジャンプ攻撃2の初期化
/// </summary>
void JumpAttack2State::Initialize(Player & player)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();
    am.PlaySounds(CRI_PLAYSOUNDS_SWORD_GESTURE1);

    player.ChangeAnimation(L"JumpAttack2");     //ジャンプ攻撃2アニメーション再生
    player.SetLightAttackInputState(false);     //弱攻撃終了
    player.SetAttackState(true);		        //攻撃判定ON

    int weapon_damage = player.GetWeaponInfo()->GetJumpAttack2Damage();
    player.SetDamage(weapon_damage);	        //ジャンプ攻撃2のダメージを設定

    //ちょっと上昇
    player.SetVelocity(0.5f);
}

/// <summary>
/// ジャンプ攻撃2の更新
/// </summary>
PlayerState * JumpAttack2State::Update(Player & player)
{

    //コンボ入力が無かったらそのまま落下
    if (!player.IsAnim(L"JumpAttack2"))
    {
        //空中コンボ入力
        if (player.GetLightAttackInputState())
            return new JumpAttack3State();

        return new JumpingState();
    }
    return nullptr;
}

/// <summary>
/// ジャンプ攻撃3の初期化
/// </summary>
void JumpAttack3State::Initialize(Player & player)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();
    am.PlaySounds(CRI_PLAYSOUNDS_SWORD_GESTURE1);

    player.ChangeAnimation(L"JumpAttack3");     //ジャンプ攻撃3アニメーション再生
    player.SetLightAttackInputState(false);     //弱攻撃終了
    player.SetAttackState(true);		        //攻撃判定ON

    int weapon_damage = player.GetWeaponInfo()->GetJumpAttack3Damage();
    player.SetDamage(weapon_damage);	        //ジャンプ攻撃3のダメージを設定

    player.SetVelocity(-0.6f);
}

/// <summary>
/// ジャンプ攻撃3の更新
/// </summary>
PlayerState * JumpAttack3State::Update(Player & player)
{
    //着地
    if (!player.GetJumpState())
        return new PushInState();

    return nullptr;
}

/// <summary>
/// 剣差し込み初期化
/// </summary>
/// <param name="player"></param>
void PushInState::Initialize(Player & player)
{
    player.ChangeAnimation(L"PushIn");
    player.SetLightAttackInputState(false);
}
PlayerState * PushInState::Update(Player & player)
{
    //もとに戻る
    if (!player.IsAnim(L"PushIn"))
        return new StandingState();

    return nullptr;
}

/// <summary>
///	回避状態の初期化
/// </summary>
void EvadeState::Initialize(Player & player)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();
    am.PlaySounds(CRI_PLAYSOUNDS_EVADE);

    player.ChangeAnimation(L"Evade");   //回避アニメーション再生
    player.SetEvadeInputState(false);   //回避入力終了
    player.SetAttackState(false);		//攻撃判定OFF
    player.SetCollisionState(false);	//当たり判定OFF

    //視点方向の取得
    m_move_vec = player.GetMoveVec();
    m_evade_speed = Player::EVADE_SPEED;
}

/// <summary>
/// 回避状態の更新
/// </summary>
PlayerState * EvadeState::Update(Player & player)
{
    Vector3 moveV(m_move_vec.x * m_evade_speed, 0.0f, -m_move_vec.y * m_evade_speed);
    player.SetPos(player.GetPos() + moveV);

    //回避が終了したら当たり判定を戻し、立ち状態へ
    if (!player.IsAnim(L"Evade"))
    {
        player.SetCollisionState(true);	//当たり判定ON

        //ジャンプ
        if (player.GetJumpState())
            return new JumpingState();

        return new StandingState();
    }
    return nullptr;
}

/// <summary>
/// 死亡状態の初期化
/// </summary>
void DeadState::Initialize(Player & player)
{
    player.ChangeAnimation(L"Dead");    //死亡アニメーション再生
    player.SetAttackState(false);		//攻撃判定OFF
    player.SetCollisionState(false);	//当たり判定OFF
}

/// <summary>
/// 死亡状態の更新
/// </summary>
PlayerState * DeadState::Update(Player & player)
{
    //死亡モーション終了
    if (!player.IsAnim(L"Dead"))
        player.Dead();

    return nullptr;
}
#pragma endregion
