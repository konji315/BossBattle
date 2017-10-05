//Å`/Å`/Å`//Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/
//Content:ÉGÉtÉFÉNÉgÉNÉâÉX
//
//day:2017/07/16
//
//acotr:Koshida Kenji
//Å`/Å`/Å`//Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/

#pragma once

#include "Effekseer.h"
#include "EffekseerRendererDX11.h"
#include <vector>
#include <d3d11.h>
#include <SimpleMath.h>
#include "../Camera/FPSCamera.h"
#include "../../Singleton.h"

class EffectManager : public singleton<EffectManager>
{
private:
	EffectManager() {};

public:
	
	enum EffectID
	{
		FIREBALL,
		BOSS_BALL,
		FIRE_HIT,
		DEATH_EXPLOSION1,
		DEATH_EXPLOSION2,
		NUM
	};
public:
	friend class singleton<EffectManager>;
	void Initialize(ID3D11Device* device,ID3D11DeviceContext* context,Camera* camera,int width,int height);

	void Update();
	void Render();

	void Destroy();

	//EffekseerÇÃç¿ïWånÇDirectXÇÃç¿ïWånÇ…ïœä∑
	static Effekseer::Vector3D DXVec2EFKVec(DirectX::SimpleMath::Vector3 vec);

	Effekseer::Handle PlayEffect(EffectID id,DirectX::SimpleMath::Vector3 pos);

//private:
	EffekseerRenderer::Renderer*	 m_renderer;
	Effekseer::Manager*				 m_manager;
	std::vector<Effekseer::Effect*>  m_effect;

	Camera* m_camera;
};