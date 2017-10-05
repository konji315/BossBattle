#include "FireBall.h"
#include "../../Singleton.h"
#include "../Collider/CollisionChecker.h"
#include "../Manager/AudioManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

const float FireBall::MOVE_SPEED = 1.5f;

FireBall::FireBall(const DirectX::SimpleMath::Matrix & mat)
{
	AudioManager& am = singleton<AudioManager>::GetInstance();
	EffectManager& efk = singleton<EffectManager>::GetInstance();
	CollisionChecker& cc = singleton<CollisionChecker>::GetInstance();

	am.PlaySounds(CRI_PLAYSOUNDS_FIREBALL1);

	Matrix wand_world = mat;

	Vector3 scale;
	Quaternion rotq;
	Vector3 pos;

	wand_world.Decompose(scale, rotq, pos);

	//�ړ������x�N�g���̎Z�o
	m_attack_vec = Vector3(0, 0, -MOVE_SPEED);
	m_attack_vec = Vector3::Transform(m_attack_vec, rotq);

	//���ˊJ�n�ꏊ�̐ݒ�
	m_obj_effect.SetPos(pos);

	//�G�t�F�N�g�Đ�
	m_effect_handle = efk.PlayEffect(EffectManager::EffectID::FIREBALL, m_obj_effect.GetPos());

	m_collision_sphere.Initialize();
	m_collision_sphere.SetParent(&m_obj_effect);
	m_collision_sphere.SetOffset(Vector3(0, 0, 0));
	m_collision_sphere.SetLocalRadius(2.0f);
	m_collision_sphere.Update();

	cc.RegisterSphereCollider(CollisionChecker::TAG::FIREBALL, &m_collision_sphere);
}

FireBall::~FireBall()
{

}

/// <summary>
/// �X�V
/// </summary>
void FireBall::Update()
{
	EffectManager& efk = singleton<EffectManager>::GetInstance();

	//�ړ������ֈړ�
	m_obj_effect.SetPos(m_obj_effect.GetPos() + m_attack_vec);

	m_obj_effect.Update();

	Vector3 efk_pos = m_obj_effect.GetPos();

	efk.m_manager->SetLocation(m_effect_handle, efk_pos.x,efk_pos.y,efk_pos.z);

	m_collision_sphere.Update();
}

/// <summary>
/// �`��
/// </summary>
void FireBall::Render()
{
	m_collision_sphere.Render();
}
