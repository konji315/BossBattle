//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:�v���C�V�[��
//
//day:2017/06/28
//
//acotr:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/

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

	bool FadeOut();				//�t�F�[�h�A�E�g

private:
	//���f��
	Obj3d m_wall;

	LandShape m_collision_wall;

	LandShape m_ground;

	//�J����
	Camera* m_camera;

	//�v���C���[
	Player m_player;

	std::unique_ptr<Enemy> m_boss;

	//�v���C���[���͏��
	IPlayerInput m_input;

	//std::unique_ptr<DirectX::SpriteFont> m_font;

	Canvas m_canvas;
	float m_alpha;
	bool m_fade_flag;
};

