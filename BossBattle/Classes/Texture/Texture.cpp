#include "Texture.h"
#include "..\Common\DXTKResouces.h"
#include "..\Utility\Singleton.h"
#include <WICTextureLoader.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

Texture::Texture(const wchar_t* filename)
{
    DXTK::DXTKResources& dxtk = singleton<DXTK::DXTKResources>::GetInstance();

    Microsoft::WRL::ComPtr<ID3D11Resource> resource;

    //テクスチャの読み込み
    DX::ThrowIfFailed(CreateWICTextureFromFile(dxtk.m_device, filename, resource.GetAddressOf(), m_texture.ReleaseAndGetAddressOf()));

    //テクスチャ情報の取得
    Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
    DX::ThrowIfFailed(resource.As(&tex));
    tex->GetDesc(&m_desc);

    m_origin.x = float(m_desc.Width / 2);
    m_origin.y = float(m_desc.Height / 2);

    //描画範囲の取得
    m_rect.left = 0;
    m_rect.top = 0;
    m_rect.right = m_desc.Width;
    m_rect.bottom = m_desc.Height;

    //色設定
    m_color = Colors::White;

    //表示
    m_visible = true;
}

/// <summary>
/// 描画
/// </summary>
void Texture::Render()
{
    DXTK::DXTKResources& dxtk = singleton<DXTK::DXTKResources>::GetInstance();

    if(m_visible)
        dxtk.m_sprite_batch->Draw(m_texture.Get(), m_pos, &m_rect, m_color,0.0f, m_origin);
}

/// <summary>
/// 画像の設定
/// </summary>
void Texture::SetTexture(const wchar_t* filename)
{
    DXTK::DXTKResources& dxtk = singleton<DXTK::DXTKResources>::GetInstance();

    if (m_texture != nullptr)
    {
        m_texture.Reset();
        m_texture = nullptr;
    }

    DX::ThrowIfFailed(CreateWICTextureFromFile(dxtk.m_device, filename,nullptr,m_texture.ReleaseAndGetAddressOf()));

}
