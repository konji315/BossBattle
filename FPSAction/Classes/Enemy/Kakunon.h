//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:�J�N�m���N���X
//
//day:2017/07/10
//
//acotr:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
#pragma once
#include "Enemy.h"

class Kakunon : public Enemy
{
public:
	Kakunon();
	~Kakunon();

	void Initialize()override;
	void Update()override;
	void Render()override;
};