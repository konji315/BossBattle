#include "TPSCamera.h"
#include "..\Common\DXTKResouces.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

const float TPSCamera::CAMERA_DISTANCE = 15.0f;     //プレイヤーとカメラの距離
const float TPSCamera::CAMERA_TURN_SPEED = 3.0f;    //カメラの旋回速度

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="width">画面の横幅</param>
/// <param name="height">画面の縦幅</param>
TPSCamera::TPSCamera(int width, int height)
    :Camera(width, height)
    , m_target_pos(Vector3::Zero)
    , m_camera_angle(0.0f)
{
}

/// <summary>
/// FPSカメラの更新
/// </summary>
void TPSCamera::Update()
{
    //プレイヤーが設定されている場合
    if (m_player)
    {
        SetTargetPos(m_player->GetPos());

        SetTargetAngle(m_player->GetCameraRotate());
    }

    Vector3 uppos;
    Vector3 eyepos, refpos;

    // プレイヤーの上空２ｍを参照点とする
    refpos = m_target_pos + Vector3(0, 2, 0);
    // 参照点と視点の差分ベクトル
    Vector3 cameraV(0, 5, CAMERA_DISTANCE);
    // プレイヤーの後ろに回り込むための回転行列を生成
    Matrix rotmat = Matrix::CreateRotationX(m_camera_angle.x) * Matrix::CreateRotationY(m_camera_angle.y);
    // 差分ベクトルを回転させる
    cameraV = Vector3::TransformNormal(cameraV, rotmat);
    // 視点座標を計算
    eyepos = refpos + cameraV;
    // 視点を現在位置から補間する
    eyepos = m_eyepos + (eyepos - m_eyepos) * 0.05f;
    // 参照点を現在位置から補間する
    refpos = m_refpos + (refpos - m_refpos) * 0.2f;

    this->SetEyePos(eyepos);
    this->SetRefPos(refpos);

    //基底クラスの更新
    Camera::Update();
}