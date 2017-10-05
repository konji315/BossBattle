#include "BOSS.h"
#include "../Magic/BossBall.h"
#include "../Collider/CollisionChecker.h"
#include "../../Singleton.h"
#include "../Manager/AudioManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

const int Enemy::MAX_HP = 100;

const Vector3 BOSS::IDLE_MOVE_POINT(0, -2.0f, 0);
const Vector3 BOSS::START_ANGLE(0, 0, 0);
const float BOSS::SHAKE_MOVE = 0.8f;

/// <summary>
/// コンストラクタ
/// </summary>
BOSS::BOSS()
{
	m_is_dead = false;
	m_max_hp = MAX_HP;
	m_hp = m_max_hp;
	m_shake_angle = 0;
	m_death_effect = -1;

	CollisionChecker& cc = singleton<CollisionChecker>::GetInstance();

	m_pos = Vector3(0.0f, 50.0f, -50.0f);

	m_model_enemy.LoadModel(L"Resources/BOSS.cmo");

	m_model_enemy.SetPos(m_pos);
	m_model_enemy.SetAngle(m_angle);
	m_model_enemy.SetScaling(1.5f);
	m_model_enemy.Update();

	m_collision_sphere.Initialize();
	m_collision_sphere.SetParent(&m_model_enemy);
	m_collision_sphere.SetOffset(Vector3(0.0f, 0.0f, 0.0f));
	m_collision_sphere.SetLocalRadius(8.0f);
	m_collision_sphere.Update();

	m_state = new IdleState();
	m_state->Initialize(*this);

	cc.RegisterSphereCollider(CollisionChecker::BOSS, &m_collision_sphere);

	m_status_canvas.SetTexture("HP", L"Resources/Sprites/enemy_HP_bar.png");
	m_status_canvas.SetTexturePos("HP", Vector2(512, 120));
}

/// <summary>
/// デストラクタ
/// </summary>
BOSS::~BOSS()
{
	if (m_state != nullptr)
		delete m_state;

	//弾削除
	for (auto itr = m_boss_ball.begin(); itr != m_boss_ball.end(); itr++)
	{
		if (*itr != nullptr)
			delete *itr;
	}
}

/// <summary>
/// 初期化
/// </summary>
void BOSS::Initialize()
{
	m_idle_timer = 0;
	m_idle_count = 0;

	m_move_timer = 0;

	m_angle = Vector3::Zero;
}

/// <summary>
/// 更新
/// </summary>
void BOSS::Update()
{
	AudioManager& am = singleton<AudioManager>::GetInstance();
	CollisionChecker& cc = singleton<CollisionChecker>::GetInstance();

	BOSSState* state = m_state->Update(*this);

	if (state != nullptr)
	{
		delete m_state;
		m_state = state;
		m_state->Initialize(*this);
	}

	//時間によるエフェクトの削除
	for (auto itr = m_boss_ball.begin(); itr != m_boss_ball.end(); itr++)
	{
		if (*itr != nullptr)
		{
			if (!(*itr)->IsEffectExit())
			{
				cc.DeleteSphereCollider(CollisionChecker::BOSS_BALL, const_cast<SphereNode*>(&(*itr)->GetCollisonSphere()));
				delete (*itr);
				*itr = nullptr;
			}
		}
	}

	//プレイヤーにヒットしたら削除
	for (auto itr = m_boss_ball.begin(); itr != m_boss_ball.end(); itr++)
	{
		if (*itr != nullptr)
		{
			if(cc.CheckSphereCollider(CollisionChecker::PLAYER,(*itr)->GetCollisonSphere()))
			{
				cc.DeleteSphereCollider(CollisionChecker::BOSS_BALL, const_cast<SphereNode*>(&(*itr)->GetCollisonSphere()));
				delete (*itr);
				*itr = nullptr;
			}
		}
	}

	//弾の更新
	for (auto itr = m_boss_ball.begin(); itr != m_boss_ball.end(); itr++)
	{
		if(*itr != nullptr)
			(*itr)->Update();
	}

	m_model_enemy.SetPos(m_pos);
	m_model_enemy.SetAngle(m_angle);
	m_model_enemy.Update();

	m_collision_sphere.Update();

	//生きているかどうか
	if (m_hp >= 0)
	{
		//ファイアーボールに当たったら
		if (cc.CheckSphereCollider(CollisionChecker::TAG::FIREBALL, m_collision_sphere))
		{
			am.PlaySounds(CRI_PLAYSOUNDS_FIREBALL_HIT,0.3f);
			m_hp -= 5;
		}
	}

	float width = static_cast<float>(m_status_canvas.GetDesc("HP").Width);
	RECT rect = m_status_canvas.GetRect("HP");
	rect.right = static_cast<LONG>(width * m_hp / m_max_hp);

	m_status_canvas.SetRect("HP", rect);
}

