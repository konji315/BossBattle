#include "Kakunon.h"
#include "../../Singleton.h"
#include "../Collider/CollisionChecker.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// コンストラクタ
/// </summary>
Kakunon::Kakunon()
{
	CollisionChecker& cc = singleton<CollisionChecker>::GetInstance();

	m_model_enemy.LoadModel(L"Resources/kakunonn.cmo");

	m_collision_sphere.Initialize();
	m_collision_sphere.SetParent(&m_model_enemy);
	m_collision_sphere.SetOffset(Vector3(0.0f, 0.0f, 0.0f));
	m_collision_sphere.SetLocalRadius(1.0f);

	cc.RegisterSphereCollider(CollisionChecker::TAG::ENEMY, &m_collision_sphere);

	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
Kakunon::~Kakunon()
{
	
}

/// <summary>
/// 初期化
/// </summary>
void Kakunon::Initialize()
{
	m_pos = Vector3(0, 2, 0);
}

/// <summary>
/// 更新
/// </summary>
void Kakunon::Update()
{
	m_model_enemy.SetPos(m_pos);
	m_model_enemy.Update();

	m_collision_sphere.Update();
}

/// <summary>
/// 描画
/// </summary>
void Kakunon::Render()
{
	m_model_enemy.Render();
	m_collision_sphere.Render();
}
