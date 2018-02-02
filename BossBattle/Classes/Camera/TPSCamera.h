//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:TPSƒJƒƒ‰
//
//day:2017/06/30
//
//acotr:Koshida Kenji
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/

#pragma once
#include "Camera.h"
#include "..\Character\Player\Player.h"

class TPSCamera : public Camera
{
public:
    //ƒvƒŒƒCƒ„[‚ÆƒJƒƒ‰‚Ì‹——£
    static const float CAMERA_DISTANCE;
    //ƒJƒƒ‰‚Ìù‰ñ‘¬“x
    static const float CAMERA_TURN_SPEED;

    //ƒRƒ“ƒXƒgƒ‰ƒNƒ^
    TPSCamera(int width, int height);

    //–ˆƒtƒŒ[ƒ€XV
    void Update()override;

    //ƒ^[ƒQƒbƒg‚Ìİ’è
    void SetTargetPos(const DirectX::SimpleMath::Vector3& targetpos) { m_target_pos = targetpos; };
    void SetTargetAngle(const DirectX::SimpleMath::Vector3& player_angle) { m_camera_angle = player_angle; };

    //ƒvƒŒƒCƒ„[î•ñ‚Ìİ’è
    void SetPlayer(Player* player) { m_player = player; };
private:
    DirectX::SimpleMath::Vector3 m_target_pos;      //ƒJƒƒ‰‚ÌÀ•W
    DirectX::SimpleMath::Vector3 m_camera_angle;    //ƒJƒƒ‰‚ÌŠp“x

    Player* m_player;   //ƒvƒŒƒCƒ„[î•ñ
};