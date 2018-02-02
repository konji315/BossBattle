//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:UI•`‰æƒNƒ‰ƒX
//
//day:2017/07/26
//
//acotr:Koshida Kenji
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
#pragma once
#include <map>
#include <string>
#include "Texture.h"

class Canvas
{
public:
    Canvas() {}
    virtual ~Canvas() {}

    //ƒeƒNƒXƒ`ƒƒ‚Ìİ’è
    void SetTexture(std::string tag, const wchar_t*filename);
    //ƒeƒNƒXƒ`ƒƒ‚ÌˆÊ’u‚Ìİ’è
    void SetTexturePos(std::string tag, const DirectX::SimpleMath::Vector2& pos);
    //•`‰æ”ÍˆÍ‚Ìİ’è
    void SetRect(std::string tag, const RECT& rect);
    //•\¦”ñ•\¦‚Ìİ’è
    void SetVisible(std::string tag, bool visible);
    //F‚Ìİ’è
    void SetColor(std::string tag, const DirectX::SimpleMath::Color& color);

    //•`‰æ”ÍˆÍ‚Ìæ“¾
    const RECT& GetRect(std::string tag);
    //ƒeƒNƒXƒ`ƒƒî•ñ‚Ìæ“¾
    const CD3D11_TEXTURE2D_DESC& GetDesc(std::string tag);
    //•\¦ó‘Ô‚Ìæ“¾
    bool GetVisible(std::string tag);
    //F‚Ìæ“¾
    const DirectX::SimpleMath::Color& GetColor(std::string tag);
    //ˆÊ’u‚Ìæ“¾
    const DirectX::SimpleMath::Vector2& GetPos(std::string tag);

    //•`‰æ
    void Render();
private:
    std::map<std::string, std::unique_ptr<Texture>> m_textures; //ƒeƒNƒXƒ`ƒƒŒQ
};