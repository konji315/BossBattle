#include "EffectManager.h"

/// <summary>
/// 初期化
/// </summary>
void EffectManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, Camera* camera, int width, int height)
{
	m_camera = camera;

	m_renderer = EffekseerRendererDX11::Renderer::Create(device, context, 2000);

	m_manager = Effekseer::Manager::Create(2000);

	m_manager->SetSpriteRenderer(m_renderer->CreateSpriteRenderer());

	m_manager->SetRibbonRenderer(m_renderer->CreateRibbonRenderer());

	m_manager->SetRingRenderer(m_renderer->CreateRingRenderer());

	m_manager->SetTextureLoader(m_renderer->CreateTextureLoader());

	m_manager->SetCoordinateSystem(::Effekseer::CoordinateSystem::RH);

	m_renderer->SetProjectionMatrix(
		::Effekseer::Matrix44().PerspectiveFovRH(camera->GetFov(), (float)width / (float)height, camera->GetNearClip(), camera->GetFar()));

	m_effect.resize(EffectID::NUM);

	m_effect[EffectID::FIREBALL] = Effekseer::Effect::Create(m_manager, (const EFK_CHAR*)L"Resources/fireball.efk");

	m_effect[EffectID::BOSS_BALL] = Effekseer::Effect::Create(m_manager, (const EFK_CHAR*)L"Resources/boss_ball.efk");

	m_effect[EffectID::FIRE_HIT] = Effekseer::Effect::Create(m_manager, (const EFK_CHAR*)L"Resources/hit_effect.efk");

	m_effect[EffectID::DEATH_EXPLOSION1] = Effekseer::Effect::Create(m_manager, (const EFK_CHAR*)L"Resources/boss_explosion1.efk");

	m_effect[EffectID::DEATH_EXPLOSION2] = Effekseer::Effect::Create(m_manager, (const EFK_CHAR*)L"Resources/boss_explosion2.efk");
}

/// <summary>
/// 更新
/// </summary>
void EffectManager::Update()
{
	m_renderer->SetCameraMatrix(Effekseer::Matrix44().LookAtRH(DXVec2EFKVec(m_camera->GetEyePos()), DXVec2EFKVec(m_camera->GetRefPos()), DXVec2EFKVec(m_camera->GetUpvec())));

	m_manager->Update();
}

/// <summary>
/// 描画
/// </summary>
void EffectManager::Render()
{
	m_renderer->BeginRendering();
	m_manager->Draw();
	m_renderer->EndRendering();
}

//削除
void EffectManager::Destroy()
{
	m_manager->Destroy();
	m_renderer->Destory();

	for (auto itr = m_effect.begin(); itr != m_effect.end(); itr++)
	{
		ES_SAFE_RELEASE(*itr);
	}
}

/// <summary>
/// Effekseerの座標系をDirectXの座標系に変換
/// </summary>
/// <param name="vec">DirectXでの座標</param>
/// <returns>Effekseerでの座標</returns>
Effekseer::Vector3D EffectManager::DXVec2EFKVec(DirectX::SimpleMath::Vector3 vec)
{
	return Effekseer::Vector3D(vec.x, vec.y, vec.z);
}

/// <summary>
/// エフェクトの再生
/// </summary>
/// <param name="id"></param>
/// <returns></returns>
Effekseer::Handle EffectManager::PlayEffect(EffectID id, DirectX::SimpleMath::Vector3 pos)
{
	Effekseer::Handle handle;

	handle = m_manager->Play(m_effect[id], pos.x, pos.y, pos.z);

	return handle;
}
