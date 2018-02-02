#include "PlayScene.h"
#include "WICTextureLoader.h"
#include <SimpleMath.h>
#include "..\Common\DXTKResouces.h"
#include "..\Manager\AudioManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

PlayScene::~PlayScene()
{
    //敵の終了処理
    for (auto enemy : m_enemys)
    {
        if (enemy != nullptr)
        {
            delete enemy;
            enemy = nullptr;
        }
    }
}

void PlayScene::Enter(int width, int height, Camera* camera)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();
    DXTK::DXTKResources& dxtk = singleton<DXTK::DXTKResources>::GetInstance();
    ParticleEffectManager& pem = singleton<ParticleEffectManager>::GetInstance();

    //音停止
    am.StopSound();
    am.PlaySounds(CRI_PLAYSOUNDS_BATTLE3_BGM, 0.3f);

    //カメラ情報設定
    m_window_width = width;
    m_window_height = height;
    m_camera = camera;
    m_camera->SetEyePos(Vector3(0, 12, 100));

    //プレイヤー位置情報設定
    m_player.SetPos(Vector3(0, 30, 50));

    //一番外の壁
    m_wall.LoadModel(L"Resources/Wall.cmo");
    m_wall.SetScaling(1 / 40.0f);
    m_wall.SetPos(Vector3(0, -100, 0));

    //外側の壁
    m_collision_wall.Initialize(L"Wall", L"");
    m_collision_wall.SetScale(1 / 58.0f);
    m_collision_wall.SetTrans(Vector3(0, -100, 0));

    //地面
    m_ground.Initialize(L"Field", L"Field");

    //内側の壁
    m_center_wall.Initialize(L"Field", L"");
    m_center_wall.SetScale(0.23f);
    m_center_wall.SetTrans(Vector3(0, 80, 0));

    //スケルトン設定
    m_skeleton = std::make_unique<Skeleton>();
    m_skeleton->SetPlayerInfo(&m_player);
    m_skeleton->SetCamera(m_camera);

    m_alpha = 0.0f;
    m_txt_grav = 0.0f;

    //UI設定
    m_canvas.SetTexture("FadeOut", L"Resources/Sprites/fade_white.png");
    m_canvas.SetTexturePos("FadeOut", Vector2(512, 384));
    m_canvas.SetColor("FadeOut", { 1,1,1,m_alpha });
    m_canvas.SetTexture("GameOver", L"Resources/Sprites/gameover.png");
    m_canvas.SetTexturePos("GameOver", Vector2(512, -384));

    dynamic_cast<BossCamera*>(m_camera)->SetBoss(m_skeleton.get());
}

