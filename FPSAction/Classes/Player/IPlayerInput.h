//Å`/Å`/Å`//Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/
//Content:ÉvÉåÉCÉÑÅ[ÇÃÉCÉìÉ^Å[ÉtÉFÅ[ÉX
//
//day:2017/07/13
//
//acotr:Koshida Kenji
//Å`/Å`/Å`//Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/

#pragma once
#include "../../Command.h"
#include "Player.h"

class IPlayerInput
{
	enum Button
	{
		A,
		B,
		X,
		Y,
		LB,
		RB,
		LT,
		RT,

		NUM
	};

public:
	IPlayerInput();
	virtual ~IPlayerInput();

	void ButtonInput(Player* player);

	void SetCmdButtonA(Command* A) ;
	void SetCmdButtonB(Command* B) ;
	void SetCmdButtonX(Command* X) ;
	void SetCmdButtonY(Command* Y) ;
	void SetCmdButtonLB(Command* LB);
	void SetCmdButtonRB(Command* RB);
	void SetCmdButtonLT(Command* LT);
	void SetCmdButtonRT(Command* RT);

private:
	Command* m_button[Button::NUM];
};

