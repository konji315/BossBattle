//Å`/Å`/Å`//Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/
//Content:èÒÇÃÉNÉâÉX
//
//day:2017/07/19
//
//acotr:Koshida Kenji
//Å`/Å`/Å`//Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/

#pragma once
#include "Weapon.h"
#include "../Magic/Magic.h"

class Wand : public Weapon
{
	const static DirectX::SimpleMath::Vector3 FIRST_ANGLE;
	const static DirectX::SimpleMath::Vector3 FIRST_POS;

	const static DirectX::SimpleMath::Vector3 LIGHT_ATTACK_PATH[4];
	const static DirectX::SimpleMath::Vector3 LIGHT_ATTACK_ANGLE[4];

public:
	Wand();
	virtual ~Wand();

	void Initialize()override;
	void Update()override;
	void Render()override;

	bool LightAttack()override;
	bool HeavyAttack()override;

	void DestroyFireBall();
private:
	Magic* m_fire_ball;

	float m_lerp_time;

	int m_path_num;
};