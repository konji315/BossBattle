#include "CollisionChecker.h"

CollisionChecker::CollisionChecker()
{
	m_collider.resize(100);
}


CollisionChecker::~CollisionChecker()
{
}

/// <summary>
/// �����蔻��̓o�^
/// </summary>
/// <param name="collision"></param>
void CollisionChecker::RegisterSphereCollider(TAG tag , Sphere* collision)
{
	m_collider[tag].push_back(collision);
}

/// <summary>
/// �����蔻��̍폜
/// </summary>
/// <param name="tag">�^�O</param>
/// <param name="collision">����</param>
void CollisionChecker::DeleteSphereCollider(TAG tag,Sphere* collision)
{
	for (auto itr = m_collider[tag].begin(); itr != m_collider[tag].end();)
	{
		if (*itr == collision)
		{
			itr = m_collider[tag].erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

/// <summary>
/// �w�肳�ꂽ�^�O�̓����蔻��ƏՓ˂��Ă��邩�ǂ���
/// </summary>
/// <param name="check_tag">�^�O���</param>
/// <param name="check_collision">����</param>
/// <returns>�Փ˂̗L��</returns>
bool CollisionChecker::CheckSphereCollider(TAG check_tag, const Sphere& check_collision)
{
	for (auto itr = m_collider[check_tag].begin(); itr != m_collider[check_tag].end(); itr++)
	{
		if (CheckSphere2Sphere(**itr, check_collision))
		{
			return true;
		}
	}
	return false;
}