//～/～/～//～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/
//Content:3Dオブジェクトのクラス
//
//day:2017/04/29
//
//acotr:Koshida Kenji
//～/～/～//～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/
#pragma once
#include <windows.h>
#include <wrl/client.h>
#include <d3d11_1.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>

#include "AnimationModel.h"
#include "..\Camera\Camera.h"

class Obj3d
{
public:
    //静的メンバ変数の初期化
    static void InitializeStatic(
        Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice,
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext,
        Camera* camera);

private:
    //静的メンバ変数
    //デバイス
    static Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;

    //デバイスコンテキスト
    static Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;

    //カメラ
    static Camera* m_camera;

    //汎用描画ステート
    static std::unique_ptr<DirectX::CommonStates> m_states;

    ////エフェクトファクトリ
    static std::unique_ptr<DirectX::EffectFactory> m_factory;
public:
    Obj3d();

    //モデルをロード
    void LoadModel(const wchar_t* filename);

    //オブジェクトのライティングを無効に
    void DisableLighting();

    //更新
    void Update();
    //アニメーションの更新
    void UpdateAnimation(const std::wstring& clip_name);

    //描画
    void Render();

    //スケーリング(XYZ)
    void SetScaling(const DirectX::SimpleMath::Vector3& scale) { m_obj_scale = scale; }
    void SetScaling(const float radius) { m_obj_scale = DirectX::SimpleMath::Vector3(radius); }
    //平行移動(XYZ)
    void SetPos(const DirectX::SimpleMath::Vector3& pos) { m_obj_pos = pos; }
    //親の３Dオブジェクトのセット
    void SetParent(Obj3d* parent) { m_parent = parent; }
    //ボーン情報のセット
    void SetBoneWorld(const DirectX::SimpleMath::Matrix& boneworld) { m_bone_world = boneworld; }
    //回転角(XYZ)
    void SetAngle(const DirectX::SimpleMath::Vector3& angle) { m_obj_rot = angle; m_use_quternion = false; }
    //回転角(クォータニオン)
    void SetAngleQ(const DirectX::SimpleMath::Quaternion& angle) { m_obj_rotQ = angle; m_use_quternion = true; }
    //ワールド座標
    void SetWorld(const DirectX::SimpleMath::Matrix& world) { m_obj_world = world; }


    //ワールド行列
    const DirectX::SimpleMath::Matrix& GetWorld() { return m_obj_world; }
    //スケーリング(XYZ)
    const DirectX::SimpleMath::Vector3& GetScale() { return m_obj_scale; }
    //回転角(XYZ)
    const DirectX::SimpleMath::Vector3& GetAngle() { return m_obj_rot; }
    //平行移動(XYZ)
    const DirectX::SimpleMath::Vector3& GetPos() { return m_obj_pos; }
    //親オブジェクトの取得
    Obj3d* GetParent() { return m_parent; };
    ////ワールドの取得
    //const DirectX::SimpleMath::Matrix& GetWorld() { return m_obj_world; }
    //ボーン情報の取得
    DirectX::SimpleMath::Matrix GetBoneWorld(int bonenum);
    //アニメーション終了かどうか
    const bool IsAnim(const std::wstring& clip_name);

    //アニメーション時間を初期化
    void ResetAnimation() { m_model->m_anim_state.animTime = 0.0f; }

    //モデルの色を変える
    void ChangeModelColor(const DirectX::XMVECTORF32& color);
    //モデルの色をリセット
    void ResetColor();
private:
    std::unique_ptr<AnimationModel> m_model;	//3Dモデル

    DirectX::SimpleMath::Matrix m_obj_world;	//ワールド行列
    DirectX::SimpleMath::Vector3 m_obj_scale;	//スケーリング(XYZ)
    DirectX::SimpleMath::Vector3 m_obj_rot;	    //回転角(XYZ:オイラー角)
    DirectX::SimpleMath::Quaternion m_obj_rotQ;	//回転角（クォータニオン）
    DirectX::SimpleMath::Vector3 m_obj_pos;	    //平行移動(XYZ)
    Obj3d* m_parent;                        	//親の3Dオブジェクトのポインタ
    DirectX::SimpleMath::Matrix m_bone_world;	//追跡するボーン
    bool m_use_quternion;                    	//回転をクォータニオンで持つかどうか
};

