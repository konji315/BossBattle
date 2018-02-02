#include "Canvas.h"
#include "..\Utility\Singleton.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

/// /// <summary>
/// 画像のセット
/// </summary>
/// <param name="tag">タグ名</param>
/// <param name="filename">ファイル名</param>
void Canvas::SetTexture(string tag, const wchar_t * filename)
{
    m_textures[tag] = std::make_unique<Texture>(filename);
}

/// <summary>
/// 指定したタグの画像の座標を変更
/// </summary>
/// <param name="tag">タグ</param>
/// <param name="pos">座標</param>
void Canvas::SetTexturePos(std::string tag, const DirectX::SimpleMath::Vector2 & pos)
{
    m_textures[tag]->SetPos(pos);
}

/// <summary>
/// 範囲の設定
/// </summary>
/// <param name="tag">タグ</param>
/// <param name="rect">範囲</param>
void Canvas::SetRect(std::string tag, const RECT & rect)
{
    m_textures[tag]->SetRect(rect);
}

/// <summary>
/// 表示非表示
/// </summary>
/// <param name="tag">タグ</param>
/// <param name="visible">表示か非表示か</param>
void Canvas::SetVisible(std::string tag, bool visible)
{
    m_textures[tag]->SetVisible(visible);
}

/// <summary>
/// 色のセット
/// </summary>
/// <param name="tag">タグ</param>
/// <param name="color">色</param>
void Canvas::SetColor(std::string tag, const Color& color)
{
    m_textures[tag]->SetColor(color);
}

/// <summary>
/// 範囲の取得
/// </summary>
/// <param name="tag">タグ</param>
/// <returns>範囲</returns>
const RECT & Canvas::GetRect(std::string tag)
{
    return m_textures[tag]->GetRect();
}

/// <summary>
/// 画像情報の取得
/// </summary>
/// <param name="tag">タグ</param>
/// <returns>画像情報</returns>
const CD3D11_TEXTURE2D_DESC & Canvas::GetDesc(std::string tag)
{
    return m_textures[tag]->GetDesc();
}

/// <summary>
/// 表示かどうか
/// </summary>
/// <param name="tag">タグ</param>
/// <returns>表示非表示</returns>
bool Canvas::GetVisible(std::string tag)
{
    return m_textures[tag]->GetVisible();
}

/// <summary>
/// 色の取得
/// </summary>
/// <returns>色</returns>
const Color& Canvas::GetColor(std::string tag)
{
    return m_textures[tag]->GetColor();
}

/// <summary>
/// 座標の取得
/// </summary>
const DirectX::SimpleMath::Vector2 & Canvas::GetPos(std::string tag)
{
    return m_textures[tag]->GetPos();
}

/// <summary>
/// 描画
/// </summary>
void Canvas::Render()
{
    for (auto itr = m_textures.begin(); itr != m_textures.end(); itr++)
        itr->second->Render();
}
