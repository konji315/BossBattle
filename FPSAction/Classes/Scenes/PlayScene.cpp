#include "PlayScene.h"
#include "WICTextureLoader.h"
#include <SimpleMath.h>
#include "TitleScene.h"
#include "../../DXTKResouces.h"
#include "../Manager/AudioManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

PlayScene::PlayScene()
{
}

void PlayScene::Enter(int width,int height,Camera* camera)
{
	AudioManager& am = singleton<AudioManager>::GetInstance();
	DXTK::DXTKResources& dxtk = singleton<DXTK::DXTKResources>::GetInstance();

	am.StopSound();
	am.PlaySounds(CRI_PLAYSOUNDS_BATTLE3_BGM,0.5f);

	m_window_width = width;
	m_window_height = height;
	m_camera = camera;

	//m_model_ground.LoadModel(L"Resources/Ground200m.cmo");

	//m_model_ground.DisableLighting();
	m_wall.LoadModel(L"Resources/Wall.cmo");
	m_wall.SetScaling(Vector3(1 / 25.0f, 1 / 25.0f, 1 / 25.0f));
	m_wall.SetPos(Vector3(0, -100, 0));

	m_collision_wall.Initialize(L"Wall", L"");
	m_collision_wall.SetScale(1 / 30.0f);
	m_collision_wall.SetTrans(Vector3(0, -100, 0));


	m_ground.Initialize(L"Field", L"Field");

	m_ground.DisableLighting();

	m_player.SetPos(Vector3(0, 50, 20));

	dynamic_cast<FPSCamera*>(m_camera)->SetPlayer(&m_player);

	m_boss = std::make_unique<BOSS>();

	m_alpha = 0;
	m_fade_flag = false;

	m_canvas.SetTexture("FadeOut", L"Resources/Sprites/fade_white.png");
	m_canvas.SetTexturePos("FadeOut", Vector2(512, 384));
	Color color = Colors::Transparent;
	m_canvas.SetColor("FadeOut", {1,1,1,m_alpha});


	//m_font = std::make_unique<SpriteFont>(dxtk.m_device, L"msgothic.spritefont");

}

std::unique_ptr<Scene> PlayScene::Update()
{
	m_input.ButtonInput(&m_player);

	//モデルたちの更新
	m_wall.Update();
	m_collision_wall.Update();

	m_ground.Update();

	{//自機の地形へのめり込み防止
	 //自機の当たり判定の取得
		Sphere sphere = m_player.GetCollisionNodeBody();
		//自機のワールド座標取得
		Vector3 trans = m_player.GetPos();
		//球の中心から目標のセンターへのベクトル
		Vector3 sphere2Player = trans - sphere.center;

		//めり込み防止ベクトル
		Vector3 reject;

		if (m_ground.IntersectSphere(sphere, &reject))
		{
			sphere.center += reject;
		}
		if (m_collision_wall.IntersectSphere(sphere, &reject))
		{
			sphere.center += reject;
		}
		m_player.SetPos(sphere.center + sphere2Player);
	}

	if (m_player.GetVelocity().y <= 0.0f)
	{//自機が地面に立つ処理
	 //自機の頭から足元への線分
		Segment player_segment;

		Vector3 trans = m_player.GetPos();

		player_segment.start = trans + Vector3(0, 1, 0);
		player_segment.end = trans + Vector3(0, -0.5f, 0);

		//交点座標
		Vector3 inter;

		//地形と線分の当たり判定
		if (m_ground.IntersectSegment(player_segment, &inter))
		{
			//Y座標のみ交点へ移動
			trans.y = inter.y;

			m_player.StopJump();
		}
		else
		{
			//落下開始
			m_player.StartFall();
		}

		m_player.SetPos(trans);

	}

	m_player.Update();

	//ボス撃破
	if (m_boss != nullptr)
	{
		if (m_boss->IsDead())
		{
			Enemy* boss = m_boss.release();
			delete boss;
			m_fade_flag = true;
		}
		else
		{
			m_boss->Update();
		}
	}
	m_camera->Update();

	//フェードアウト
	if (m_fade_flag)
	{
		if (FadeOut())
		{
			return std::make_unique<PlayScene>();
		}
	}

	return nullptr;
}

void PlayScene::Render()
{
	EffectManager& efk = singleton<EffectManager>::GetInstance();
	DXTK::DXTKResources& dxtk = singleton<DXTK::DXTKResources>::GetInstance();

	dxtk.m_sprite_batch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());

	//wchar_t text[12];
	//_itow_s((int)(m_wall.GetPos().y), text, 10);

	//wchar_t t[10] = L"posy : ";

	//Vector2 origin = m_font->MeasureString(t) / 2.f;

	//m_font->DrawString(dxtk.m_sprite_batch.get(), wcscat(t, text),
	//	Vector2(700, 20), Colors::White, 0.f, origin);

	//m_model_ground.Render();
	m_wall.Render();
	m_collision_wall.Draw();

	m_ground.Draw();
	//m_model_skydome.Render();

	m_player.Render();

	if (m_boss != nullptr)
	{
		m_boss->Render();
	}

	m_canvas.Render();

	dxtk.m_sprite_batch->End();

	efk.Render();

}


/// <summary>
/// フェードアウト
/// </summary>
/// <returns>終了か</returns>
bool PlayScene::FadeOut()
{
	if (m_alpha > 1.0f)
	{
		return true;
	}

	m_alpha += 0.005f;

	Color color = { 1,1,1,m_alpha };

	m_canvas.SetColor("FadeOut", color);

	return false;
}
