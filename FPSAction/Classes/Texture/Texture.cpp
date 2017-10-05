#include "Texture.h"
#include "../../DXTKResouces.h"
#include "../../Singleton.h"
#include <WICTextureLoader.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

Texture::Texture(const wchar_t* filename)
{
	DXTK::DXTKResources& dxtk = singleton<DXTK::DXTKResources>::GetInstance();

	Microsoft::WRL::ComPtr<ID3D11Resource> resource;

	DX::ThrowIfFailed(CreateWICTextureFromFile(dxtk.m_device, filename, resource.GetAddressOf(), m_texture.ReleaseAndGetAddressOf()));

	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
	DX::ThrowIfFailed(resource.As(&tex));

	tex->GetDesc(&m_desc);

	m_origin.x = float(m_desc.Width / 2);
	m_origin.y = float(m_desc.Height / 2);

	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = m_desc.Width;
	m_rect.bottom = m_desc.Height;

	m_color = Colors::White;

	m_visible = true;
}

Texture::~Texture()
{
}

/// <summary>
/// ï`âÊ
/// </summary>
void Texture::Render()
{
	DXTK::DXTKResources& dxtk = singleton<DXTK::DXTKResources>::GetInstance();

	if(m_visible)
		dxtk.m_sprite_batch->Draw(m_texture.Get(), m_pos, &m_rect, m_color,0.0f, m_origin);
}

/// <summary>
/// âÊëúÇÃê›íË
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
