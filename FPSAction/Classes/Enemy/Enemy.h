//〜/〜/〜//〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/
//Content:敵の基底クラス
//
//day:2017/07/10
//
//acotr:Koshida Kenji
//〜/〜/〜//〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/

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

	const DirectX::SimpleMath::Vector3& GetPos() { return m_pos; }			//座標の取得

	const SphereNode& GetCollisonSphere() { return m_collision_sphere; }	//判定の取得

	void SetPos(const DirectX::SimpleMath::Vector3& pos) { m_pos = pos; }	//座標の設定

	bool ExistsHP()															//体力があるかどうか
	{
		if (m_hp <= 0)
			return true;

		return false;
	}

	void Dead() { m_is_dead = true; }										//死亡
	bool IsDead() { return m_is_dead; }										//倒されたかどうか
protected:
	Obj3d m_model_enemy;					//モデル
	SphereNode m_collision_sphere;			//当たり判定

	DirectX::SimpleMath::Vector3 m_pos;		//座標
	DirectX::SimpleMath::Vector3 m_angle;	//角度

	float m_move_speed;						//移動速度

	int m_max_hp;							//最大体力
	int m_hp;								//体力

	Canvas m_status_canvas;					//ステータスキャンバス

	bool m_is_dead;							//死亡フラグ
};

