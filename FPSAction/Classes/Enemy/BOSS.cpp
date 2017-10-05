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
/// �R���X�g���N�^
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
/// �f�X�g���N�^
/// </summary>
BOSS::~BOSS()
{
	if (m_state != nullptr)
		delete m_state;

	//�e�폜
	for (auto itr = m_boss_ball.begin(); itr != m_boss_ball.end(); itr++)
	{
		if (*itr != nullptr)
			delete *itr;
	}
}

/// <summary>
/// ������
/// </summary>
void BOSS::Initialize()
{
	m_idle_timer = 0;
	m_idle_count = 0;

	m_move_timer = 0;

	m_angle = Vector3::Zero;
}

/// <summary>
/// �X�V
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

	//���Ԃɂ��G�t�F�N�g�̍폜
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

	//�v���C���[�Ƀq�b�g������폜
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

	//�e�̍X�V
	for (auto itr = m_boss_ball.begin(); itr != m_boss_ball.end(); itr++)
	{
		if(*itr != nullptr)
			(*itr)->Update();
	}

	m_model_enemy.SetPos(m_pos);
	m_model_enemy.SetAngle(m_angle);
	m_model_enemy.Update();

	m_collision_sphere.Update();

	//�����Ă��邩�ǂ���
	if (m_hp >= 0)
	{
		//�t�@�C�A�[�{�[���ɓ���������
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
/// �`��
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
/// �ҋ@���[�V�����̃Z�b�g
/// </summary>
/// <param name="pos"></param>
void BOSS::SetIdlePos(const DirectX::SimpleMath::Vector3 pos1, const DirectX::SimpleMath::Vector3 pos2)
{
	m_idle_start_pos = pos1;

	m_idle_end_pos = pos2;
}

/// <summary>
/// �ҋ@���[�V����
/// </summary>
/// <returns>�I�����ǂ���</returns>
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
/// �ړ��|�C���g�̐ݒ�
/// </summary>
/// <param name="start_pos">�J�n�n�_</param>
/// <param name="end_pos">�I���n�_</param>
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
/// �e����
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
/// ���S���o
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
/// �ҋ@��Ԃ̃R���X�g���N�^
/// </summary>
IdleState::IdleState()
{
}

/// <summary>
/// �ҋ@��Ԃ̏�����
/// </summary>
/// <param name="boss">�{�X</param>
void IdleState::Initialize(BOSS & boss)
{
	boss.Initialize();

	Vector3 idle_pos = boss.GetPos();

	boss.SetIdlePos(idle_pos, idle_pos + BOSS::IDLE_MOVE_POINT);
}

/// <summary>
/// �ҋ@��Ԃ̍X�V
/// </summary>
/// <param name="boss">�{�X</param>
/// <returns>���̏��</returns>
BOSSState * IdleState::Update(BOSS & boss)
{
	//�ҋ@���[�V����
	if (boss.Idle())
	{
		return new MoveState();
	}

	//���񂾂��ǂ���
	if (boss.ExistsHP())
	{
		return new DeathState();
	}

	return nullptr;
}

/// <summary>
/// �ړ���Ԃ̃R���X�g���N�^
/// </summary>
MoveState::MoveState()
{
}

/// <summary>
/// �ړ���Ԃ̏�����
/// </summary>
/// <param name="boss">�{�X</param>
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
/// �ړ���Ԃ̍X�V
/// </summary>
/// <param name="boss">�{�X</param>
/// <returns>���̏��</returns>
BOSSState * MoveState::Update(BOSS & boss)
{
	//�ړ����I��������
	if (boss.Move())
	{
		return new Attack1State();
	}

	//���񂾂��ǂ���
	if (boss.ExistsHP())
	{
		return new DeathState();
	}

	return nullptr;
}

/// <summary>
/// �U��1��Ԃ̃R���X�g���N�^
/// </summary>
Attack1State::Attack1State()
{

}

/// <summary>
/// �U��1��Ԃ̏�����
/// </summary>
/// <param name="boss">�{�X</param>
void Attack1State::Initialize(BOSS & boss)
{
	boss.ShotBall();
}

/// <summary>
/// �U��1��Ԃ̍X�V
/// </summary>
/// <param name="boss">�{�X</param>
/// <returns>���̏��</returns>
BOSSState * Attack1State::Update(BOSS & boss)
{
	//���񂾂��ǂ���
	if (boss.ExistsHP())
	{
		return new DeathState();
	}

	return new IdleState();
}

/// <summary>
/// �U��2��Ԃ̃R���X�g���N�^
/// </summary>
Attack2State::Attack2State()
{
}

/// <summary>
/// �U��2��Ԃ̏�����
/// </summary>
/// <param name="boss">�{�X</param>
void Attack2State::Initialize(BOSS & boss)
{
}

/// <summary>
/// �U��2��Ԃ̍X�V
/// </summary>
/// <param name="boss">�{�X</param>
/// <returns>���̏��</returns>
BOSSState * Attack2State::Update(BOSS & boss)
{
	//���񂾂��ǂ���
	if (boss.ExistsHP())
	{
		return new DeathState();
	}

	return nullptr;
}

/// <summary>
/// ���S��Ԃ̃R���X�g���N�^
/// </summary>
DeathState::DeathState()
{
}

/// <summary>
/// ���S��Ԃ̏�����
/// </summary>
/// <param name="boss">�{�X</param>
void DeathState::Initialize(BOSS & boss)
{

}

/// <summary>
/// ���S��Ԃ̍X�V
/// </summary>
/// <param name="boss">�{�X</param>
/// <returns>���̏��</returns>
BOSSState * DeathState::Update(BOSS & boss)
{
	EffectManager& efk = singleton<EffectManager>::GetInstance();
	//���j���o
	if (boss.DeathMotion())
	{
		boss.Dead();

		efk.PlayEffect(EffectManager::DEATH_EXPLOSION2, boss.GetPos());
	}

	return nullptr;
}
