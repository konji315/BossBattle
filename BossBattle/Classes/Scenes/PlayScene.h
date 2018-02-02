//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:ƒvƒŒƒCƒV[ƒ“
//
//day:2017/06/28
//
//acotr:Koshida Kenji
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/

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

    //‰Šú‰»
    void Enter(int width, int height, Camera* camera)override;
    //XV
    std::unique_ptr<Scene> Update()override;
    //•`‰æ
    void Render()override;
    //ƒtƒF[ƒhƒAƒEƒg
    bool FadeOut();

private:
    void Hit(Player& weapon, Enemy* enemy, const DirectX::SimpleMath::Vector3& point);
    void Hit(Enemy* enemy, Player& player, const DirectX::SimpleMath::Vector3& point);
private:
    Obj3d m_wall;                   //ƒ‚ƒfƒ‹

    LandShape m_collision_wall;     //ŠO‘¤‚Ì•Ç
    LandShape m_ground;             //’n–Ê
    LandShape m_center_wall;        //“à‘¤‚Ì•Ç

    Camera* m_camera;               //ƒJƒƒ‰

    Player m_player;                //ƒvƒŒƒCƒ„[

    std::vector <Enemy*> m_enemys;	//“GŒQ

    std::unique_ptr<Skeleton> m_skeleton;	//ƒ{ƒX(ƒXƒPƒ‹ƒgƒ“)

    IPlayerInput m_input;           //ƒvƒŒƒCƒ„[“ü—Íî•ñ

    //UIî•ñ
    Canvas m_canvas;
    float m_alpha;
    float m_txt_grav;
};

