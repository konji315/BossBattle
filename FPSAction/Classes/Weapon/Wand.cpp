#include "Wand.h"
#include "../Magic/FireBall.h"
#include "../Collider/CollisionChecker.h"
#include "../../Singleton.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

const Vector3 Wand::FIRST_ANGLE = Vector3(-0.4f, 0, 0);
const Vector3 Wand::FIRST_POS = Vector3(0.3f, 4.2f, -2.0f);

const Vector3 Wand::LIGHT_ATTACK_PATH[4] =
{
	Vector3(0,0,0),
	Vector3(0.5f, 0.2f, 0.3f),
	Vector3(-0.5f, -0.2f, -0.3f),
	Vector3(0,0,0)
};

const Vector3 Wand::LIGHT_ATTACK_ANGLE[4] =
{
	Vector3(0,0,0),
	Vector3(0.5f, 0, 0),
	Vector3(-0.5f, 0, 0),
	Vector3(0,0,0)
};

/// <summary>
/// 杖のコンストラクタ
/// </summary>
Wand::Wand()
{
	m_fire_ball = nullptr;

	m_pos = FIRST_POS;
	m_angle = FIRST_ANGLE;

	m_model_weapon.LoadModel(L"Resources/Wand.cmo");

	m_model_weapon.SetPos(m_pos);
	m_model_weapon.SetAngle(m_angle);

	m_model_weapon.SetParent(&m_tmp_player);

	Initialize();
}

/// <summary>
/// 杖のデストラクタ
/// </summary>
Wand::~Wand()
{
	if (m_fire_ball != nullptr)
		delete m_fire_ball;
}

/// <summary>
/// 杖の初期化
/// </summary>
void Wand::Initialize()
{
	m_lerp_time = 0;
	m_path_num = 0;
}

/// <summary>
/// 杖の更新
/// </summary>
void Wand::Update()
{
	if (m_fire_ball != nullptr)
		m_fire_ball->Update();

	m_tmp_player.SetPos(m_player_pos);
	m_tmp_player.SetAngle(m_player_angle);

	m_tmp_player.Update();

	m_model_weapon.SetPos(m_pos);
	m_model_weapon.SetAngle(m_angle);
	m_model_weapon.Update();
}

/// <summary>
/// 描画
/// </summary>
void Wand::Render()
{
	m_model_weapon.Render();

	if (m_fire_ball != nullptr)
		m_fire_ball->Render();
}

/// <summary>
/// 杖の弱攻撃
/// </summary>
bool Wand::LightAttack()
{
	CollisionChecker& cc = singleton<CollisionChecker>::GetInstance();
	EffectManager& efk = singleton<EffectManager>::GetInstance();

	//エフェクトが消えたら
	if (m_fire_ball != nullptr)
	{
		if (!m_fire_ball->IsEffectExit())
		{
			cc.DeleteSphereCollider(CollisionChecker::FIREBALL, const_cast<SphereNode*>(&(m_fire_ball->GetCollisonSphere())));
			DestroyFireBall();
			return true;
		}
	}

	//ボスにあたったらヒットエフェクト
	if (m_fire_ball != nullptr)
	{
		if (cc.CheckSphereCollider(CollisionChecker::BOSS, m_fire_ball->GetCollisonSphere()))
		{
			cc.DeleteSphereCollider(CollisionChecker::FIREBALL, const_cast<SphereNode*>(&(m_fire_ball->GetCollisonSphere())));
			efk.PlayEffect(EffectManager::FIRE_HIT, m_fire_ball->GetPos());
			DestroyFireBall();
			return true;
		}
	}

	//指定パス間の移動
	if (m_lerp_time <= 1)
	{
		m_lerp_time += 1 / 12.0f;
		m_pos = DirectX::XMVectorLerp(FIRST_POS + LIGHT_ATTACK_PATH[m_path_num], FIRST_POS + LIGHT_ATTACK_PATH[m_path_num + 1], m_lerp_time);
		m_angle = DirectX::XMVectorLerp(FIRST_ANGLE + LIGHT_ATTACK_ANGLE[m_path_num], FIRST_ANGLE + LIGHT_ATTACK_ANGLE[m_path_num + 1], m_lerp_time);
	}
	else if (m_path_num < 2)
	{
		m_lerp_time = 0;
		m_path_num++;

		//振った瞬間にファイアーボール発射
		if (m_path_num == 1)
		{
			if (m_fire_ball == nullptr)
			{
				m_fire_ball = new FireBall(m_tmp_player.GetWorld());
			}
		}
	}
	return false;
}

/// <summary>
/// 杖の強攻撃
/// </summary>
bool Wand::HeavyAttack()
{
	return false;
}

//ファイアーボール消去
void Wand::DestroyFireBall()
{
	m_fire_ball->StopEffect();
	delete m_fire_ball;
	m_fire_ball = nullptr;
	Initialize();
}
