//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:ƒtƒ@ƒCƒA[ƒ{[ƒ‹ƒNƒ‰ƒX
//
//day:2017/07/19
//
//acotr:Koshida Kenji
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
#pragma once
#include "Magic.h"

class FireBall : public Magic
{
	static const float MOVE_SPEED;		//ƒtƒ@ƒCƒA[ƒ{[ƒ‹‚ÌˆÚ“®‘¬“x
public:
	FireBall(const DirectX::SimpleMath::Matrix& mat);
	~FireBall();

	void Update()override;
	void Render()override;

private:
	DirectX::SimpleMath::Vector3 m_attack_vec;
};

