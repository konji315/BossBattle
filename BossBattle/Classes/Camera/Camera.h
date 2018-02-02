//～/～/～//～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/
//Content:カメラ基底クラス
//
//day:2017/05/11
//
//acotr:Koshida Kenji
//～/～/～//～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/
#pragma once

#include "..\..\pch.h"
#include <SimpleMath.h>

class Camera
{
public:
    //コンストラクタ
    Camera(int width, int height);

    virtual ~Camera() {}

    virtual void Update();

    //ビュー行列取得
    const DirectX::SimpleMath::Matrix& Camera::GetViewMatrix() { return m_camera_view; }
    //プロジェクション行列の取得
    const DirectX::SimpleMath::Matrix& Camera::GetProjectionMatrix() { return m_camera_proj; }

    //カメラ座標の設定
    void SetEyePos(const DirectX::SimpleMath::Vector3& eyepos)  { m_eyepos = eyepos; }
    //参照点の設定
    void SetRefPos(const DirectX::SimpleMath::Vector3& refpos)  { m_refpos = refpos; }
    //上方向ベクトルの設定
    void SetUpvecSet(const DirectX::SimpleMath::Vector3& upvec) { m_upvec = upvec; }
    //視野角の設定
    void SetFovY(float fovY) { m_fovY = fovY; }
    //アスペクト比の設定
    void SetAspect(float aspect) { m_aspect = aspect; }

    void SetNearClip(float near_clip) { m_near_clip = near_clip; }
    void SetfarClip(float far_clip) { m_far_clip = far_clip; }

    //カメラ座標の取得
    const DirectX::SimpleMath::Vector3& GetEyePos() { return m_eyepos; };
    //参照点の取得
    const DirectX::SimpleMath::Vector3& GetRefPos() { return m_refpos; };
    //上方向ベクトルの取得
    const DirectX::SimpleMath::Vector3& GetUpvec() { return m_upvec; };
    //視野角の取得
    float GetFov() { return m_fovY; };

    float GetNearClip() { return m_near_clip; };
    float GetFar() { return m_far_clip; };

    //ビルボード行列の取得
    const DirectX::SimpleMath::Matrix& GetBillboard() const { return m_billboard; }
    const DirectX::SimpleMath::Matrix& GetBillboardConstrainY() const { return m_billboard_constrainY; }

    //ビルボード行列の計算
    void CalcBillboard();

    //カメラを振動
    void ShakeCamera(int power) { m_shake_power = power; }
protected:
    DirectX::SimpleMath::Matrix m_camera_view;    //ビュー行列
    DirectX::SimpleMath::Vector3 m_eyepos;        //視点
    DirectX::SimpleMath::Vector3 m_refpos;        //参照点
    DirectX::SimpleMath::Vector3 m_upvec;         //上方向ベクトル
    DirectX::SimpleMath::Matrix m_camera_proj;    //プロジェクション行列

    float m_fovY;      //縦方向視野角
    float m_aspect;    //アスペクト比
    float m_near_clip;  //ニアクリップ
    float m_far_clip;   //ファークリップ

    DirectX::SimpleMath::Matrix m_billboard;             // ビルボード行列
    DirectX::SimpleMath::Matrix m_billboard_constrainY;   // ビルボード行列(Y軸周り限定）

    int m_shake_power;  //振動
};