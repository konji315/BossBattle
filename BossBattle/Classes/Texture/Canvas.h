//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:UI`æNX
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

    //eNX`ÌÝè
    void SetTexture(std::string tag, const wchar_t*filename);
    //eNX`ÌÊuÌÝè
    void SetTexturePos(std::string tag, const DirectX::SimpleMath::Vector2& pos);
    //`æÍÍÌÝè
    void SetRect(std::string tag, const RECT& rect);
    //\¦ñ\¦ÌÝè
    void SetVisible(std::string tag, bool visible);
    //FÌÝè
    void SetColor(std::string tag, const DirectX::SimpleMath::Color& color);

    //`æÍÍÌæ¾
    const RECT& GetRect(std::string tag);
    //eNX`îñÌæ¾
    const CD3D11_TEXTURE2D_DESC& GetDesc(std::string tag);
    //\¦óÔÌæ¾
    bool GetVisible(std::string tag);
    //FÌæ¾
    const DirectX::SimpleMath::Color& GetColor(std::string tag);
    //ÊuÌæ¾
    const DirectX::SimpleMath::Vector2& GetPos(std::string tag);

    //`æ
    void Render();
private:
    std::map<std::string, std::unique_ptr<Texture>> m_textures; //eNX`Q
};