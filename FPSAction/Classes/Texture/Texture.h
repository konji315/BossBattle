//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:‰æ‘œƒNƒ‰ƒX
//
//day:2017/07/26
//
//acotr:Koshida Kenji
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/

#pragma once
#include "../../pch.h"
#include <d3d11.h>
#include <SimpleMath.h>

class Texture
{
public:
	Texture(const wchar_t* filename);
	virtual ~Texture();

	void Render();

	void SetPos(DirectX::SimpleMath::Vector2 pos) { m_pos = pos; }
	void SetRect(const RECT& rect) { m_rect = rect; }
	void SetVisible(bool visible) { m_visible = visible; }
	void SetTexture(const wchar_t* filename);
	void SetColor(const DirectX::SimpleMath::Color& color) { m_color = color; }

	const RECT& GetRect() { return m_rect; }
	const CD3D11_TEXTURE2D_DESC& GetDesc() { return m_desc; }
	bool GetVisible() { return m_visible; }
	const DirectX::SimpleMath::Color& GetColor() { return m_color; }

	const DirectX::SimpleMath::Vector2& GetPos() { return m_pos; }

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	DirectX::SimpleMath::Vector2 m_pos;			//À•W
	DirectX::SimpleMath::Vector2 m_origin;		//ƒTƒCƒY

	CD3D11_TEXTURE2D_DESC m_desc;				//‰æ‘œî•ñ

	RECT m_rect;								//•`‰æ”ÍˆÍ

	bool m_visible;								//•\¦”ñ•\¦

	DirectX::SimpleMath::Color m_color;						//Fî•ñ
};

