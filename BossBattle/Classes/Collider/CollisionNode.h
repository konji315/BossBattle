//〜/〜/〜//〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/
//Content:当たり判定のデバッグ表示と自動計算
//
//day:2017/06/19
//
//actor:Koshida Kenji
//〜/〜/〜//〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/〜/

#pragma once
#include "Collision.h"
#include "..\Model\Obj3d.h"

class CollisionNode
{
protected:
    //表示非表示
    static bool m_visible;

public:
    CollisionNode() { m_bone_world = DirectX::SimpleMath::Matrix::Identity; }

    virtual void Initialize() = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;

    //親をセット
    void SetParent(Obj3d* parent);
    //ボーン行列をセット
    void SetBoneWorld(const DirectX::SimpleMath::Matrix& world) { m_bone_world = world; }

    //オフセットのセット
    void SetOffset(DirectX::SimpleMath::Vector3& offset) { m_offset = offset; }
    //表示フラグのセット
    static void SetDebugVisible(bool flag) { m_visible = flag; };
    //表示フラグのゲット
    static bool GetDebugVisible() { return m_visible; };
protected:
    //デバッグ表示用オブジェクト
    Obj3d m_Obj;

    //親となるボーン行列
    DirectX::SimpleMath::Matrix m_bone_world;

    //親からのオフセット
    DirectX::SimpleMath::Vector3 m_offset;
};

/// <summary>
/// 球のあたり判定ノード
/// </summary>
class SphereNode : public CollisionNode, public Sphere
{
public:
    SphereNode();

    void Initialize();
    void Update();
    void Render();

    void SetLocalRadius(float radius) { m_local_radius = radius; }
protected:
    //ローカル半径
    float m_local_radius;
};