//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:����̊��N���X
//
//day:2017/07/9
//
//acotr:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/

#pragma once
#include <d3d11.h>
#include <SimpleMath.h>

#include "../../Obj3d.h"

class Weapon
{
public:
	
	Weapon() {};
	virtual ~Weapon() {};

	//������
	virtual void Initialize() = 0;

	//�X�V
	virtual void Update() = 0;
	//�`��
	virtual void Render() = 0;

	//��U��
	virtual bool LightAttack() = 0;

	//���U��
	virtual bool HeavyAttack() = 0;

	//�v���C���[�̃Z�b�g
	virtual void SetPlayerPos(const DirectX::SimpleMath::Vector3& pos) { m_player_pos = pos; };
	virtual void SetPlayerAngle(const DirectX::SimpleMath::Vector3& angle) { m_player_angle = angle; };
protected:
	Obj3d m_model_weapon;
	Obj3d m_tmp_player;

	DirectX::SimpleMath::Vector3 m_pos;
	DirectX::SimpleMath::Vector3 m_angle;

	DirectX::SimpleMath::Vector3 m_player_pos;
	DirectX::SimpleMath::Vector3 m_player_angle;
};