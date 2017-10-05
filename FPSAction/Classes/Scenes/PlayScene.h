//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:ƒvƒŒƒCƒV[ƒ“
//
//day:2017/06/28
//
//acotr:Koshida Kenji
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/

#pragma once
#include "Scene.h"

#include "../../Obj3d.h"

#include "../Camera/FPSCamera.h"
#include "../Player/Player.h"
#include "../Player/IPlayerInput.h"

#include "../Enemy/BOSS.h"

#include "../../LandShape.h"

#include <SpriteFont.h>
#include <SpriteBatch.h>

class PlayScene : public Scene
{
public:
	PlayScene();

	void Enter(int width,int height,Camera* camera)override;
	std::unique_ptr<Scene> Update()override;
	void Render()override;

	bool FadeOut();				//ƒtƒF[ƒhƒAƒEƒg

private:
	//ƒ‚ƒfƒ‹
	Obj3d m_wall;

	LandShape m_collision_wall;

	LandShape m_ground;

	//ƒJƒƒ‰
	Camera* m_camera;

	//ƒvƒŒƒCƒ„[
	Player m_player;

	std::unique_ptr<Enemy> m_boss;

	//ƒvƒŒƒCƒ„[“ü—Íî•ñ
	IPlayerInput m_input;

	//std::unique_ptr<DirectX::SpriteFont> m_font;

	Canvas m_canvas;
	float m_alpha;
	bool m_fade_flag;
};

