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
    m_use_quternion = false;
    m_model = nullptr;
    m_bone_world = Matrix::Identity;
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
    m_model = AnimationModel::AnimCreateFromCMO(m_d3dDevice.Get(),
        filename,
        *m_factory);
    m_model->m_anim_state.m_bone_world_transforms.resize(m_model->m_bones.size());
}

/// <summary>
/// ライティングを切る
/// </summary>
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

    if (m_use_quternion)
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

    //親行列に帰属
    if (m_parent)
        m_obj_world *= m_bone_world * m_parent->GetWorld();
}

/// <summary>
/// アニメーションの変更
/// </summary>
/// <param name="clip_name">アニメーション名</param>
void Obj3d::UpdateAnimation(const std::wstring & clip_name)
{
    if (m_model)
        m_model->UpdateAnimation(clip_name);
}

/// <summary>
/// 描画
/// </summary>
void Obj3d::Render()
{
    assert(m_camera);

    if (m_model)
        m_model->Draw(m_d3dContext.Get(), *m_states.get(), m_obj_world, m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());
}

/// <summary>
/// ボーン行列の取得
/// </summary>
/// <param name="bonenum">ボーン番号</param>
/// <returns>ボーン行列</returns>
DirectX::SimpleMath::Matrix Obj3d::GetBoneWorld(int bonenum)
{
    DirectX::XMMATRIX mat = XMLoadFloat4x4(&(m_model->m_anim_state.m_bone_world_transforms[bonenum]));

    return mat;
}

/// <summary>
/// アニメーション中かどうか
/// </summary>
/// <param name="clip_name">クリップ名</param>
const bool Obj3d::IsAnim(const std::wstring & clip_name)
{
    auto found = m_model->m_clips.find(clip_name);
    if (found != m_model->m_clips.end())
    {
        //アニメーション終了時
        if (m_model->m_anim_state.end)
            return false;
    }
    return true;
}

/// <summary>
/// モデルの色を変える
/// </summary>
/// <param name="color">色</param>
void Obj3d::ChangeModelColor(const XMVECTORF32& color)
{

    m_model->UpdateEffects([&](IEffect* effect)
    {
        auto lights = dynamic_cast<IEffectLights*>(effect);
        if (lights)
        {
            lights->SetAmbientLightColor(color);
            lights->SetLightDiffuseColor(0, color);
            lights->SetLightSpecularColor(0, color);
        }
    });
}

/// <summary>
/// 色を元に戻す
/// </summary>
void Obj3d::ResetColor()
{
    m_model->UpdateEffects([&](IEffect* effect)
    {
        auto lights = dynamic_cast<IEffectLights*>(effect);
        if (lights)
            lights->EnableDefaultLighting();
    });
}
