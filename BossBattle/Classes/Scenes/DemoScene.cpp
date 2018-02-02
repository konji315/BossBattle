#include "DemoScene.h"

#include "WICTextureLoader.h"
#include <SimpleMath.h>
#include "..\Common\DXTKResouces.h"
#include "..\Manager\AudioManager.h"
#include "..\Scenes\PlayScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void DemoScene::Enter(int width, int height, Camera* camera)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();
    DXTK::DXTKResources& dxtk = singleton<DXTK::DXTKResources>::GetInstance();

    //音停止
    am.StopSound();

    //カメラ設定
    m_window_width = width;
    m_window_height = height;
    m_camera = dynamic_cast<BossCamera*>(camera);

    //壁の設定
    m_wall.LoadModel(L"Resources/Wall.cmo");
    m_wall.SetScaling(1 / 40.0f);
    m_wall.SetPos(Vector3(0, -100, 0));

    m_collision_wall.Initialize(L"Wall", L"");
    m_collision_wall.SetScale(1 / 58.0f);
    m_collision_wall.SetTrans(Vector3(0, -100, 0));

    m_ground.Initialize(L"Field", L"Field");

    m_center_wall.Initialize(L"Field", L"");
    m_center_wall.SetScale(0.23f);
    m_center_wall.SetTrans(Vector3(0, 80, 0));

    //スケルトンの設定
    m_skeleton = std::make_unique<Skeleton>();
    m_skeleton->SetCamera(m_camera);
    m_skeleton->SetPos(Vector3(0, -50, 0));
    m_skeleton->SetState(new Skeleton::DemoState());
    m_skeleton->CalcDamage(Skeleton::MAX_HP);

    m_alpha = 1;

    //UIの読み込み
    {
        m_canvas.SetTexture("Fade_Black", L"Resources/Sprites/fade_black.png");
        m_canvas.SetTexturePos("Fade_Black", Vector2(512, 384));
        m_canvas.SetColor("Fade_Black", { 1,1,1,m_alpha });

        m_canvas.SetTexture("Frame", L"Resources/Sprites/frame_black.png");
        m_canvas.SetTexturePos("Frame", Vector2(512, 384));

        m_canvas.SetTexture("Skip", L"Resources/Sprites/skip.png");
        m_canvas.SetTexturePos("Skip", Vector2(940, 740));
    }

    m_cut = CUT::CUT1;
}

/// <summary>
/// 更新
/// </summary>
std::unique_ptr<Scene> DemoScene::Update()
{
    DXTK::DXTKResources& dxtk = singleton<DXTK::DXTKResources>::GetInstance();
    AudioManager& am = singleton<AudioManager>::GetInstance();

    //キーボードの入力状態の取得
    Keyboard::State key_state = dxtk.m_keyboard->GetState();

    //ムービースキップ
    if (dxtk.m_button_tracker->start == GamePad::ButtonStateTracker::PRESSED || dxtk.m_key_tracker->IsKeyPressed(Keyboard::Escape))
        return std::make_unique<PlayScene>();

    //モデルの更新
    m_wall.Update();
    m_collision_wall.Update();

    m_ground.Update();
    m_center_wall.Update();

    //ボスの更新
    if (m_skeleton->GetPos().y <= 0)
        m_skeleton->SetPos(m_skeleton->GetPos() + Vector3(0, 0.03f, 0));

    m_skeleton->Update();

    static float timer = 0;
    static float timer2 = 0;
    static float rotate = 0;

    switch (m_cut)
    {
    case DemoScene::CUT1:   //場面1

        m_camera->SetTargetPos(XMVectorLerp(Vector3(0, -4, 10), Vector3(0, -4, 8), timer));
        if (timer >= 2.0f)
            m_camera->ShakeCamera(1000);

        if (timer >= 5.0f)
        {
            //フェードアウト
            if (FadeOut())
            {
                timer = 0;
                m_camera->SetTargetPos(Vector3(0, 15, 0));
                m_cut = CUT::CUT2;
            }
        }
        else
        {
            //フェードイン
            FadeIn();
        }
        break;
    case DemoScene::CUT2:   //場面2
        m_camera->ShakeCamera(1000);
        rotate += XMConvertToRadians(-0.1f);
        m_camera->SetTargetAngle(Vector3(0, rotate, 0));
        m_camera->SetRefPos(Vector3(0, -5, 0));
        if (timer >= 5.0f)
        {
            //フェードアウト
            if (FadeOut())
            {
                timer = 0;
                m_camera->SetTargetPos(Vector3(0, 20, -20));
                m_camera->SetTargetAngle(Vector3(0, XMConvertToRadians(-90), 0));
                m_cut = CUT::CUT3;
            }
        }
        else
        {
            //フェードイン
            FadeIn();
        }
        break;
    case DemoScene::CUT3:   //場面3
        m_camera->SetRefPos(Vector3(0, 8, 0));

        if (timer >= 7.5f)
        {
            m_camera->ShakeCamera(10000);
            m_camera->SetTargetPos(XMVectorLerp(Vector3(0, 20, -20), Vector3(0, 22, 0), timer2));
            timer2 += 1 / 60.0f;
            //フェードアウト
            if (FadeOut())
            {
                m_camera->ShakeCamera(0);
                return std::make_unique<PlayScene>();
            }
        }
        else
        {
            //フェードイン
            FadeIn();
            m_camera->ShakeCamera(1000);
        }

        break;
    default:
        break;
    }

    timer += 1 / 60.0f;

    //カメラの更新
    m_camera->Update();

    return nullptr;
}

/// <summary>
/// 描画
/// </summary>
void DemoScene::Render()
{
    DXTK::DXTKResources& dxtk = singleton<DXTK::DXTKResources>::GetInstance();

    dxtk.m_sprite_batch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());

    //壁の描画
    m_wall.Render();
    m_collision_wall.Draw();
    m_center_wall.Draw();

    //地面の描画
    m_ground.Draw();

    //スケルトンの描画
    m_skeleton->Render();

    //UIの描画
    m_canvas.Render();

    dxtk.m_sprite_batch->End();
}

/// <summary>
/// フェードイン
/// </summary>
bool DemoScene::FadeIn()
{
    if (m_alpha < 0.0f)
        return true;

    m_alpha -= 0.005f;

    Color color = { 1,1,1,m_alpha };
    m_canvas.SetColor("Fade_Black", color);

    return false;
}

/// <summary>
/// フェードアウト
/// </summary>
bool DemoScene::FadeOut()
{
    if (m_alpha > 1.0f)
        return true;

    m_alpha += 0.005f;

    Color color = { 1,1,1,m_alpha };

    m_canvas.SetColor("Fade_Black", color);

    return false;
}