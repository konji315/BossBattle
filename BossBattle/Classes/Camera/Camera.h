//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:�J�������N���X
//
//day:2017/05/11
//
//acotr:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
#pragma once

#include "..\..\pch.h"
#include <SimpleMath.h>

class Camera
{
public:
    //�R���X�g���N�^
    Camera(int width, int height);

    virtual ~Camera() {}

    virtual void Update();

    //�r���[�s��擾
    const DirectX::SimpleMath::Matrix& Camera::GetViewMatrix() { return m_camera_view; }
    //�v���W�F�N�V�����s��̎擾
    const DirectX::SimpleMath::Matrix& Camera::GetProjectionMatrix() { return m_camera_proj; }

    //�J�������W�̐ݒ�
    void SetEyePos(const DirectX::SimpleMath::Vector3& eyepos)  { m_eyepos = eyepos; }
    //�Q�Ɠ_�̐ݒ�
    void SetRefPos(const DirectX::SimpleMath::Vector3& refpos)  { m_refpos = refpos; }
    //������x�N�g���̐ݒ�
    void SetUpvecSet(const DirectX::SimpleMath::Vector3& upvec) { m_upvec = upvec; }
    //����p�̐ݒ�
    void SetFovY(float fovY) { m_fovY = fovY; }
    //�A�X�y�N�g��̐ݒ�
    void SetAspect(float aspect) { m_aspect = aspect; }

    void SetNearClip(float near_clip) { m_near_clip = near_clip; }
    void SetfarClip(float far_clip) { m_far_clip = far_clip; }

    //�J�������W�̎擾
    const DirectX::SimpleMath::Vector3& GetEyePos() { return m_eyepos; };
    //�Q�Ɠ_�̎擾
    const DirectX::SimpleMath::Vector3& GetRefPos() { return m_refpos; };
    //������x�N�g���̎擾
    const DirectX::SimpleMath::Vector3& GetUpvec() { return m_upvec; };
    //����p�̎擾
    float GetFov() { return m_fovY; };

    float GetNearClip() { return m_near_clip; };
    float GetFar() { return m_far_clip; };

    //�r���{�[�h�s��̎擾
    const DirectX::SimpleMath::Matrix& GetBillboard() const { return m_billboard; }
    const DirectX::SimpleMath::Matrix& GetBillboardConstrainY() const { return m_billboard_constrainY; }

    //�r���{�[�h�s��̌v�Z
    void CalcBillboard();

    //�J������U��
    void ShakeCamera(int power) { m_shake_power = power; }
protected:
    DirectX::SimpleMath::Matrix m_camera_view;    //�r���[�s��
    DirectX::SimpleMath::Vector3 m_eyepos;        //���_
    DirectX::SimpleMath::Vector3 m_refpos;        //�Q�Ɠ_
    DirectX::SimpleMath::Vector3 m_upvec;         //������x�N�g��
    DirectX::SimpleMath::Matrix m_camera_proj;    //�v���W�F�N�V�����s��

    float m_fovY;      //�c��������p
    float m_aspect;    //�A�X�y�N�g��
    float m_near_clip;  //�j�A�N���b�v
    float m_far_clip;   //�t�@�[�N���b�v

    DirectX::SimpleMath::Matrix m_billboard;             // �r���{�[�h�s��
    DirectX::SimpleMath::Matrix m_billboard_constrainY;   // �r���{�[�h�s��(Y���������j

    int m_shake_power;  //�U��
};