//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:“–‚½‚è”»’è‚ÌƒfƒoƒbƒO•\¦‚Æ©“®ŒvZ
//
//day:2017/06/19
//
//actor:Koshida Kenji
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/

#pragma once
#include "Collision.h"
#include "../../Obj3d.h"

class CollisionNode
{
protected:
	//•\¦”ñ•\¦
	static bool m_visible;

public:
	//CollisionNode();
	//virtual ~CollisionNode();
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	//e‚ğƒZƒbƒg
	void SetParent(Obj3d* parent);
	//ƒIƒtƒZƒbƒg‚ÌƒZƒbƒg
	void SetOffset(DirectX::SimpleMath::Vector3& offset) { m_offset = offset; }
	//•\¦ƒtƒ‰ƒO‚ÌƒZƒbƒg
	static void SetDebugVisible(bool flag) { m_visible = flag; };
	//•\¦ƒtƒ‰ƒO‚ÌƒQƒbƒg
	static bool GetDebugVisible() { return m_visible; };
protected:
	//ƒfƒoƒbƒO•\¦—pƒIƒuƒWƒFƒNƒg
	Obj3d m_Obj;


	//e‚©‚ç‚ÌƒIƒtƒZƒbƒg
	DirectX::SimpleMath::Vector3 m_offset;
};

/// <summary>
/// ‹…‚Ì‚ ‚½‚è”»’èƒm[ƒh
/// </summary>
class SphereNode : public CollisionNode,public Sphere
{
public:
	SphereNode();

	void Initialize();
	void Update();
	void Render();

	void SetLocalRadius(float radius) { m_local_radius = radius; }
protected:
	//ƒ[ƒJƒ‹”¼Œa
	float m_local_radius;
};