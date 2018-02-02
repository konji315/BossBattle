#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// コンストラクタ
/// <param name="width">ウィンドウの横幅</param>
/// <param name="height">ウィンドウの縦幅</param>
/// </summary>
Camera::Camera(int width, int height)
{
    m_upvec = Vector3(0, 1.0f, 0);

    m_fovY = XMConvertToRadians(60.0f);
    m_aspect = (float)width / height;
    m_near_clip = 0.1f;
    m_far_clip = 1000.0f;
    m_shake_power = 0;
}

/// <summary>
/// 更新処理
/// </summary>
void Camera::Update()
{
    //ビュー行列生成
    m_camera_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);

    //プロジェクション行列生成
    m_camera_proj = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_near_clip, m_far_clip);

    // ビルボード行列を計算
    CalcBillboard();
}

/// <summary>
/// ビルボード行列の計算
/// </summary>
void Camera::CalcBillboard()
{
    // 視線方向
    Vector3 eyeDir = m_eyepos - m_refpos;
    // Y軸
    Vector3 Y = Vector3::UnitY;
    // X軸
    Vector3 X = Y.Cross(eyeDir);
    X.Normalize();
    // Z軸
    Vector3 Z = X.Cross(Y);
    Z.Normalize();
    // Y軸周りビルボード行列（右手系の為Z方向反転）
    m_billboard_constrainY = Matrix::Identity;
    m_billboard_constrainY.m[0][0] = X.x;
    m_billboard_constrainY.m[0][1] = X.y;
    m_billboard_constrainY.m[0][2] = X.z;
    m_billboard_constrainY.m[1][0] = Y.x;
    m_billboard_constrainY.m[1][1] = Y.y;
    m_billboard_constrainY.m[1][2] = Y.z;
    m_billboard_constrainY.m[2][0] = -Z.x;
    m_billboard_constrainY.m[2][1] = -Z.y;
    m_billboard_constrainY.m[2][2] = -Z.z;

    Y = eyeDir.Cross(X);
    Y.Normalize();
    eyeDir.Normalize();
    // ビルボード行列（右手系の為Z方向反転）
    m_billboard = Matrix::Identity;
    m_billboard.m[0][0] = X.x;
    m_billboard.m[0][1] = X.y;
    m_billboard.m[0][2] = X.z;
    m_billboard.m[1][0] = Y.x;
    m_billboard.m[1][1] = Y.y;
    m_billboard.m[1][2] = Y.z;
    m_billboard.m[2][0] = -eyeDir.x;
    m_billboard.m[2][1] = -eyeDir.y;
    m_billboard.m[2][2] = -eyeDir.z;
}