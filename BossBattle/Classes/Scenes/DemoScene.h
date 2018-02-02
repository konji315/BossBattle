//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:ƒfƒ‚ƒV[ƒ“
//
//day:2017/12/02
//
//acotr:Koshida Kenji
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/

#pragma once
#include "Scene.h"
#include "..\Model\Obj3d.h"
#include "..\Character\Enemy\Skeleton.h"
#include "..\..\LandShape.h"
#include "..\Manager\ParticleEffectManager.h"

class DemoScene : public Scene
{
public:
    //ê–Ê
    enum CUT
    {
        CUT1,
        CUT2,
        CUT3
    };

public:
    DemoScene() {}
    ~DemoScene() {}

    //‰Šú‰»
    void Enter(int width,int height,Camera* camera)override;
    //XV
    std::unique_ptr<Scene> Update()override;
    //•`‰æ
    void Render()override;

    //ƒtƒF[ƒhƒCƒ“
    bool FadeIn();
    //ƒtƒF[ƒhƒAƒEƒg
    bool FadeOut();

private:
    //•Ç
    Obj3d m_wall;
    LandShape m_collision_wall;
    LandShape m_ground;
    LandShape m_center_wall;

    BossCamera* m_camera;                   //ƒJƒƒ‰

    std::unique_ptr<Skeleton> m_skeleton;	//ƒ{ƒX(ƒXƒPƒ‹ƒgƒ“)

    Canvas m_canvas;                        //ƒLƒƒƒ“ƒoƒX
    float m_alpha;

    CUT m_cut;                              //ê–Êî•ñ
};

