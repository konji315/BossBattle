//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:“G‚ÌŠî’êƒNƒ‰ƒX
//
//day:2017/07/10
//
//acotr:Koshida Kenji
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/

#pragma once
#include <d3d11.h>
#include <SimpleMath.h>

#include "../../Obj3d.h"

#include "../Collider/CollisionNode.h"

#include "../Texture/Canvas.h"

class Enemy
{
public :
	static const int MAX_HP;

public:
	Enemy() {};
	virtual ~Enemy() {};
	
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render()= 0;

	const DirectX::SimpleMath::Vector3& GetPos() { return m_pos; }			//À•W‚Ìæ“¾

	const SphereNode& GetCollisonSphere() { return m_collision_sphere; }	//”»’è‚Ìæ“¾

	void SetPos(const DirectX::SimpleMath::Vector3& pos) { m_pos = pos; }	//À•W‚Ìİ’è

	bool ExistsHP()															//‘Ì—Í‚ª‚ ‚é‚©‚Ç‚¤‚©
	{
		if (m_hp <= 0)
			return true;

		return false;
	}

	void Dead() { m_is_dead = true; }										//€–S
	bool IsDead() { return m_is_dead; }										//“|‚³‚ê‚½‚©‚Ç‚¤‚©
protected:
	Obj3d m_model_enemy;					//ƒ‚ƒfƒ‹
	SphereNode m_collision_sphere;			//“–‚½‚è”»’è

	DirectX::SimpleMath::Vector3 m_pos;		//À•W
	DirectX::SimpleMath::Vector3 m_angle;	//Šp“x

	float m_move_speed;						//ˆÚ“®‘¬“x

	int m_max_hp;							//Å‘å‘Ì—Í
	int m_hp;								//‘Ì—Í

	Canvas m_status_canvas;					//ƒXƒe[ƒ^ƒXƒLƒƒƒ“ƒoƒX

	bool m_is_dead;							//€–Sƒtƒ‰ƒO
};

