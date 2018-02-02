#include "Canvas.h"
#include "..\Utility\Singleton.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

/// /// <summary>
/// �摜�̃Z�b�g
/// </summary>
/// <param name="tag">�^�O��</param>
/// <param name="filename">�t�@�C����</param>
void Canvas::SetTexture(string tag, const wchar_t * filename)
{
    m_textures[tag] = std::make_unique<Texture>(filename);
}

/// <summary>
/// �w�肵���^�O�̉摜�̍��W��ύX
/// </summary>
/// <param name="tag">�^�O</param>
/// <param name="pos">���W</param>
void Canvas::SetTexturePos(std::string tag, const DirectX::SimpleMath::Vector2 & pos)
{
    m_textures[tag]->SetPos(pos);
}

/// <summary>
/// �͈͂̐ݒ�
/// </summary>
/// <param name="tag">�^�O</param>
/// <param name="rect">�͈�</param>
void Canvas::SetRect(std::string tag, const RECT & rect)
{
    m_textures[tag]->SetRect(rect);
}

/// <summary>
/// �\����\��
/// </summary>
/// <param name="tag">�^�O</param>
/// <param name="visible">�\������\����</param>
void Canvas::SetVisible(std::string tag, bool visible)
{
    m_textures[tag]->SetVisible(visible);
}

/// <summary>
/// �F�̃Z�b�g
/// </summary>
/// <param name="tag">�^�O</param>
/// <param name="color">�F</param>
void Canvas::SetColor(std::string tag, const Color& color)
{
    m_textures[tag]->SetColor(color);
}

/// <summary>
/// �͈͂̎擾
/// </summary>
/// <param name="tag">�^�O</param>
/// <returns>�͈�</returns>
const RECT & Canvas::GetRect(std::string tag)
{
    return m_textures[tag]->GetRect();
}

/// <summary>
/// �摜���̎擾
/// </summary>
/// <param name="tag">�^�O</param>
/// <returns>�摜���</returns>
const CD3D11_TEXTURE2D_DESC & Canvas::GetDesc(std::string tag)
{
    return m_textures[tag]->GetDesc();
}

/// <summary>
/// �\�����ǂ���
/// </summary>
/// <param name="tag">�^�O</param>
/// <returns>�\����\��</returns>
bool Canvas::GetVisible(std::string tag)
{
    return m_textures[tag]->GetVisible();
}

/// <summary>
/// �F�̎擾
/// </summary>
/// <returns>�F</returns>
const Color& Canvas::GetColor(std::string tag)
{
    return m_textures[tag]->GetColor();
}

/// <summary>
/// ���W�̎擾
/// </summary>
const DirectX::SimpleMath::Vector2 & Canvas::GetPos(std::string tag)
{
    return m_textures[tag]->GetPos();
}

/// <summary>
/// �`��
/// </summary>
void Canvas::Render()
{
    for (auto itr = m_textures.begin(); itr != m_textures.end(); itr++)
        itr->second->Render();
}