/// <summary>
/// 更新
/// </summary>
std::unique_ptr<Scene> PlayScene::Update()
{
    AudioManager& am = singleton<AudioManager>::GetInstance();

    //プレイヤー更新
    m_player.SetCameraRotate(dynamic_cast<BossCamera*>(m_camera)->GetCameraAngle());
    m_player.Update();

    //カメラ更新
    m_camera->Update();

    //入力
    m_input.ButtonInput(&m_player);

    //モデルの更新
    m_wall.Update();

    //外側の壁の更新
    m_collision_wall.Update();

    //地面の更新
    m_ground.Update();

    //内側の壁の更新
    m_center_wall.Update();

    //地形へのめり込み防止
    {
        //当たり判定の取得
        Sphere sphere = m_player.GetCollisonSphere();
        //自機のワールド座標取得
        Vector3 trans = m_player.GetPos();
        //球の中心から目標のセンターへのベクトル
        Vector3 sphere2Player = trans - sphere.center;

        //めり込み防止ベクトル
        Vector3 reject;

        //地面とのめり込み排斥
        if (m_ground.IntersectSphere(sphere, &reject))
        {
            sphere.center += reject;
        }
        //外側の壁とのめり込み排斥
        if (m_collision_wall.IntersectSphere(sphere, &reject))
        {
            sphere.center += reject;
        }
        //内側の壁とのめり込み排斥
        if (m_center_wall.IntersectSphere(sphere, &reject))
        {
            sphere.center += reject;
        }
        m_player.SetPos(sphere.center + sphere2Player);
    }

    //敵とのめり込み排斥処理
    {
        const Sphere& player_sphere = m_player.GetCollisonSphere();

        Vector3 trans = m_player.GetPos();

        Vector3 reject;

        for (auto enemy : m_enemys)
        {
            const Sphere& enemy_sphere = enemy->GetCollisonSphere();
            //接点からの方向ベクトル取得
            if (CheckSphere2Sphere(player_sphere, enemy_sphere, &reject))
            {
                Vector3 vec = trans - reject;
                vec.Normalize();

                m_player.SetPos(trans + vec * 0.4f);
            }
        }

        //ボスの手とのめり込み排斥処理
        for (int i = 0; i < Skeleton::HAND_NUM; i++)
        {
            const Sphere& hand_sphere = m_skeleton->GetHandCollision(i);
            if (CheckSphere2Sphere(player_sphere, hand_sphere, &reject) && m_player.GetCollisionState())
            {
                Vector3 vec = trans - reject;
                vec.Normalize();

                m_player.SetPos(trans + vec);
            }
        }
        //ボスの頭とのめり込み排斥処理
        for (int i = 0; i < Skeleton::HORN_NUM; i++)
        {
            const Sphere& horn_sphere = m_skeleton->GetHornCollision(i);
            if (CheckSphere2Sphere(player_sphere, horn_sphere, &reject))
            {
                Vector3 vec = trans - reject;
                vec.Normalize();

                m_player.SetPos(trans + vec);
            }
        }
    }

    //自機が地面に立つ処理
    if (m_player.GetVelocity().y <= 0.0f)
    {
        //自機の頭から足元への線分
        Segment player_segment;

        Vector3 trans = m_player.GetPos();

        player_segment.start = trans + Vector3(0, -1, 0);
        player_segment.end = trans + Vector3(0, -2.5f, 0);

        //交点座標
        Vector3 inter;

        //地形と線分の当たり判定
        if (m_ground.IntersectSegment(player_segment, &inter))
        {
            //Y座標のみ交点へ移動
            trans.y = inter.y + 2.0f;

            m_player.StopJump();
        }
        else
        {
            //落下開始
            m_player.StartFall();
        }

        m_player.SetPos(trans);

    }

    //プレイヤーから敵への攻撃判定
    {
        //プレイヤーが攻撃中かどうか
        if (m_player.GetAttackState())
        {
            for (auto enemy : m_enemys)
            {
                //敵がダメージ受付(判定を取る)状態か
                if (enemy->GetCollisionState())
                {
                    //武器と敵の当たり判定
                    const Sphere& weapon_sphere = m_player.GetWeaponInfo()->GetCollisonSphere();
                    const Sphere& enemy_sphere = enemy->GetCollisonSphere();

                    Vector3 point;

                    //武器と敵が当たったらダメージを与える
                    if (CheckSphere2Sphere(weapon_sphere, enemy_sphere, &point))
                        Hit(m_player, enemy, point);
                }
            }
        }
    }

    //プレイヤーからボスへの攻撃判定
    {
        if (m_player.GetAttackState())
        {
            if (m_skeleton->GetCollisionState())
            {
                const Sphere& weapon_sphere = m_player.GetWeaponInfo()->GetCollisonSphere();
                //const Sphere* horn_sphere = m_skeleton->GetHornCollision();

                //手との当たり判定
                for (int i = 0; i < Skeleton::HAND_NUM; i++)
                {
                    const Sphere& hand_sphere = m_skeleton->GetHandCollision(i);
                    Vector3 point;
                    if (CheckSphere2Sphere(weapon_sphere, hand_sphere, &point))
                    {
                        //プレイヤーの与えるダメージ
                        int damage = m_player.GetDamage();
                        //スタン値蓄積
                        m_skeleton->CalcStunDamage(damage);
                        //攻撃が当たった
                        m_player.SetAttackState(false);
                        m_player.HitEffect(point);

                        am.PlaySounds(CRI_PLAYSOUNDS_SLASH);
                    }
                }

                //角との当たり判定
                for (int i = 0; i < Skeleton::HORN_NUM; i++)
                {
                    const Sphere& horn_sphere = m_skeleton->GetHornCollision(i);
                    Vector3 point;
                    if (CheckSphere2Sphere(weapon_sphere, horn_sphere, &point))
                    {
                        //プレイヤーの与えるダメージ
                        int damage = m_player.GetDamage();
                        //ダメージ計算
                        m_skeleton->CalcDamage(damage);
                        m_skeleton->HitDamageColor(Colors::Orange);

                        //攻撃が当たった
                        m_player.SetAttackState(false);
                        m_player.HitEffect(point);

                        am.PlaySounds(CRI_PLAYSOUNDS_SLASH);
                    }
                }

            }
        }
    }

    //敵からプレイヤーへの攻撃判定
    {
        //ボスが攻撃中
        if (m_skeleton->GetAttackState())
        {
            //プレイヤーの判定がある
            if (m_player.GetCollisionState())
            {
                const Sphere& player_sphere = m_player.GetCollisonSphere();

                //手にヒット
                for (int i = 0; i < Skeleton::HAND_NUM; i++)
                {
                    const Sphere& hand_sphere = m_skeleton->GetHandCollision(i);
                    Vector3 point;
                    if (CheckSphere2Sphere(player_sphere, hand_sphere, &point))
                    {
                        //ダメージを受ける
                        Hit(m_skeleton.get(), m_player, point);
                    }
                }
            }
        }
    }

    //プレイヤーが死亡したらGameOver演出
    if (m_player.IsDead())
    {
        if (m_canvas.GetPos("GameOver").y <= 390)
        {
            m_txt_grav += 0.05f;
            //Game Overが画面縦半分まで行ったら
            if (m_canvas.GetPos("GameOver").y >= 384)
                m_txt_grav = -m_txt_grav + 5;

            m_canvas.SetTexturePos("GameOver", Vector2(m_canvas.GetPos("GameOver").x, m_canvas.GetPos("GameOver").y + m_txt_grav));
        }
        else
        {
            static float timer = 0;
            timer += 1 / 60.0f;
            if (timer >= 3.0f)
                return std::make_unique<PlayScene>();
        }
    }

    //敵の更新
    for (auto enemy : m_enemys)
        enemy->Update();

    //ボスの更新
    if (!m_skeleton->IsDead())
        m_skeleton->Update();

    //フェードアウト
    if (!m_skeleton->ExistsHP())
    {
        if (FadeOut())
            return std::make_unique<PlayScene>();
    }

    return nullptr;
}

