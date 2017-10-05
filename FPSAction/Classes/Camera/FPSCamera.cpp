#include "FPSCamera.h"
#include "../../DXTKResouces.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

const float FPSCamera::CAMERA_DISTANCE = 7.0f;
const float FPSCamera::CAMERA_TURN_SPEED = 3.0f;

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="width">��ʂ̉���</param>
/// <param name="height">��ʂ̏c��</param>
FPSCamera::FPSCamera(int width,int height)
	:Camera(width,height)
	,m_target_pos(Vector3::Zero)
	,m_camera_angle(0.0f)
	,m_move_horizontal(0.0f)
{
}

/// <summary>
/// FPS�J�����̍X�V
/// </summary>
void FPSCamera::Update()
{
	//�v���C���[���ݒ肳��Ă���ꍇ
	if (m_player)
	{
		SetTargetPos(m_player->GetPos());

		SetTargetAngle(m_player->GetAngle());
	}

	Vector3 uppos;
	Vector3 eyepos, refpos;

	uppos = m_target_pos + Vector3(0, 5.0f, 0);

	Vector3 cameraV(0.0f, 0.0f, -CAMERA_DISTANCE);

	//����ʒu����]������
	Matrix rotmatX = Matrix::CreateRotationX(m_camera_angle.x);

	Matrix rotmatY = Matrix::CreateRotationY(m_camera_angle.y);

	Matrix rotmat = rotmatX * rotmatY;

	cameraV = Vector3::TransformNormal(cameraV, rotmat);
	eyepos = uppos + cameraV * 0.1f;

	//���_���W�̌v�Z
	refpos = uppos + cameraV;

	this->SetEyeSet(eyepos);
	this->SetRefSet(refpos);

	//���N���X�̍X�V
	Camera::Update();
}