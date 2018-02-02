#include "IPlayerInput.h"
#include "..\..\Common\DXTKResouces.h"
#include "..\..\Utility\Singleton.h"
#include <SimpleMath.h>
#include <d3d11.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// �R���X�g���N�^
/// </summary>
IPlayerInput::IPlayerInput()
{
    m_button[Button::A] = new JumpCommand();        //�W�����v�R�}���h
    m_button[Button::B] = new NonCommand();         //�R�}���h����
    m_button[Button::X] = new LightAttackCommand(); //��U���R�}���h
    m_button[Button::Y] = new NonCommand();         //�R�}���h����
    m_button[Button::LB] = new EvadeCommand();      //����R�}���h
    m_button[Button::RB] = new RunCommand();        //����R�}���h
    m_button[Button::LT] = nullptr;                 //�ݒ薳��
    m_button[Button::RT] = nullptr;                 //�ݒ薳��
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
IPlayerInput::~IPlayerInput()
{
    //�{�^���폜
    for (int i = 0; i < Button::NUM; i++)
    {
        if (m_button[i] != nullptr)
            delete m_button[i];
    }
}

/// <summary>
/// �v���C���[�̓���
/// </summary>
/// <param name="player">�v���C���[</param>
void IPlayerInput::ButtonInput(Player* player)
{
    DXTK::DXTKResources& dxtk = singleton<DXTK::DXTKResources>::GetInstance();

    if (player != nullptr)
    {
        auto pad_state = dxtk.m_gamepad->GetState(0);

        //�Q�[���p�b�h�ڑ���
        if (pad_state.IsConnected())
        {
            //A���������Ƃ�
            if (dxtk.m_button_tracker->a == GamePad::ButtonStateTracker::PRESSED)
                m_button[Button::A]->execute(player);
            //B���������Ƃ�
            if (dxtk.m_button_tracker->b == GamePad::ButtonStateTracker::PRESSED)
                m_button[Button::B]->execute(player);
            //X���������Ƃ�
            if (dxtk.m_button_tracker->x == GamePad::ButtonStateTracker::PRESSED)
                m_button[Button::X]->execute(player);
            //Y���������Ƃ�
            if (dxtk.m_button_tracker->y == GamePad::ButtonStateTracker::PRESSED)
                m_button[Button::Y]->execute(player);
            //L���������Ƃ�
            if (dxtk.m_button_tracker->leftShoulder == GamePad::ButtonStateTracker::PRESSED)
                m_button[Button::LB]->execute(player);

            //�����ԏ�����
            player->Run(false);

            //R�������Ă����
            if (pad_state.buttons.rightShoulder)
                m_button[Button::RB]->execute(player);

            //�ړ��ʂ̐ݒ�
            player->SetHorizontal(pad_state.thumbSticks.leftX, pad_state.thumbSticks.leftY);
        }
        //�L�[�{�[�h����(�ً}�p)
        else
        {
            //�L�[�{�[�h�̓��͏�Ԃ̎擾
            Keyboard::State key_state = dxtk.m_keyboard->GetState();

            //Z�L�[�ŃW�����v
            if (dxtk.m_key_tracker->IsKeyPressed(Keyboard::Z))
                m_button[Button::A]->execute(player);
            //X�L�[�ōU��
            if (dxtk.m_key_tracker->IsKeyPressed(Keyboard::X))
                m_button[Button::X]->execute(player);
            //C�L�[�ŉ��
            if (dxtk.m_key_tracker->IsKeyPressed(Keyboard::C))
                m_button[Button::LB]->execute(player);


            //�����ԏ�����
            player->Run(true);

            //�ړ������x�N�g���̐ݒ�
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
/// A�{�^���̐ݒ�
/// </summary>
/// <param name="A">�R�}���h</param>
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
/// B�{�^���̐ݒ�
/// </summary>
/// <param name="B">�R�}���h</param>
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
/// X�{�^���̐ݒ�
/// </summary>
/// <param name="X">�R�}���h</param>
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
/// Y�{�^���̐ݒ�
/// </summary>
/// <param name="Y">�R�}���h</param>
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
/// LB�{�^���̐ݒ�
/// </summary>
/// <param name="LB">�R�}���h</param>
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
/// RB�{�^���̐ݒ�
/// </summary>
/// <param name="RB">�R�}���h</param>
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
/// LT�{�^���̐ݒ�
/// </summary>
/// <param name="LT">�R�}���h</param>
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
/// RT�{�^���̐ݒ�
/// </summary>
/// <param name="RT">�R�}���h</param>
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