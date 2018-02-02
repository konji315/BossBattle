#include "IPlayerInput.h"
#include "..\..\Common\DXTKResouces.h"
#include "..\..\Utility\Singleton.h"
#include <SimpleMath.h>
#include <d3d11.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// コンストラクタ
/// </summary>
IPlayerInput::IPlayerInput()
{
    m_button[Button::A] = new JumpCommand();        //ジャンプコマンド
    m_button[Button::B] = new NonCommand();         //コマンド無し
    m_button[Button::X] = new LightAttackCommand(); //弱攻撃コマンド
    m_button[Button::Y] = new NonCommand();         //コマンド無し
    m_button[Button::LB] = new EvadeCommand();      //回避コマンド
    m_button[Button::RB] = new RunCommand();        //走りコマンド
    m_button[Button::LT] = nullptr;                 //設定無し
    m_button[Button::RT] = nullptr;                 //設定無し
}

/// <summary>
/// デストラクタ
/// </summary>
IPlayerInput::~IPlayerInput()
{
    //ボタン削除
    for (int i = 0; i < Button::NUM; i++)
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

    if (player != nullptr)
    {
        auto pad_state = dxtk.m_gamepad->GetState(0);

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
            //Lを押したとき
            if (dxtk.m_button_tracker->leftShoulder == GamePad::ButtonStateTracker::PRESSED)
                m_button[Button::LB]->execute(player);

            //走り状態初期化
            player->Run(false);

            //Rを押している間
            if (pad_state.buttons.rightShoulder)
                m_button[Button::RB]->execute(player);

            //移動量の設定
            player->SetHorizontal(pad_state.thumbSticks.leftX, pad_state.thumbSticks.leftY);
        }
        //キーボード入力(緊急用)
        else
        {
            //キーボードの入力状態の取得
            Keyboard::State key_state = dxtk.m_keyboard->GetState();

            //Zキーでジャンプ
            if (dxtk.m_key_tracker->IsKeyPressed(Keyboard::Z))
                m_button[Button::A]->execute(player);
            //Xキーで攻撃
            if (dxtk.m_key_tracker->IsKeyPressed(Keyboard::X))
                m_button[Button::X]->execute(player);
            //Cキーで回避
            if (dxtk.m_key_tracker->IsKeyPressed(Keyboard::C))
                m_button[Button::LB]->execute(player);


            //走り状態初期化
            player->Run(true);

            //移動方向ベクトルの設定
            Vector2 move_vec;
            if (key_state.Left)
                move_vec.x = -1;
            if (key_state.Right)
                move_vec.x =  1;
            if (key_state.Up)
                move_vec.y =  1;
            if (key_state.Down)
                move_vec.y = -1;

            move_vec.Normalize();
            player->SetHorizontal(move_vec.x, move_vec.y);

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
