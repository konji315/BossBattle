#include "BossCamera.h"
#include "..\Common\DXTKResouces.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//ターゲットとカメラの距離
const float BossCamera::CAMERA_DISTANCE = 50.0f;

/// <summary>
/// ボスカメラのコンストラクタ
/// </summary>
/// <param name="width">画面の横幅</param>
/// <param name="height">画面の縦幅</param>
BossCamera::BossCamera(int width, int height)
    :Camera(width, height)
    , m_target_pos(Vector3::Zero)
    , m_camera_angle(0.0f)
    , m_boss(nullptr)
{
}

/// <summary>
/// ボスカメラの更新
/// </summary>
void BossCamera::Update()
{
    //プレイヤーが設定されている場合
    if (m_boss)
    {
        auto player = m_boss->GetPlayerInfo();

        if (player)
        {
            Vector3 player_pos = m_boss->GetPlayerInfo()->GetPos();

            SetTargetPos(player_pos);

            float radian = atan2(player_pos.z, player_pos.x);
            SetTargetAngle(Vector3(0, -radian, 0));
        }
    }

    Vector3 uppos;
    Vector3 eyepos, refpos;

    //ターゲットの上空10ｍを参照点とする
    refpos = m_target_pos + Vector3(0, 10, 0);

    //参照点と視点の差分ベクトル
    Vector3 cameraV(0, 0, CAMERA_DISTANCE);

    //カメラを揺らす
    if (m_shake_power > 0)
    {
        Vector3 power;
        power.x = rand() % ((m_shake_power + 1) * 2) / 1000.0f - (m_shake_power + 1) / 1000.0f;
        power.y = rand() % ((m_shake_power + 1) * 2) / 1000.0f - (m_shake_power + 1) / 1000.0f;
        power.z = rand() % ((m_shake_power + 1) * 2) / 1000.0f - (m_shake_power + 1) / 1000.0f;

        cameraV += power;

        m_shake_power *= 0.98f;
    }

    //回転行列を生成
    Matrix rotmat = Matrix::CreateRotationY(m_camera_angle.y + XMConvertToRadians(90));
    //差分ベクトルを回転させる
    cameraV = Vector3::TransformNormal(cameraV, rotmat);
    //視点座標を計算
    eyepos = refpos + cameraV;
    //視点を現在位置から補間する
    eyepos = m_eyepos + (eyepos - m_eyepos) * 0.1f;
    //参照点を現在位置から補間する
    refpos = m_refpos + (refpos - m_refpos) * 0.2f;

    this->SetEyePos(eyepos);
    this->SetRefPos(refpos);

    //基底クラスの更新
    Camera::Update();
}