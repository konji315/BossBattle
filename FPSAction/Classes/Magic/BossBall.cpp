#include "BossBall.h"
#include "../../Singleton.h"
#include "../Collider/CollisionChecker.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

const float BossBall::MOVE_SPEED = 0.5f;

float GetRandF(float val) {
	return (float)(rand() % (1000000)) / 1000000.0f*val;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossBall::BossBall(const DirectX::SimpleMath::Matrix& mat)
{
	EffectManager& efk = singleton<EffectManager>::GetInstance();
	CollisionChecker& cc = singleton<CollisionChecker>::GetInstance();

	Matrix wand_world = mat;

	Vector3 scale;
	Quaternion rotq;
	Vector3 pos;

	wand_world.Decompose(scale, rotq, pos);

	//�ړ������x�N�g���̎Z�o
	double x_vec = ((float)rand() / 16383.5 - 1.0f) * MOVE_SPEED;
	double y_vec = ((float)rand() / 16383.5 - 1.0f) * MOVE_SPEED;
	double z_vec = ((float)rand() / 16383.5 - 1.0f) * MOVE_SPEED;

	m_move_vec = Vector3(x_vec, y_vec, z_vec);

	m_move_vec.Normalize();

	//���ˊJ�n�ꏊ�̐ݒ�
	m_obj_effect.SetPos(pos);

	//�G�t�F�N�g�Đ�
	m_effect_handle = efk.PlayEffect(EffectManager::EffectID::BOSS_BALL, m_obj_effect.GetPos());

	m_collision_sphere.Initialize();
	m_collision_sphere.SetParent(&m_obj_effect);
	m_collision_sphere.SetOffset(Vector3(0, 0, 0));
	m_collision_sphere.SetLocalRadius(4.0f);
	m_collision_sphere.Update();

	cc.RegisterSphereCollider(CollisionChecker::TAG::BOSS_BALL, &m_collision_sphere);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossBall::~BossBall()
{
}

/// <summary>
/// �X�V
/// </summary>
void BossBall::Update()
{
	EffectManager& efk = singleton<EffectManager>::GetInstance();

	//�ړ������ֈړ�
	m_obj_effect.SetPos(m_obj_effect.GetPos() + m_move_vec);

	m_obj_effect.Update();

	Vector3 efk_pos = m_obj_effect.GetPos();

	efk.m_manager->SetLocation(m_effect_handle, efk_pos.x, efk_pos.y, efk_pos.z);

	m_collision_sphere.Update();
}

/// <summary>
/// �`��
/// </summary>
void BossBall::Render()
{
	m_collision_sphere.Render();
}
