#include "CollisionNode.h"

using namespace DirectX;

using namespace DirectX::SimpleMath;

bool CollisionNode::m_visible = false;

void CollisionNode::SetParent(Obj3d * parent)
{
    m_Obj.SetParent(parent);
}

/// <summary>
/// コンストラクタ
/// </summary>
SphereNode::SphereNode()
{
    m_local_radius = 1.0f;
}

/// <summary>
/// 初期化
/// </summary>
void SphereNode::Initialize()
{
    m_Obj.LoadModel(L"Resources/SphereNode.cmo");
}

/// <summary>
/// 更新
/// </summary>
void SphereNode::Update()
{
    m_Obj.SetPos(m_offset);
    m_Obj.SetScaling(m_local_radius);
    m_Obj.SetBoneWorld(m_bone_world);

    //ワールド行列の更新
    m_Obj.Update();

    {//ワールド行列での当たり判定球の計算
        //オブジェクトのワールド行列を取得

        const Matrix& worldm = m_Obj.GetWorld();

        //モデル座標系の中心
        Vector3 center(0, 0, 0);

        //モデル座標系の右端
        Vector3 right(1, 0, 0);

        center = Vector3::Transform(center, worldm);
        right = Vector3::Transform(right, worldm);

        Sphere::center = center;
        Sphere::radius = Vector3::Distance(center, right);
    }
}

/// <summary>
/// 描画
/// </summary>
void SphereNode::Render()
{
    if (m_visible)
        m_Obj.Render();
}
