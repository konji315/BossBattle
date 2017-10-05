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

#include "Classes\Camera\Camera.h"

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
	//virtual ~Obj3d();

	//モデルをロード
	void LoadModel(const wchar_t* filename);

	//オブジェクトのライティングを無効に
	void DisableLighting();

	//更新
	void Update();

	//描画
	void Render();

	//スケーリング(XYZ)
	void SetScaling(const DirectX::SimpleMath::Vector3& scale) {m_obj_scale = scale;};
	void SetScaling(const float radius) { m_obj_scale = DirectX::SimpleMath::Vector3(radius); }
	//平行移動(XYZ)
	void SetPos(const DirectX::SimpleMath::Vector3& pos) {m_obj_pos = pos;};
	//親の３Dオブジェクトのセット
	void SetParent(Obj3d* parent) {m_parent = parent;};
	//回転角(XYZ)
	void SetAngle(const DirectX::SimpleMath::Vector3& angle) { m_obj_rot = angle; m_UseQuternion = false;};
	//回転角(クォータニオン)
	void SetAngleQ(const DirectX::SimpleMath::Quaternion& angle) { m_obj_rotQ = angle; m_UseQuternion = true; };
	//ワールド座標
	void SetWorld(const DirectX::SimpleMath::Matrix& world) { m_obj_world = world; }


	//ワールド行列
	const DirectX::SimpleMath::Matrix& GetWorld() {return m_obj_world;};
	//スケーリング(XYZ)
	const DirectX::SimpleMath::Vector3& GetScale() {return m_obj_scale;};
	//回転角(XYZ)
	const DirectX::SimpleMath::Vector3& GetAngle() {return m_obj_rot;};
	//平行移動(XYZ)
	const DirectX::SimpleMath::Vector3& GetPos() {return m_obj_pos;};
	//親オブジェクトの取得
	Obj3d* GetParent() {return m_parent;};
	////ワールドの取得
	//const DirectX::SimpleMath::Matrix& GetWorld() { return m_obj_world; }

private:
	//メンバ変数

	//3Dモデル
	std::unique_ptr<DirectX::Model> m_model;
	//ワールド行列
	DirectX::SimpleMath::Matrix m_obj_world;
	//スケーリング(XYZ)
	DirectX::SimpleMath::Vector3 m_obj_scale;
	//回転角(XYZ:オイラー角)
	DirectX::SimpleMath::Vector3 m_obj_rot;
	//回転角（クォータニオン）
	DirectX::SimpleMath::Quaternion m_obj_rotQ;
	//平行移動(XYZ)
	DirectX::SimpleMath::Vector3 m_obj_pos;
	//親の3Dオブジェクトのポインタ
	Obj3d* m_parent;
	//回転をクォータニオンで持つかどうか
	bool m_UseQuternion;
};

