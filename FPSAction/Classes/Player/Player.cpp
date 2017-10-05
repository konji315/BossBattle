#include "Player.h"
#include "../../DXTKResouces.h"
#include "../Weapon/Wand.h"
#include "../Collider/CollisionChecker.h"
#include "../Manager/AudioManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

const float Player::TURN_SPEED = 3.0f;
const float Player::STEP_SPEED = 0.05f;
const float Player::WALK_SPEED = 0.5f;
const float Player::RUN_SPEED = 0.8f;
const float Player::STEP_TIME = 1.0f;
const float Player::GRAVITY_ACC = 0.03f;
const float Player::JUMP_SPEED_FIRST = 0.5f;
const float Player::JUMP_SPEED_MAX = 3.0f;
const float Player::DAMAGE_EFFECT_TIMER = 0.3f;

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player()
{
	AudioManager& am = singleton<AudioManager>::GetInstance();
	EffectManager& efk = singleton<EffectManager>::GetInstance();
	DXTK::DXTKResources& dxtk = singleton<DXTK::DXTKResources>::GetInstance();
	CollisionChecker& cc = singleton<CollisionChecker>::GetInstance();

	m_pos = Vector3(0,0,0);
	m_angle = Vector3::Zero;

	m_max_hp = 50;
	m_hp = m_max_hp;
	m_horizontal.x = 0;
	m_horizontal.z = 0;
	m_step_timer = 0;
	m_is_step = false;
	m_step_height = 0;
	m_is_jump = false;
	m_is_run = false;
	m_move_speed = 0;
	m_damage_effect_timer = DAMAGE_EFFECT_TIMER;

	m_is_lightattack = false;
	m_is_heavyattack = false;

	m_velocity = Vector3();

	m_state = new StandingState();
	m_state->Initialize(*this);

	m_weapon = new Wand();

	m_player_collision.Initialize();
	m_player_collision.SetParent(&m_tmp_player);
	m_player_collision.SetOffset(Vector3(0, 3.0f, 0));
	m_player_collision.SetLocalRadius(3.0f);
	m_player_collision.Update();

	cc.RegisterSphereCollider(CollisionChecker::PLAYER, &m_player_collision);

	m_status_canvas.SetTexture("Weapon", L"Resources/Sprites/wandmark.png");
	m_status_canvas.SetTexturePos("Weapon", Vector2(80,80));
	m_status_canvas.SetTexture("HP", L"Resources/Sprites/player_hp.png");
	m_status_canvas.SetTexturePos("HP", Vector2(280, 60));

	m_status_canvas.SetTexture("HP_bar", L"Resources/Sprites/player_hp_bar.png");
	m_status_canvas.SetTexturePos("HP_bar", Vector2(280, 60));

	m_status_canvas.SetTexture("damage_effect", L"Resources/Sprites/damage_effect.png");
	m_status_canvas.SetTexturePos("damage_effect", Vector2(512, 384));
	m_status_canvas.SetVisible("damage_effect", false);

	Initialize();
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
/// プレイヤーの初期化
/// </summary>
void Player::Initialize()
{
}

/// <summary>
/// プレイヤーの更新
/// </summary>
void Player::Update()
{
	AudioManager& am = singleton<AudioManager>::GetInstance();
	CollisionChecker& cc = singleton<CollisionChecker>::GetInstance();

	PlayerState* state = m_state->Update(*this);

	if (state != nullptr)
	{
		delete m_state;
		m_state = state;
		m_state->Initialize(*this);
	}

	//ジャンプ中
	if (m_is_jump)
	{
		m_velocity.y -= GRAVITY_ACC;
		if (m_velocity.y < -JUMP_SPEED_MAX)
			m_velocity.y = -JUMP_SPEED_MAX;
	}

	m_pos += m_velocity;
	
	//弱攻撃中
	if (m_is_lightattack)
	{
		if (m_weapon->LightAttack())
		{
			m_is_lightattack = false;
		}
	}

	//強攻撃中
	if (m_is_heavyattack)
	{
		m_weapon->HeavyAttack();
	}

	m_tmp_player.SetPos(m_pos);
	m_tmp_player.SetAngle(m_angle);

	m_tmp_player.Update();

	m_weapon->SetPlayerPos(m_pos);
	m_weapon->SetPlayerAngle(m_angle);
	m_weapon->Update();

	m_player_collision.Update();

	//敵の攻撃に当たったら
	if (cc.CheckSphereCollider(CollisionChecker::TAG::BOSS_BALL, m_player_collision))
	{
		am.PlaySounds(CRI_PLAYSOUNDS_PLAYER_HIT);

		m_status_canvas.SetVisible("damage_effect", true);

		m_hp -= 10;
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

	float width = m_status_canvas.GetDesc("HP_bar").Width;
	RECT rect = m_status_canvas.GetRect("HP_bar");
	rect.right = width * m_hp / m_max_hp;

	m_status_canvas.SetRect("HP_bar", rect);
}

/// <summary>
/// プレイヤーの描画
/// </summary>
void Player::Render()
{
	m_weapon->Render();
	m_player_collision.Render();

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
/// プレイヤーのステップ
/// </summary>
bool Player::Step(Player::Horizontal horizontal)
{
	//ステップ中ではない場合ステップ開始
	if (!m_is_step)
	{
		m_step_timer = STEP_TIME;
		m_is_step = true;
	}

	//ステップ中
	if(m_is_step)
	{
		if (m_step_timer >= 0.0f)
		{
			m_step_timer -= STEP_SPEED;

			m_step_ammo = sinf(XM_PI * (m_step_timer)) * 0.2f - m_step_height;

			//入力方向への移動量の生成
			Vector3 moveV(horizontal.x * m_move_speed, m_step_ammo, -horizontal.z * m_move_speed);

			Move(moveV);

			m_step_height += m_step_ammo;
		}
		else
		{
			m_is_step = false;
			return true;
		}
	}

	return false;
}

/// <summary>
/// 
/// プレイヤーの移動
/// </summary>
/// <param name="movev"></param>
void Player::Move(DirectX::SimpleMath::Vector3 movev)
{
	Matrix transmat = Matrix::CreateTranslation(m_pos);
	Matrix rotmatY = Matrix::CreateRotationY(m_angle.y);

	Matrix world = rotmatY * transmat;

	movev = SimpleMath::Vector3::TransformNormal(movev, world);

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
	if (!m_is_jump && !m_is_step)
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
/// 武器による弱攻撃開始
/// </summary>
void Player::StartLightAttack()
{
	m_is_lightattack = true;
}

/// <summary>
/// 武器による強攻撃開始
/// </summary>
void Player::StartHeavyAttack()
{
	m_is_heavyattack = true;
}

/// <summary>
/// 立ち状態のコンストラクタ
/// </summary>
StandingState::StandingState()
{

}

/// <summary>
/// 立ち状態の初期化
/// </summary>
/// <param name="player">プレイヤー</param>
void StandingState::Initialize(Player & player)
{

}

/// <summary>
/// 立ち状態の更新
/// </summary>
/// <param name="player">プレイヤー</param>
/// <returns>次のステート</returns>
PlayerState * StandingState::Update(Player & player)
{
	//ジャンプボタンが押されたらジャンプ
	if (player.IsJump())
		return new JumpingState();

	//移動開始
	if (player.GetHorizontal().x || player.GetHorizontal().z)
	{
		return new WalkingState();
	}

	return nullptr;
}

/// <summary>
/// 歩き状態のコンストラクタ
/// </summary>
WalkingState::WalkingState()
{

}

/// <summary>
/// 歩き状態の初期化
/// </summary>
/// <param name="player">プレイヤー</param>
void WalkingState::Initialize(Player & player)
{
	//player.Initialize();
}

/// <summary>
/// 歩き状態の更新
/// </summary>
/// <param name="player">プレイヤー</param>
/// <returns>次のステート</returns>
PlayerState * WalkingState::Update(Player & player)
{
	//入力方向にステップ

	//ジャンプボタンが押されたらジャンプ
	if (player.IsJump())
		return new JumpingState();

	//ステップ終了時
	if (player.Step(player.GetHorizontal()))
	{
		return new StandingState();
	}

	return nullptr;
}

/// <summary>
/// ジャンプ状態のコンストラクタ
/// </summary>
JumpingState::JumpingState()
{
}

/// <summary>
/// ジャンプ状態の初期化
/// </summary>
/// <param name="player">プレイヤー</param>
void JumpingState::Initialize(Player & player)
{
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

	if (!player.IsJump())
		return new StandingState();

	return nullptr;
}
