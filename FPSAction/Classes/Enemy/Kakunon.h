//Å`/Å`/Å`//Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/
//Content:ÉJÉNÉmÉìÉNÉâÉX
//
//day:2017/07/10
//
//acotr:Koshida Kenji
//Å`/Å`/Å`//Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/
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