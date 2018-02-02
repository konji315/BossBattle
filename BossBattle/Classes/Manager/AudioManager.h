//Å`/Å`/Å`//Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/
//Content:ÉTÉEÉìÉhÉ}ÉlÅ[ÉWÉÉÅ[ÉNÉâÉX
//
//day:2017/07/28
//
//acotr:Koshida Kenji
//Å`/Å`/Å`//Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/Å`/

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
    //èâä˙âª
    void Initialize()
    {
        ADX2Le::Initialize("Resources/Audio/TPSAudio.acf");

        ADX2Le::LoadAcb("Resources/Audio/PlaySounds.acb", "Resources/Audio/PlaySounds.awb");
    }

    //çXêV
    void Update()
    {
        ADX2Le::Update();
    }

    //çÌèú
    void Destroy()
    {
        ADX2Le::Finalize();
    }

    //çƒê∂
    static const CriAtomExPlaybackId PlaySounds(int id, float volume = 1.0f)
    {
        return ADX2Le::Play(id, volume);
    }

    //í‚é~
    void StopSound()
    {
        ADX2Le::Stop();
    }
};

