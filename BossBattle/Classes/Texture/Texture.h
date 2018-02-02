//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:�摜�N���X
//
//day:2017/07/26
//
//acotr:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/

#pragma once
#include "..\..\pch.h"
#include <d3d11.h>
#include <SimpleMath.h>

class Texture
{
public:
    Texture(const wchar_t* filename);
    virtual ~Texture() {}

    //�`��
    void Render();

    //�ʒu�̐ݒ�
    void SetPos(DirectX::SimpleMath::Vector2 pos) { m_pos = pos; }
    //4���̏��̐ݒ�
    void SetRect(const RECT& rect) { m_rect = rect; }
    //�\����\���̐ݒ�
    void SetVisible(bool visible) { m_visible = visible; }
    //�e�N�X�`���̐ݒ�
    void SetTexture(const wchar_t* filename);
    //�F�̐ݒ�
    void SetColor(const DirectX::SimpleMath::Color& color) { m_color = color; }

    //4���̏��̎擾
    const RECT& GetRect() { return m_rect; }
    //�e�N�X�`�����̎擾
    const CD3D11_TEXTURE2D_DESC& GetDesc() { return m_desc; }
    //�\����Ԃ̎擾
    bool GetVisible() { return m_visible; }
    //�F�̎擾
    const DirectX::SimpleMath::Color& GetColor() { return m_color; }
    //�ʒu�̎擾
    const DirectX::SimpleMath::Vector2& GetPos() { return m_pos; }

private:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture; //�e�N�X�`�����

    DirectX::SimpleMath::Vector2 m_pos;			                //���W
    DirectX::SimpleMath::Vector2 m_origin;		                //�T�C�Y

    CD3D11_TEXTURE2D_DESC m_desc;				                //�摜���

    RECT m_rect;								                //�`��͈�

    bool m_visible;								                //�\����\��

    DirectX::SimpleMath::Color m_color;			                //�F���
};

