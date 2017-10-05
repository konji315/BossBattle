//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:ƒJƒƒ‰§ŒäƒNƒ‰ƒX
//
//day:2017/05/11
//
//acotr:Koshida Kenji
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
#include "../../pch.h"
#include <SimpleMath.h>

#pragma once
class Camera
{
public:
	Camera(int width ,int height);

	virtual ~Camera();

	virtual void Update();

	const DirectX::SimpleMath::Matrix& Camera::GetViewMatrix();
	const DirectX::SimpleMath::Matrix& Camera::GetProjectionMatrix();

	void SetEyeSet(const DirectX::SimpleMath::Vector3& eyepos);
	void SetRefSet(const DirectX::SimpleMath::Vector3& refpos);
	void SetUpvecSet(const DirectX::SimpleMath::Vector3& upvec);
	void SetFovY(float fovY);
	void SetAspect(float aspect);
	void SetNearClip(float nearClip);
	void SetfarClip(float farClip);

	const DirectX::SimpleMath::Vector3& GetEyePos() { return m_eyepos; };
	const DirectX::SimpleMath::Vector3& GetRefPos() { return m_refpos; };
	const DirectX::SimpleMath::Vector3& GetUpvec() { return m_upvec; };
	float GetFov() { return m_fovY; };
	float GetNearClip() { return m_nearClip; };
	float GetFar() { return m_farClip; };
	//bool Project(const DirectX::SimpleMath::Vector3& worldPos, DirectX::SimpleMath::Vector2* scrrenPos);
protected:
	//ƒrƒ…[s—ñ
	DirectX::SimpleMath::Matrix m_camera_view;
	//‹“_
	DirectX::SimpleMath::Vector3 m_eyepos;
	//’‹“_
	DirectX::SimpleMath::Vector3 m_refpos;
	//ã•ûŒü
	DirectX::SimpleMath::Vector3 m_upvec;

	//ƒvƒƒWƒFƒNƒVƒ‡ƒ“s—ñ
	DirectX::SimpleMath::Matrix m_camera_proj;

	//c•ûŒü‹–ìŠp
	float m_fovY;
	//ƒAƒXƒyƒNƒg”ä
	float m_aspect;
	//ƒjƒAƒNƒŠƒbƒv
	float m_nearClip;
	//ƒtƒ@[ƒNƒŠƒbƒv
	float m_farClip;
};