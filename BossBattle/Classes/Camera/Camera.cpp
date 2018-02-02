#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// �R���X�g���N�^
/// <param name="width">�E�B���h�E�̉���</param>
/// <param name="height">�E�B���h�E�̏c��</param>
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
/// �X�V����
/// </summary>
void Camera::Update()
{
    //�r���[�s�񐶐�
    m_camera_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);

    //�v���W�F�N�V�����s�񐶐�
    m_camera_proj = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_near_clip, m_far_clip);

    // �r���{�[�h�s����v�Z
    CalcBillboard();
}

/// <summary>
/// �r���{�[�h�s��̌v�Z
/// </summary>
void Camera::CalcBillboard()
{
    // ��������
    Vector3 eyeDir = m_eyepos - m_refpos;
    // Y��
    Vector3 Y = Vector3::UnitY;
    // X��
    Vector3 X = Y.Cross(eyeDir);
    X.Normalize();
    // Z��
    Vector3 Z = X.Cross(Y);
    Z.Normalize();
    // Y������r���{�[�h�s��i�E��n�̈�Z�������]�j
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
    // �r���{�[�h�s��i�E��n�̈�Z�������]�j
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