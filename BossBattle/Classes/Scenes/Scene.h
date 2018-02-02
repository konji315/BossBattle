//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:�V�[���̊��N���X
//
//day:2017/06/28
//
//acotr:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/

#pragma once
#include <memory>
#include "..\Camera\BossCamera.h"

class Scene
{
public:
    virtual ~Scene() {};

    virtual void Enter(int width, int height, Camera* camera) = 0;

    virtual std::unique_ptr<Scene> Update() = 0;
    virtual void Render() = 0;
protected:
    int m_window_width;		//��ʂ̉���
    int m_window_height;	//��ʂ̏c��s
};

