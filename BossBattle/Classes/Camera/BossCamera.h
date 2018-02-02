//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:�{�X�J�����N���X
//
//day:2017/11/29
//
//acotr:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/

#pragma once
#include "Camera.h"
#include "..\Character\Enemy\Enemy.h"

class BossCamera :public Camera
{
public:
    //�^�[�Q�b�g�ƃJ�����̋���
    static const float CAMERA_DISTANCE;

    //�R���X�g���N�^
    BossCamera(int width, int height);

    //���t���[���X�V
    void Update()override;

    //�^�[�Q�b�g�̐ݒ�
    void SetTargetPos(const DirectX::SimpleMath::Vector3& targetpos) { m_target_pos = targetpos; };
    void SetTargetAngle(const DirectX::SimpleMath::Vector3& angle) { m_camera_angle = angle; };

    //�{�X���̐ݒ�
    void SetBoss(Enemy* boss) { m_boss = boss; }

    //�J�����̊p�x���擾
    const DirectX::SimpleMath::Vector3& GetCameraAngle() { return m_camera_angle; }
private:
    DirectX::SimpleMath::Vector3 m_target_pos;      //�J�����̍��W

    DirectX::SimpleMath::Vector3 m_camera_angle;    //�J�����̊p�x

    Enemy* m_boss;                                  //�{�X���
};