//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:�f���V�[��
//
//day:2017/12/02
//
//acotr:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/

#pragma once
#include "Scene.h"
#include "..\Model\Obj3d.h"
#include "..\Character\Enemy\Skeleton.h"
#include "..\..\LandShape.h"
#include "..\Manager\ParticleEffectManager.h"

class DemoScene : public Scene
{
public:
    //���
    enum CUT
    {
        CUT1,
        CUT2,
        CUT3
    };

public:
    DemoScene() {}
    ~DemoScene() {}

    //������
    void Enter(int width,int height,Camera* camera)override;
    //�X�V
    std::unique_ptr<Scene> Update()override;
    //�`��
    void Render()override;

    //�t�F�[�h�C��
    bool FadeIn();
    //�t�F�[�h�A�E�g
    bool FadeOut();

private:
    //��
    Obj3d m_wall;
    LandShape m_collision_wall;
    LandShape m_ground;
    LandShape m_center_wall;

    BossCamera* m_camera;                   //�J����

    std::unique_ptr<Skeleton> m_skeleton;	//�{�X(�X�P���g��)

    Canvas m_canvas;                        //�L�����o�X
    float m_alpha;

    CUT m_cut;                              //��ʏ��
};

