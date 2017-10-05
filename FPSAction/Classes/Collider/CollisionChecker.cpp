#include "CollisionChecker.h"

CollisionChecker::CollisionChecker()
{
	m_collider.resize(100);
}


CollisionChecker::~CollisionChecker()
{
}

/// <summary>
/// 当たり判定の登録
/// </summary>
/// <param name="collision"></param>
void CollisionChecker::RegisterSphereCollider(TAG tag , Sphere* collision)
{
	m_collider[tag].push_back(collision);
}

/// <summary>
/// 当たり判定の削除
/// </summary>
/// <param name="tag">タグ</param>
/// <param name="collision">判定</param>
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
/// 指定されたタグの当たり判定と衝突しているかどうか
/// </summary>
/// <param name="check_tag">タグ情報</param>
/// <param name="check_collision">判定</param>
/// <returns>衝突の有無</returns>
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