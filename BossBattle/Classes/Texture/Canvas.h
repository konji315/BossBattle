//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:UI�`��N���X
//
//day:2017/07/26
//
//acotr:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
#pragma once
#include <map>
#include <string>
#include "Texture.h"

class Canvas
{
public:
    Canvas() {}
    virtual ~Canvas() {}

    //�e�N�X�`���̐ݒ�
    void SetTexture(std::string tag, const wchar_t*filename);
    //�e�N�X�`���̈ʒu�̐ݒ�
    void SetTexturePos(std::string tag, const DirectX::SimpleMath::Vector2& pos);
    //�`��͈͂̐ݒ�
    void SetRect(std::string tag, const RECT& rect);
    //�\����\���̐ݒ�
    void SetVisible(std::string tag, bool visible);
    //�F�̐ݒ�
    void SetColor(std::string tag, const DirectX::SimpleMath::Color& color);

    //�`��͈͂̎擾
    const RECT& GetRect(std::string tag);
    //�e�N�X�`�����̎擾
    const CD3D11_TEXTURE2D_DESC& GetDesc(std::string tag);
    //�\����Ԃ̎擾
    bool GetVisible(std::string tag);
    //�F�̎擾
    const DirectX::SimpleMath::Color& GetColor(std::string tag);
    //�ʒu�̎擾
    const DirectX::SimpleMath::Vector2& GetPos(std::string tag);

    //�`��
    void Render();
private:
    std::map<std::string, std::unique_ptr<Texture>> m_textures; //�e�N�X�`���Q
};