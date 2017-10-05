//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:FPS�J����
//
//day:2017/06/30
//
//acotr:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/

#pragma once
#include "Camera.h"
#include "../Player/Player.h"

class FPSCamera:public Camera
{
public:
	//�v���C���[�ƃJ�����̋���
	static const float CAMERA_DISTANCE;
	//�J�����̐��񑬓x
	static const float CAMERA_TURN_SPEED;

	FPSCamera(int width,int height);

	//���t���[���X�V
	void Update()override;

	void SetTargetPos(const DirectX::SimpleMath::Vector3& targetpos) { m_target_pos = targetpos; };
	void SetTargetAngle(const DirectX::SimpleMath::Vector3& player_angle) { m_camera_angle = player_angle; };

	void SetPlayer(Player* player) { m_player = player; };
private:
	//�J�����̍��W
	DirectX::SimpleMath::Vector3 m_target_pos;
	
	DirectX::SimpleMath::Vector3 m_camera_angle;

	float m_step_timer;

	float m_move_horizontal;

	Player* m_player;
};