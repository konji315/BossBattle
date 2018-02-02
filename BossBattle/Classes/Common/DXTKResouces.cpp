#include "..\..\pch.h"
#include "DXTKResouces.h"

using namespace DirectX;

/// <summary>
/// コンストラクタ
/// </summary>
DXTK::DXTKResources::DXTKResources()
{
    //キーボード
    m_keyboard = std::make_unique<Keyboard>();
    m_key_tracker = std::make_unique<Keyboard::KeyboardStateTracker>();
    //マウス
    m_mouse = std::make_unique<Mouse>();
    m_mouse_tracker = std::make_unique<Mouse::ButtonStateTracker>();
    //ゲームパッド
    m_gamepad = std::make_unique<GamePad>();
    m_button_tracker = std::make_unique<GamePad::ButtonStateTracker>();
}

DXTK::DXTKResources::~DXTKResources()
{
}

/// <summary>
/// リソースの初期化
/// </summary>
/// <param name="device">デバイス</param>
/// <param name="context">コンテキスト</param>
void DXTK::DXTKResources::Initializer(ID3D11Device* device, ID3D11DeviceContext* context)
{
    m_device = device;
    m_context = context;

    // コモンステートを作成
    m_state = std::make_unique<CommonStates>(device);

    // スプライトバッチ
    m_sprite_batch = std::make_unique<SpriteBatch>(context);

    // スプライトフォント
    m_sprite_font = std::make_unique<SpriteFont>(device, L"msgothic.spritefont");
}

/// <summary>
/// リソースの更新
/// </summary>
void DXTK::DXTKResources::UpdateInputState()
{
    // キー入力情報を取得
    Keyboard::State key = m_keyboard->GetState();
    m_key_tracker->Update(key);

    // マウス情報を取得
    Mouse::State mouse = m_mouse->GetState();
    m_mouse_tracker->Update(mouse);

    //ボタンの情報を取得
    GamePad::State pad = m_gamepad->GetState(0);
    m_button_tracker->Update(pad);
}
