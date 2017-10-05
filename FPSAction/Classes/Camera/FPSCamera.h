//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:FPSƒJƒƒ‰
//
//day:2017/06/30
//
//acotr:Koshida Kenji
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/

#pragma once
#include "Camera.h"
#include "../Player/Player.h"

class FPSCamera:public Camera
{
public:
	//ƒvƒŒƒCƒ„[‚ÆƒJƒƒ‰‚Ì‹——£
	static const float CAMERA_DISTANCE;
	//ƒJƒƒ‰‚Ìù‰ñ‘¬“x
	static const float CAMERA_TURN_SPEED;

	FPSCamera(int width,int height);

	//–ˆƒtƒŒ[ƒ€XV
	void Update()override;

	void SetTargetPos(const DirectX::SimpleMath::Vector3& targetpos) { m_target_pos = targetpos; };
	void SetTargetAngle(const DirectX::SimpleMath::Vector3& player_angle) { m_camera_angle = player_angle; };

	void SetPlayer(Player* player) { m_player = player; };
private:
	//ƒJƒƒ‰‚ÌÀ•W
	DirectX::SimpleMath::Vector3 m_target_pos;
	
	DirectX::SimpleMath::Vector3 m_camera_angle;

	float m_step_timer;

	float m_move_horizontal;

	Player* m_player;
};