/// <summary>
/// 描画
/// </summary>
void PlayScene::Render()
{
    DXTK::DXTKResources& dxtk = singleton<DXTK::DXTKResources>::GetInstance();

    dxtk.m_sprite_batch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());

    //壁の描画
    m_wall.Render();
    m_collision_wall.Draw();
    m_center_wall.Draw();

    //地面
    m_ground.Draw();

    //プレイヤー
    m_player.Render();

    //敵描画
    for (auto enemy : m_enemys)
        enemy->Render();

    //ボス
    m_skeleton->Render();

    //UI
    m_canvas.Render();

    dxtk.m_sprite_batch->End();
}


/// <summary>
/// フェードアウト
/// </summary>
/// <returns>終了か</returns>
bool PlayScene::FadeOut()
{
    if (m_alpha > 1.2f)
        return true;

    m_alpha += 0.003f;

    Color color = { 1,1,1,m_alpha };

    m_canvas.SetColor("FadeOut", color);

    return false;
}

/// <summary>
/// 武器が敵に当たった処理
/// </summary>
/// <param name="weapon">武器</param>
/// <param name="enemy">敵</param>
void PlayScene::Hit(Player& player, Enemy * enemy, const DirectX::SimpleMath::Vector3& point)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();

    //プレイヤーの与えるダメージ
    int damage = player.GetDamage();

    //ダメージ計算&被ダメ状態&色変更
    enemy->CalcDamage(damage);
    enemy->SetCollisionState(false);
    enemy->HitDamageColor(Colors::Orange);

    //攻撃が当たった
    player.SetAttackState(false);
    player.HitEffect(point);

    am.PlaySounds(CRI_PLAYSOUNDS_SLASH);
}

/// <summary>
/// 敵がプレイヤーに当たった処理
/// </summary>
/// <param name="enemy">敵</param>
/// <param name="player">プレイヤー</param>
void PlayScene::Hit(Enemy * enemy, Player & player, const DirectX::SimpleMath::Vector3& point)
{
    AudioManager& am = singleton<AudioManager>::GetInstance();

    //敵の与えるダメージ
    int damage = enemy->GetDamage();

    //ダメージ計算＆被ダメ状態へ
    player.CalcDamage(damage);
    player.DamageEffect();

    //攻撃が当たった
    enemy->SetAttackState(false);

    am.PlaySounds(CRI_PLAYSOUNDS_PLAYER_HIT, 0.8f);
}
