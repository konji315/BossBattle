//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:�G�̊��N���X
//
//day:2017/07/10
//
//acotr:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/

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

	const DirectX::SimpleMath::Vector3& GetPos() { return m_pos; }			//���W�̎擾

	const SphereNode& GetCollisonSphere() { return m_collision_sphere; }	//����̎擾

	void SetPos(const DirectX::SimpleMath::Vector3& pos) { m_pos = pos; }	//���W�̐ݒ�

	bool ExistsHP()															//�̗͂����邩�ǂ���
	{
		if (m_hp <= 0)
			return true;

		return false;
	}

	void Dead() { m_is_dead = true; }										//���S
	bool IsDead() { return m_is_dead; }										//�|���ꂽ���ǂ���
protected:
	Obj3d m_model_enemy;					//���f��
	SphereNode m_collision_sphere;			//�����蔻��

	DirectX::SimpleMath::Vector3 m_pos;		//���W
	DirectX::SimpleMath::Vector3 m_angle;	//�p�x

	float m_move_speed;						//�ړ����x

	int m_max_hp;							//�ő�̗�
	int m_hp;								//�̗�

	Canvas m_status_canvas;					//�X�e�[�^�X�L�����o�X

	bool m_is_dead;							//���S�t���O
};

