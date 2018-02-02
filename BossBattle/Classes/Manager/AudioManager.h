//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:�T�E���h�}�l�[�W���[�N���X
//
//day:2017/07/28
//
//acotr:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/

#pragma once
#include "..\Audio\ADX2Le.h"
#include "..\Audio\PlaySounds.h"
#include "..\Utility\Singleton.h"

class AudioManager : public singleton<AudioManager>
{
private:
    AudioManager() {};

public:
    friend class singleton<AudioManager>;
    //������
    void Initialize()
    {
        ADX2Le::Initialize("Resources/Audio/TPSAudio.acf");

        ADX2Le::LoadAcb("Resources/Audio/PlaySounds.acb", "Resources/Audio/PlaySounds.awb");
    }

    //�X�V
    void Update()
    {
        ADX2Le::Update();
    }

    //�폜
    void Destroy()
    {
        ADX2Le::Finalize();
    }

    //�Đ�
    static const CriAtomExPlaybackId PlaySounds(int id, float volume = 1.0f)
    {
        return ADX2Le::Play(id, volume);
    }

    //��~
    void StopSound()
    {
        ADX2Le::Stop();
    }
};

