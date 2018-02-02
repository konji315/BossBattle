#include "BossCamera.h"
#include "..\Common\DXTKResouces.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//�^�[�Q�b�g�ƃJ�����̋���
const float BossCamera::CAMERA_DISTANCE = 50.0f;

/// <summary>
/// �{�X�J�����̃R���X�g���N�^
/// </summary>
/// <param name="width">��ʂ̉���</param>
/// <param name="height">��ʂ̏c��</param>
BossCamera::BossCamera(int width, int height)
    :Camera(width, height)
    , m_target_pos(Vector3::Zero)
    , m_camera_angle(0.0f)
    , m_boss(nullptr)
{
}

/// <summary>
/// �{�X�J�����̍X�V
/// </summary>
void BossCamera::Update()
{
    //�v���C���[���ݒ肳��Ă���ꍇ
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

    //�^�[�Q�b�g�̏��10�����Q�Ɠ_�Ƃ���
    refpos = m_target_pos + Vector3(0, 10, 0);

    //�Q�Ɠ_�Ǝ��_�̍����x�N�g��
    Vector3 cameraV(0, 0, CAMERA_DISTANCE);

    //�J������h�炷
    if (m_shake_power > 0)
    {
        Vector3 power;
        power.x = rand() % ((m_shake_power + 1) * 2) / 1000.0f - (m_shake_power + 1) / 1000.0f;
        power.y = rand() % ((m_shake_power + 1) * 2) / 1000.0f - (m_shake_power + 1) / 1000.0f;
        power.z = rand() % ((m_shake_power + 1) * 2) / 1000.0f - (m_shake_power + 1) / 1000.0f;

        cameraV += power;

        m_shake_power *= 0.98f;
    }

    //��]�s��𐶐�
    Matrix rotmat = Matrix::CreateRotationY(m_camera_angle.y + XMConvertToRadians(90));
    //�����x�N�g������]������
    cameraV = Vector3::TransformNormal(cameraV, rotmat);
    //���_���W���v�Z
    eyepos = refpos + cameraV;
    //���_�����݈ʒu�����Ԃ���
    eyepos = m_eyepos + (eyepos - m_eyepos) * 0.1f;
    //�Q�Ɠ_�����݈ʒu�����Ԃ���
    refpos = m_refpos + (refpos - m_refpos) * 0.2f;

    this->SetEyePos(eyepos);
    this->SetRefPos(refpos);

    //���N���X�̍X�V
    Camera::Update();
}