//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:TPS�J����
//
//day:2017/06/30
//
//acotr:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/

#pragma once
#include "Camera.h"
#include "..\Character\Player\Player.h"

class TPSCamera : public Camera
{
public:
    //�v���C���[�ƃJ�����̋���
    static const float CAMERA_DISTANCE;
    //�J�����̐��񑬓x
    static const float CAMERA_TURN_SPEED;

    //�R���X�g���N�^
    TPSCamera(int width, int height);

    //���t���[���X�V
    void Update()override;

    //�^�[�Q�b�g�̐ݒ�
    void SetTargetPos(const DirectX::SimpleMath::Vector3& targetpos) { m_target_pos = targetpos; };
    void SetTargetAngle(const DirectX::SimpleMath::Vector3& player_angle) { m_camera_angle = player_angle; };

    //�v���C���[���̐ݒ�
    void SetPlayer(Player* player) { m_player = player; };
private:
    DirectX::SimpleMath::Vector3 m_target_pos;      //�J�����̍��W
    DirectX::SimpleMath::Vector3 m_camera_angle;    //�J�����̊p�x

    Player* m_player;   //�v���C���[���
};