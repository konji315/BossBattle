#include "Kakunon.h"
#include "../../Singleton.h"
#include "../Collider/CollisionChecker.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// �R���X�g���N�^
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
/// �f�X�g���N�^
/// </summary>
Kakunon::~Kakunon()
{
	
}

/// <summary>
/// ������
/// </summary>
void Kakunon::Initialize()
{
	m_pos = Vector3(0, 2, 0);
}

/// <summary>
/// �X�V
/// </summary>
void Kakunon::Update()
{
	m_model_enemy.SetPos(m_pos);
	m_model_enemy.Update();

	m_collision_sphere.Update();
}

/// <summary>
/// �`��
/// </summary>
void Kakunon::Render()
{
	m_model_enemy.Render();
	m_collision_sphere.Render();
}
