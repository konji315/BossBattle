//Å`/Å`/Å`//Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/
//Content:É^ÉCÉgÉãÉVÅ[Éì
//
//day:2017/06/28
//
//acotr:Koshida Kenji
//Å`/Å`/Å`//Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/

#pragma once
#include "Scene.h"
#include "../Camera/FPSCamera.h"
class TitleScene : public Scene
{
public:
	TitleScene();

	void Enter(int width,int height,Camera* camera)override;
	std::unique_ptr<Scene> Update()override;
	void Render()override;
private:
};

