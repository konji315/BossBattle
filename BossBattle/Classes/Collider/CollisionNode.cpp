#include "CollisionNode.h"

using namespace DirectX;

using namespace DirectX::SimpleMath;

bool CollisionNode::m_visible = false;

void CollisionNode::SetParent(Obj3d * parent)
{
    m_Obj.SetParent(parent);
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
SphereNode::SphereNode()
{
    m_local_radius = 1.0f;
}

/// <summary>
/// ������
/// </summary>
void SphereNode::Initialize()
{
    m_Obj.LoadModel(L"Resources/SphereNode.cmo");
}

/// <summary>
/// �X�V
/// </summary>
void SphereNode::Update()
{
    m_Obj.SetPos(m_offset);
    m_Obj.SetScaling(m_local_radius);
    m_Obj.SetBoneWorld(m_bone_world);

    //���[���h�s��̍X�V
    m_Obj.Update();

    {//���[���h�s��ł̓����蔻�苅�̌v�Z
        //�I�u�W�F�N�g�̃��[���h�s����擾

        const Matrix& worldm = m_Obj.GetWorld();

        //���f�����W�n�̒��S
        Vector3 center(0, 0, 0);

        //���f�����W�n�̉E�[
        Vector3 right(1, 0, 0);

        center = Vector3::Transform(center, worldm);
        right = Vector3::Transform(right, worldm);

        Sphere::center = center;
        Sphere::radius = Vector3::Distance(center, right);
    }
}

/// <summary>
/// �`��
/// </summary>
void SphereNode::Render()
{
    if (m_visible)
        m_Obj.Render();
}
