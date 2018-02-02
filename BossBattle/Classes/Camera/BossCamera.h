//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:ƒ{ƒXƒJƒƒ‰ƒNƒ‰ƒX
//
//day:2017/11/29
//
//acotr:Koshida Kenji
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/

#pragma once
#include "Camera.h"
#include "..\Character\Enemy\Enemy.h"

class BossCamera :public Camera
{
public:
    //ƒ^[ƒQƒbƒg‚ÆƒJƒƒ‰‚Ì‹——£
    static const float CAMERA_DISTANCE;

    //ƒRƒ“ƒXƒgƒ‰ƒNƒ^
    BossCamera(int width, int height);

    //–ˆƒtƒŒ[ƒ€XV
    void Update()override;

    //ƒ^[ƒQƒbƒg‚Ìİ’è
    void SetTargetPos(const DirectX::SimpleMath::Vector3& targetpos) { m_target_pos = targetpos; };
    void SetTargetAngle(const DirectX::SimpleMath::Vector3& angle) { m_camera_angle = angle; };

    //ƒ{ƒXî•ñ‚Ìİ’è
    void SetBoss(Enemy* boss) { m_boss = boss; }

    //ƒJƒƒ‰‚ÌŠp“x‚ğæ“¾
    const DirectX::SimpleMath::Vector3& GetCameraAngle() { return m_camera_angle; }
private:
    DirectX::SimpleMath::Vector3 m_target_pos;      //ƒJƒƒ‰‚ÌÀ•W

    DirectX::SimpleMath::Vector3 m_camera_angle;    //ƒJƒƒ‰‚ÌŠp“x

    Enemy* m_boss;                                  //ƒ{ƒXî•ñ
};