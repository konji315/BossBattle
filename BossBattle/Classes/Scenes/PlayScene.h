//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:�v���C�V�[��
//
//day:2017/06/28
//
//acotr:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/

#pragma once
#include "Scene.h"

#include "..\Model\Obj3d.h"

#include "..\Character\Player\Player.h"
#include "..\Character\Player\IPlayerInput.h"
#include "..\Character\Enemy\Skeleton.h"

#include "..\..\LandShape.h"

#include "..\Manager\ParticleEffectManager.h"

#include <SpriteFont.h>
#include <SpriteBatch.h>

class PlayScene : public Scene
{
public:
    PlayScene() {}
    ~PlayScene();

    //������
    void Enter(int width, int height, Camera* camera)override;
    //�X�V
    std::unique_ptr<Scene> Update()override;
    //�`��
    void Render()override;
    //�t�F�[�h�A�E�g
    bool FadeOut();

private:
    void Hit(Player& weapon, Enemy* enemy, const DirectX::SimpleMath::Vector3& point);
    void Hit(Enemy* enemy, Player& player, const DirectX::SimpleMath::Vector3& point);
private:
    Obj3d m_wall;                   //���f��

    LandShape m_collision_wall;     //�O���̕�
    LandShape m_ground;             //�n��
    LandShape m_center_wall;        //�����̕�

    Camera* m_camera;               //�J����

    Player m_player;                //�v���C���[

    std::vector <Enemy*> m_enemys;	//�G�Q

    std::unique_ptr<Skeleton> m_skeleton;	//�{�X(�X�P���g��)

    IPlayerInput m_input;           //�v���C���[���͏��

    //UI���
    Canvas m_canvas;
    float m_alpha;
    float m_txt_grav;
};