/// <summary>
/// 描画
/// </summary>
void BOSS::Render()
{
	m_model_enemy.Render();
	m_collision_sphere.Render();

	for (auto itr = m_boss_ball.begin(); itr != m_boss_ball.end(); itr++)
	{
		if (*itr != nullptr)
			(*itr)->Render();
	}

	m_status_canvas.Render();
}

/// <summary>
/// 待機モーションのセット
/// </summary>
/// <param name="pos"></param>
void BOSS::SetIdlePos(const DirectX::SimpleMath::Vector3 pos1, const DirectX::SimpleMath::Vector3 pos2)
{
	m_idle_start_pos = pos1;

	m_idle_end_pos = pos2;
}

/// <summary>
/// 待機モーション
/// </summary>
/// <returns>終了かどうか</returns>
bool BOSS::Idle()
{
	if (m_idle_timer <= 1.0f)
	{
		m_idle_timer += 1 / 60.0f;
		m_pos = DirectX::XMVectorLerp(m_idle_start_pos, m_idle_end_pos, m_idle_timer);
	}
	else
	{
		if (m_idle_count >= 3)
		{
			return true;
		}

		m_idle_count++;
		m_idle_timer = 0;
		SetIdlePos(m_idle_end_pos, m_idle_start_pos);
	}

	return false;
}

/// <summary>
/// 移動ポイントの設定
/// </summary>
/// <param name="start_pos">開始地点</param>
/// <param name="end_pos">終了地点</param>
void BOSS::SetMovePos(const DirectX::SimpleMath::Vector3 & start_pos, const DirectX::SimpleMath::Vector3 & end_pos)
{
	AudioManager& am = singleton<AudioManager>::GetInstance();

	am.PlaySounds(CRI_PLAYSOUNDS_BOSS_MOVE);

	m_move_start_pos = start_pos;

	m_move_end_pos = end_pos;
}

bool BOSS::Move()
{
	if (m_move_timer <= 1.0f)
	{
		m_move_timer += 1 / 60.0f;
		m_pos = DirectX::XMVectorLerp(m_move_start_pos, m_move_end_pos, m_move_timer);
		m_angle = DirectX::XMVectorLerp(BOSS::START_ANGLE, BOSS::START_ANGLE + Vector3(0,XM_PI*2,0) , m_move_timer);
	}
	else
	{
		return true;
	}

	return false;
}

/// <summary>
/// 弾発射
/// </summary>
void BOSS::ShotBall()
{
	AudioManager& am = singleton<AudioManager>::GetInstance();

	am.PlaySounds(CRI_PLAYSOUNDS_BOSS_ATTACK1);

	int ball_num = rand() % 10 + 30;
	m_boss_ball.resize(ball_num);
	for (int i = 0; i < ball_num; i++)
	{
		m_boss_ball[i] = new BossBall(m_model_enemy.GetWorld());
	}
}

/// <summary>
/// 死亡演出
/// </summary>
bool BOSS::DeathMotion()
{
	EffectManager& efk = singleton<EffectManager>::GetInstance();

	if (m_death_effect < 0)
	{
		m_death_effect = efk.PlayEffect(EffectManager::DEATH_EXPLOSION1, m_pos);
	}
	else if (!efk.m_manager->Exists(m_death_effect))
	{
		return true;
	}

	m_shake_angle += 1;

	float shake = sinf(m_shake_angle);

	m_pos = Vector3(m_pos.x + shake, m_pos.y - 0.01f, m_pos.z);

	efk.m_manager->SetLocation(m_death_effect, m_pos.x,m_pos.y,m_pos.z);

	return false;
}

