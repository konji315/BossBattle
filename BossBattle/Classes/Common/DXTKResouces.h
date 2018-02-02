//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:DXTKResourceƒNƒ‰ƒX
//
//day:2017/07/16
//
//acotr:Koshida Kenji
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/

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

        // ƒfƒoƒCƒX
        ID3D11Device* m_device;

        // ƒRƒ“ƒeƒLƒXƒg
        ID3D11DeviceContext* m_context;

        // ƒL[ƒ{[ƒhŠÖŒW
        std::unique_ptr<DirectX::Keyboard> m_keyboard;
        std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_key_tracker;

        // ƒ}ƒEƒXŠÖŒW
        std::unique_ptr<DirectX::Mouse> m_mouse;
        std::unique_ptr<DirectX::Mouse::ButtonStateTracker> m_mouse_tracker;

        // ƒRƒ‚ƒ“ƒXƒe[ƒg
        std::unique_ptr<DirectX::CommonStates> m_state;

        // ƒXƒvƒ‰ƒCƒgƒoƒbƒ`
        std::unique_ptr<DirectX::SpriteBatch> m_sprite_batch;

        // ƒXƒvƒ‰ƒCƒgƒtƒHƒ“ƒg
        std::unique_ptr<DirectX::SpriteFont> m_sprite_font;

        //ƒQ[ƒ€ƒpƒbƒh
        std::unique_ptr<DirectX::GamePad> m_gamepad;
        std::unique_ptr<DirectX::GamePad::ButtonStateTracker> m_button_tracker;
    };
}


