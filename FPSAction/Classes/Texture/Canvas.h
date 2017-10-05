//Å`/Å`/Å`//Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/
//Content:UIï`âÊÉNÉâÉX
//
//day:2017/07/26
//
//acotr:Koshida Kenji
//Å`/Å`/Å`//Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/
#pragma once
#include <map>
#include <string>
#include "Texture.h"

class Canvas
{
public:
	Canvas();
	virtual ~Canvas();

	void SetTexture(std::string tag, const wchar_t*filename);
	void SetTexturePos(std::string tag, const DirectX::SimpleMath::Vector2& pos);
	void SetRect(std::string tag , const RECT& rect);
	void SetVisible(std::string tag, bool visible);
	void SetColor(std::string tag, const DirectX::SimpleMath::Color& color);

	const RECT& GetRect(std::string tag);
	const CD3D11_TEXTURE2D_DESC& GetDesc(std::string tag);
	bool GetVisible(std::string tag);
	const DirectX::SimpleMath::Color& GetColor(std::string tag);

	void Update();
	void Render();
private:
	std::map<std::string, std::unique_ptr<Texture>> m_textures;
};