/// <summary>
/// 待機状態のコンストラクタ
/// </summary>
IdleState::IdleState()
{
}

/// <summary>
/// 待機状態の初期化
/// </summary>
/// <param name="boss">ボス</param>
void IdleState::Initialize(BOSS & boss)
{
	boss.Initialize();

	Vector3 idle_pos = boss.GetPos();

	boss.SetIdlePos(idle_pos, idle_pos + BOSS::IDLE_MOVE_POINT);
}

/// <summary>
/// 待機状態の更新
/// </summary>
/// <param name="boss">ボス</param>
/// <returns>次の状態</returns>
BOSSState * IdleState::Update(BOSS & boss)
{
	//待機モーション
	if (boss.Idle())
	{
		return new MoveState();
	}

	//死んだかどうか
	if (boss.ExistsHP())
	{
		return new DeathState();
	}

	return nullptr;
}

/// <summary>
/// 移動状態のコンストラクタ
/// </summary>
MoveState::MoveState()
{
}

/// <summary>
/// 移動状態の初期化
/// </summary>
/// <param name="boss">ボス</param>
void MoveState::Initialize(BOSS & boss)
{
	boss.Initialize();

	int angle = rand() % 361;
	int angleY = rand() % 120 + 30;

	float pos_x = cosf(XMConvertToRadians((float)angle))*(rand() % 100);
	float pos_y = sinf(XMConvertToRadians((float)angleY))*(rand() % 50 + 10);
	float pos_z = sinf(XMConvertToRadians((float)angle))*(rand() % 100);

	Vector3 end_pos = Vector3(pos_x, pos_y, pos_z);

	boss.SetMovePos(boss.GetPos(),end_pos);
}

/// <summary>
/// 移動状態の更新
/// </summary>
/// <param name="boss">ボス</param>
/// <returns>次の状態</returns>
BOSSState * MoveState::Update(BOSS & boss)
{
	//移動が終了したら
	if (boss.Move())
	{
		return new Attack1State();
	}

	//死んだかどうか
	if (boss.ExistsHP())
	{
		return new DeathState();
	}

	return nullptr;
}

/// <summary>
/// 攻撃1状態のコンストラクタ
/// </summary>
Attack1State::Attack1State()
{

}

/// <summary>
/// 攻撃1状態の初期化
/// </summary>
/// <param name="boss">ボス</param>
void Attack1State::Initialize(BOSS & boss)
{
	boss.ShotBall();
}

/// <summary>
/// 攻撃1状態の更新
/// </summary>
/// <param name="boss">ボス</param>
/// <returns>次の状態</returns>
BOSSState * Attack1State::Update(BOSS & boss)
{
	//死んだかどうか
	if (boss.ExistsHP())
	{
		return new DeathState();
	}

	return new IdleState();
}

/// <summary>
/// 攻撃2状態のコンストラクタ
/// </summary>
Attack2State::Attack2State()
{
}

/// <summary>
/// 攻撃2状態の初期化
/// </summary>
/// <param name="boss">ボス</param>
void Attack2State::Initialize(BOSS & boss)
{
}

/// <summary>
/// 攻撃2状態の更新
/// </summary>
/// <param name="boss">ボス</param>
/// <returns>次の状態</returns>
BOSSState * Attack2State::Update(BOSS & boss)
{
	//死んだかどうか
	if (boss.ExistsHP())
	{
		return new DeathState();
	}

	return nullptr;
}

/// <summary>
/// 死亡状態のコンストラクタ
/// </summary>
DeathState::DeathState()
{
}

/// <summary>
/// 死亡状態の初期化
/// </summary>
/// <param name="boss">ボス</param>
void DeathState::Initialize(BOSS & boss)
{

}

/// <summary>
/// 死亡状態の更新
/// </summary>
/// <param name="boss">ボス</param>
/// <returns>次の状態</returns>
BOSSState * DeathState::Update(BOSS & boss)
{
	EffectManager& efk = singleton<EffectManager>::GetInstance();
	//撃破演出
	if (boss.DeathMotion())
	{
		boss.Dead();

		efk.PlayEffect(EffectManager::DEATH_EXPLOSION2, boss.GetPos());
	}

	return nullptr;
}
