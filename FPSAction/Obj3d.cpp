#include "pch.h"
#include "Obj3d.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//デバイス
Microsoft::WRL::ComPtr<ID3D11Device> Obj3d::m_d3dDevice;

//デバイスコンテキスト
Microsoft::WRL::ComPtr<ID3D11DeviceContext>     Obj3d::m_d3dContext;

//カメラ
Camera* Obj3d::m_camera;

//汎用描画ステート
std::unique_ptr<DirectX::CommonStates> Obj3d::m_states;

////エフェクトファクトリ
std::unique_ptr<DirectX::EffectFactory> Obj3d::m_factory;


/// <summary>
/// 初期化
/// </summary>
/// <param name="d3dDevice">デバイス</param>
/// <param name="d3dContext">デバイスコンテキスト</param>
/// <param name="m_camera">カメラ</param>
void Obj3d::InitializeStatic(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, Camera * camera)
{
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;
	m_camera = camera;

	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());

	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	//テクスチャの読み込みパス
	m_factory->SetDirectory(L"Resources");

}

/// <summary>
/// コンストラクタ
/// </summary>
Obj3d::Obj3d()
{
	m_obj_scale = Vector3(1, 1, 1);
	m_parent = nullptr;
	m_UseQuternion = false;
}

/// <summary>
/// デストラクタ
/// </summary>
//Obj3d::~Obj3d()
//{
//}

/// <summary>
/// モデルのロード
/// </summary>
/// <param name="filename">ファイルパス</param>
void Obj3d::LoadModel(const wchar_t * filename)
{
	m_model = Model::CreateFromCMO(m_d3dDevice.Get(),
		filename,
		*m_factory);
}

void Obj3d::DisableLighting()
{
	if (m_model)
	{
		// モデル内の全メッシュ分回す
		ModelMesh::Collection::const_iterator it_mesh = m_model->meshes.begin();
		for (; it_mesh != m_model->meshes.end(); it_mesh++)
		{
			ModelMesh* modelmesh = it_mesh->get();
			assert(modelmesh);

			// メッシュ内の全メッシュパーツ分回す
			std::vector<std::unique_ptr<ModelMeshPart>>::iterator it_meshpart = modelmesh->meshParts.begin();
			for (; it_meshpart != modelmesh->meshParts.end(); it_meshpart++)
			{
				ModelMeshPart* meshpart = it_meshpart->get();
				assert(meshpart);

				// メッシュパーツにセットされたエフェクトをBasicEffectとして取得
				std::shared_ptr<IEffect> ieff = meshpart->effect;
				BasicEffect* eff = dynamic_cast<BasicEffect*>(ieff.get());
				if (eff != nullptr)
				{
					// 自己発光を最大値に
					eff->SetEmissiveColor(Vector3(1, 1, 1));

					// エフェクトに含む全ての平行光源分について処理する
					for (int i = 0; i < BasicEffect::MaxDirectionalLights; i++)
					{
						// ライトを無効にする
						eff->SetLightEnabled(i, false);
					}
				}
			}
		}
	}
}

/// <summary>
/// 更新
/// </summary>
void Obj3d::Update()
{
	Matrix scalemat = Matrix::CreateScale(m_obj_scale);
	Matrix rotmat;

	if (m_UseQuternion)
	{//クォータニオンに回転を計算

		rotmat = Matrix::CreateFromQuaternion(m_obj_rotQ);
	}
	else
	{//オイラー角で回転を計算
		Matrix rotmatZ = Matrix::CreateRotationZ(m_obj_rot.z);
		Matrix rotmatX = Matrix::CreateRotationX(m_obj_rot.x);
		Matrix rotmatY = Matrix::CreateRotationY(m_obj_rot.y);
		rotmat = rotmatZ * rotmatX* rotmatY;
	}

	Matrix transmat = Matrix::CreateTranslation(m_obj_pos);

	m_obj_world = scalemat * rotmat * transmat;

	if(m_parent)
		m_obj_world *= m_parent->GetWorld();
}

/// <summary>
/// 描画
/// </summary>
void Obj3d::Render()
{
	assert(m_camera);

	if(m_model)
		m_model->Draw(m_d3dContext.Get(), *m_states.get(), m_obj_world, m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());
}