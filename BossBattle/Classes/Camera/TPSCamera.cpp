#include "TPSCamera.h"
#include "..\Common\DXTKResouces.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

const float TPSCamera::CAMERA_DISTANCE = 15.0f;     //�v���C���[�ƃJ�����̋���
const float TPSCamera::CAMERA_TURN_SPEED = 3.0f;    //�J�����̐��񑬓x

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="width">��ʂ̉���</param>
/// <param name="height">��ʂ̏c��</param>
TPSCamera::TPSCamera(int width, int height)
    :Camera(width, height)
    , m_target_pos(Vector3::Zero)
    , m_camera_angle(0.0f)
{
}

/// <summary>
/// FPS�J�����̍X�V
/// </summary>
void TPSCamera::Update()
{
    //�v���C���[���ݒ肳��Ă���ꍇ
    if (m_player)
    {
        SetTargetPos(m_player->GetPos());

        SetTargetAngle(m_player->GetCameraRotate());
    }

    Vector3 uppos;
    Vector3 eyepos, refpos;

    // �v���C���[�̏��Q�����Q�Ɠ_�Ƃ���
    refpos = m_target_pos + Vector3(0, 2, 0);
    // �Q�Ɠ_�Ǝ��_�̍����x�N�g��
    Vector3 cameraV(0, 5, CAMERA_DISTANCE);
    // �v���C���[�̌��ɉ�荞�ނ��߂̉�]�s��𐶐�
    Matrix rotmat = Matrix::CreateRotationX(m_camera_angle.x) * Matrix::CreateRotationY(m_camera_angle.y);
    // �����x�N�g������]������
    cameraV = Vector3::TransformNormal(cameraV, rotmat);
    // ���_���W���v�Z
    eyepos = refpos + cameraV;
    // ���_�����݈ʒu�����Ԃ���
    eyepos = m_eyepos + (eyepos - m_eyepos) * 0.05f;
    // �Q�Ɠ_�����݈ʒu�����Ԃ���
    refpos = m_refpos + (refpos - m_refpos) * 0.2f;

    this->SetEyePos(eyepos);
    this->SetRefPos(refpos);

    //���N���X�̍X�V
    Camera::Update();
}