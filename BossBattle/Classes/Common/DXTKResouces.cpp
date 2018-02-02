#include "..\..\pch.h"
#include "DXTKResouces.h"

using namespace DirectX;

/// <summary>
/// �R���X�g���N�^
/// </summary>
DXTK::DXTKResources::DXTKResources()
{
    //�L�[�{�[�h
    m_keyboard = std::make_unique<Keyboard>();
    m_key_tracker = std::make_unique<Keyboard::KeyboardStateTracker>();
    //�}�E�X
    m_mouse = std::make_unique<Mouse>();
    m_mouse_tracker = std::make_unique<Mouse::ButtonStateTracker>();
    //�Q�[���p�b�h
    m_gamepad = std::make_unique<GamePad>();
    m_button_tracker = std::make_unique<GamePad::ButtonStateTracker>();
}

DXTK::DXTKResources::~DXTKResources()
{
}

/// <summary>
/// ���\�[�X�̏�����
/// </summary>
/// <param name="device">�f�o�C�X</param>
/// <param name="context">�R���e�L�X�g</param>
void DXTK::DXTKResources::Initializer(ID3D11Device* device, ID3D11DeviceContext* context)
{
    m_device = device;
    m_context = context;

    // �R�����X�e�[�g���쐬
    m_state = std::make_unique<CommonStates>(device);

    // �X�v���C�g�o�b�`
    m_sprite_batch = std::make_unique<SpriteBatch>(context);

    // �X�v���C�g�t�H���g
    m_sprite_font = std::make_unique<SpriteFont>(device, L"msgothic.spritefont");
}

/// <summary>
/// ���\�[�X�̍X�V
/// </summary>
void DXTK::DXTKResources::UpdateInputState()
{
    // �L�[���͏����擾
    Keyboard::State key = m_keyboard->GetState();
    m_key_tracker->Update(key);

    // �}�E�X�����擾
    Mouse::State mouse = m_mouse->GetState();
    m_mouse_tracker->Update(mouse);

    //�{�^���̏����擾
    GamePad::State pad = m_gamepad->GetState(0);
    m_button_tracker->Update(pad);
}
