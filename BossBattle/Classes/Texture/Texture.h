//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:‰æ‘œƒNƒ‰ƒX
//
//day:2017/07/26
//
//acotr:Koshida Kenji
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/

#pragma once
#include "..\..\pch.h"
#include <d3d11.h>
#include <SimpleMath.h>

class Texture
{
public:
    Texture(const wchar_t* filename);
    virtual ~Texture() {}

    //•`‰æ
    void Render();

    //ˆÊ’u‚Ìİ’è
    void SetPos(DirectX::SimpleMath::Vector2 pos) { m_pos = pos; }
    //4‹÷‚Ìî•ñ‚Ìİ’è
    void SetRect(const RECT& rect) { m_rect = rect; }
    //•\¦”ñ•\¦‚Ìİ’è
    void SetVisible(bool visible) { m_visible = visible; }
    //ƒeƒNƒXƒ`ƒƒ‚Ìİ’è
    void SetTexture(const wchar_t* filename);
    //F‚Ìİ’è
    void SetColor(const DirectX::SimpleMath::Color& color) { m_color = color; }

    //4‹÷‚Ìî•ñ‚Ìæ“¾
    const RECT& GetRect() { return m_rect; }
    //ƒeƒNƒXƒ`ƒƒî•ñ‚Ìæ“¾
    const CD3D11_TEXTURE2D_DESC& GetDesc() { return m_desc; }
    //•\¦ó‘Ô‚Ìæ“¾
    bool GetVisible() { return m_visible; }
    //F‚Ìæ“¾
    const DirectX::SimpleMath::Color& GetColor() { return m_color; }
    //ˆÊ’u‚Ìæ“¾
    const DirectX::SimpleMath::Vector2& GetPos() { return m_pos; }

private:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture; //ƒeƒNƒXƒ`ƒƒî•ñ

    DirectX::SimpleMath::Vector2 m_pos;			                //À•W
    DirectX::SimpleMath::Vector2 m_origin;		                //ƒTƒCƒY

    CD3D11_TEXTURE2D_DESC m_desc;				                //‰æ‘œî•ñ

    RECT m_rect;								                //•`‰æ”ÍˆÍ

    bool m_visible;								                //•\¦”ñ•\¦

    DirectX::SimpleMath::Color m_color;			                //Fî•ñ
};

