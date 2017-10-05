#include "IPlayerInput.h"
#include "../../DXTKResouces.h"
#include <SimpleMath.h>
#include "../../Singleton.h"
#include <d3d11.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// コンストラクタ
/// </summary>
IPlayerInput::IPlayerInput()
{
	m_button[Button::A] = new JumpCommand();
	m_button[Button::B] = new NonCommand();
	m_button[Button::X] = new LightAttackCommand();
	m_button[Button::Y] = new HeavyAttackCommand();
	m_button[Button::LB] = nullptr;
	m_button[Button::RB] = nullptr;
	m_button[Button::LT] = nullptr;
	m_button[Button::RT] = nullptr;
}

/// <summary>
/// デストラクタ
/// </summary>
IPlayerInput::~IPlayerInput()
{
	for(int i = 0;i < Button::NUM;i++)
	{
		if (m_button[i] != nullptr)
			delete m_button[i];
	}
}

/// <summary>
/// プレイヤーの入力
/// </summary>
/// <param name="player">プレイヤー</param>
void IPlayerInput::ButtonInput(Player* player)
{
	DXTK::DXTKResources& dxtk = singleton<DXTK::DXTKResources>::GetInstance();
	auto pad_state = dxtk.m_gamepad->GetState(0);

	if (player != nullptr)
	{
		//ゲームパッド接続中
		if (pad_state.IsConnected())
		{
			//Aを押したとき
			if (dxtk.m_button_tracker->a == GamePad::ButtonStateTracker::PRESSED)
				m_button[Button::A]->execute(player);
			//Bを押したとき
			if (dxtk.m_button_tracker->b == GamePad::ButtonStateTracker::PRESSED)
				m_button[Button::B]->execute(player);
			//Xを押したとき
			if (dxtk.m_button_tracker->x == GamePad::ButtonStateTracker::PRESSED)
				m_button[Button::X]->execute(player);
			//Yを押したとき
			if (dxtk.m_button_tracker->y == GamePad::ButtonStateTracker::PRESSED)
				m_button[Button::Y]->execute(player);

			//移動量の設定
			player->SetHorizontal(pad_state.thumbSticks.leftX, pad_state.thumbSticks.leftY);

			//右スティックを左右に入力中
			if ((pad_state.thumbSticks.rightX))
				player->Rotation(Vector3(0.0f,-XMConvertToRadians(pad_state.thumbSticks.rightX * Player::TURN_SPEED),0.0f));

			float add_angle = XMConvertToDegrees(player->GetAngle().x) + pad_state.thumbSticks.rightY * Player::TURN_SPEED;

			//右スティックを上下に入力中
			if ((pad_state.thumbSticks.rightY))
			{
				//上下の視線の制限
				if (add_angle > -20 && add_angle < 60)
					player->Rotation(Vector3(XMConvertToRadians(pad_state.thumbSticks.rightY * Player::TURN_SPEED),0.0f,0.0f));
			}

			//Lを押している間ダッシュ可能に
			if (pad_state.buttons.leftShoulder)
				player->Run(true);
			else
				player->Run(false);
		}
	}
}


#pragma region SetCommand


/// <summary>
/// Aボタンの設定
/// </summary>
/// <param name="A">コマンド</param>
void IPlayerInput::SetCmdButtonA(Command * A)
{
	if (m_button[Button::A] != nullptr)
	{
		delete m_button[Button::A];
		m_button[Button::A] = nullptr;
	}
	m_button[Button::A] = A;
}

/// <summary>
/// Bボタンの設定
/// </summary>
/// <param name="B">コマンド</param>
void IPlayerInput::SetCmdButtonB(Command * B)
{
	if (m_button[Button::B] != nullptr)
	{
		delete m_button[Button::B];
		m_button[Button::B] = nullptr;
	}
	m_button[Button::B] = B;

}

/// <summary>
/// Xボタンの設定
/// </summary>
/// <param name="X">コマンド</param>
void IPlayerInput::SetCmdButtonX(Command * X)
{
	if (m_button[Button::X] != nullptr)
	{
		delete m_button[Button::X];
		m_button[Button::X] = nullptr;
	}
	m_button[Button::X] = X;
}

/// <summary>
/// Yボタンの設定
/// </summary>
/// <param name="Y">コマンド</param>
void IPlayerInput::SetCmdButtonY(Command * Y)
{
	if (m_button[Button::Y] != nullptr)
	{
		delete m_button[Button::Y];
		m_button[Button::Y] = nullptr;
	}
	m_button[Button::Y] = Y;

}

/// <summary>
/// LBボタンの設定
/// </summary>
/// <param name="LB">コマンド</param>
void IPlayerInput::SetCmdButtonLB(Command * LB)
{
	if (m_button[Button::LB] != nullptr)
	{
		delete m_button[Button::LB];
		m_button[Button::LB] = nullptr;
	}
	m_button[Button::A] = LB;

}

/// <summary>
/// RBボタンの設定
/// </summary>
/// <param name="RB">コマンド</param>
void IPlayerInput::SetCmdButtonRB(Command * RB)
{
	if (m_button[Button::RB] != nullptr)
	{
		delete m_button[Button::RB];
		m_button[Button::RB] = nullptr;
	}
	m_button[Button::RB] = RB;
}

/// <summary>
/// LTボタンの設定
/// </summary>
/// <param name="LT">コマンド</param>
void IPlayerInput::SetCmdButtonLT(Command * LT)
{
	if (m_button[Button::LT] != nullptr)
	{
		delete m_button[Button::LT];
		m_button[Button::LT] = nullptr;
	}
	m_button[Button::LT] = LT;

}

/// <summary>
/// RTボタンの設定
/// </summary>
/// <param name="RT">コマンド</param>
void IPlayerInput::SetCmdButtonRT(Command * RT)
{
	if (m_button[Button::RT] != nullptr)
	{
		delete m_button[Button::RT];
		m_button[Button::RT] = nullptr;
	}
	m_button[Button::RT] = RT;
}


#pragma endregion
