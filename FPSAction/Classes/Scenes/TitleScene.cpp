#include "TitleScene.h"
#include "WICTextureLoader.h"
#include <SimpleMath.h>
#include "../Scenes/PlayScene.h"
#include "../../DXTKResouces.h"
#include "../Manager/AudioManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

TitleScene::TitleScene()
{
}

void TitleScene::Enter(int width,int height,Camera* camera)
{
	AudioManager& am = singleton<AudioManager>::GetInstance();

	am.PlaySounds(CRI_PLAYSOUNDS_TITLE_BGM,0.5f);

	m_window_width = width;
	m_window_height = height;
}

std::unique_ptr<Scene> TitleScene::Update()
{
	DXTK::DXTKResources& dxtk = singleton<DXTK::DXTKResources>::GetInstance();

	if (dxtk.m_key_tracker->IsKeyPressed(Keyboard::Space))
	{
		return std::make_unique<PlayScene>();
	}


	return nullptr;
}

void TitleScene::Render()
{
}
