//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:DXTKResource�N���X
//
//day:2017/07/16
//
//acotr:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/

#pragma once

#include <Keyboard.h>
#include <Mouse.h>
#include <SpriteFont.h>
#include <CommonStates.h>
#include <GamePad.h>
#include "..\Utility\Singleton.h"

namespace DXTK
{
    class DXTKResources :public singleton<DXTKResources>
    {
    private:
        DXTKResources();

    public:
        friend class singleton<DXTKResources>;

        ~DXTKResources();

        void Initializer(ID3D11Device* device, ID3D11DeviceContext* context);

        void UpdateInputState();

        // �f�o�C�X
        ID3D11Device* m_device;

        // �R���e�L�X�g
        ID3D11DeviceContext* m_context;

        // �L�[�{�[�h�֌W
        std::unique_ptr<DirectX::Keyboard> m_keyboard;
        std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_key_tracker;

        // �}�E�X�֌W
        std::unique_ptr<DirectX::Mouse> m_mouse;
        std::unique_ptr<DirectX::Mouse::ButtonStateTracker> m_mouse_tracker;

        // �R�����X�e�[�g
        std::unique_ptr<DirectX::CommonStates> m_state;

        // �X�v���C�g�o�b�`
        std::unique_ptr<DirectX::SpriteBatch> m_sprite_batch;

        // �X�v���C�g�t�H���g
        std::unique_ptr<DirectX::SpriteFont> m_sprite_font;

        //�Q�[���p�b�h
        std::unique_ptr<DirectX::GamePad> m_gamepad;
        std::unique_ptr<DirectX::GamePad::ButtonStateTracker> m_button_tracker;
    };
}


