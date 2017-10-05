#pragma once
#include "../Magic/Magic.h"
class BossBall : public Magic
{
	static const float MOVE_SPEED;		//�{�[���̈ړ����x
public:
	BossBall(const DirectX::SimpleMath::Matrix& mat);
	~BossBall();

	void Update()override;
	void Render()override;

private:
	DirectX::SimpleMath::Vector3 m_move_vec;	//�ړ������x�N�g��
};

