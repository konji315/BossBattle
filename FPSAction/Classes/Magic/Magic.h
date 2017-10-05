//Å`/Å`/Å`//Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/
//Content:ñÇñ@ÉNÉâÉXÇÃäÓíÍÉNÉâÉX
//
//day:2017/07/19
//
//acotr:Koshida Kenji
//Å`/Å`/Å`//Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/
#pragma once
#include "../Collider/CollisionNode.h"
#include "../../Obj3d.h"
#include "../Manager/EffectManager.h"
#include "../../Singleton.h"

#include <SimpleMath.h>

class Magic
{
public:
	Magic() {}
	virtual ~Magic(){}

	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual void SetPos(const DirectX::SimpleMath::Vector3& vec) { m_obj_effect.SetPos(vec); }
	virtual void SetAngle(const DirectX::SimpleMath::Vector3& angle) { m_obj_effect.SetAngle(angle); }

	virtual const DirectX::SimpleMath::Vector3& GetPos() { return m_obj_effect.GetPos(); };

	//ÉGÉtÉFÉNÉgÇ™ë∂ç›ÇµÇƒÇ¢ÇÈÇ©Ç«Ç§Ç©
	virtual bool IsEffectExit()
	{
		EffectManager& efk = singleton<EffectManager>::GetInstance();

		return efk.m_manager->Exists(m_effect_handle);
	}

	const SphereNode& GetCollisonSphere() { return m_collision_sphere; }

	void StopEffect()
	{
		EffectManager& efk = singleton<EffectManager>::GetInstance();

		efk.m_manager->StopEffect(m_effect_handle);
	};
protected:
	SphereNode m_collision_sphere;
	Effekseer::Handle m_effect_handle;
	Obj3d m_obj_effect;

	float m_vanish_timer;
};