//Å`/Å`/Å`//Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/
//Content:è’ìÀîªíË
//
//day:2017/07/21
//
//acotr:Koshida Kenji
//Å`/Å`/Å`//Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/

#pragma once
#include "CollisionNode.h"
#include "../../Singleton.h"
#include <vector>

class CollisionChecker : public singleton<CollisionChecker>
{
private:
	CollisionChecker();
public:
	enum TAG
	{
		PLAYER,
		ENEMY,
		FIREBALL,
		BOSS,
		BOSS_BALL
	};
public:
	friend class singleton<CollisionChecker>;
	virtual ~CollisionChecker();

	void RegisterSphereCollider(TAG tag, Sphere* collision);
	void DeleteSphereCollider(TAG tag, Sphere* collision);

	bool CheckSphereCollider(TAG check_tag , const Sphere& check_collision);

private:
	std::vector < std::vector<Sphere*>> m_collider;
};

