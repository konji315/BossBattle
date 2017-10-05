#include "FPSCamera.h"
#include "../../DXTKResouces.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

const float FPSCamera::CAMERA_DISTANCE = 7.0f;
const float FPSCamera::CAMERA_TURN_SPEED = 3.0f;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="width">画面の横幅</param>
/// <param name="height">画面の縦幅</param>
FPSCamera::FPSCamera(int width,int height)
	:Camera(width,height)
	,m_target_pos(Vector3::Zero)
	,m_camera_angle(0.0f)
	,m_move_horizontal(0.0f)
{
}

/// <summary>
/// FPSカメラの更新
/// </summary>
void FPSCamera::Update()
{
	//プレイヤーが設定されている場合
	if (m_player)
	{
		SetTargetPos(m_player->GetPos());

		SetTargetAngle(m_player->GetAngle());
	}

	Vector3 uppos;
	Vector3 eyepos, refpos;

	uppos = m_target_pos + Vector3(0, 5.0f, 0);

	Vector3 cameraV(0.0f, 0.0f, -CAMERA_DISTANCE);

	//見る位置を回転させる
	Matrix rotmatX = Matrix::CreateRotationX(m_camera_angle.x);

	Matrix rotmatY = Matrix::CreateRotationY(m_camera_angle.y);

	Matrix rotmat = rotmatX * rotmatY;

	cameraV = Vector3::TransformNormal(cameraV, rotmat);
	eyepos = uppos + cameraV * 0.1f;

	//視点座標の計算
	refpos = uppos + cameraV;

	this->SetEyeSet(eyepos);
	this->SetRefSet(refpos);

	//基底クラスの更新
	Camera::Update();
}