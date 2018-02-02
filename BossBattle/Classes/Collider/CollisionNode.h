//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:�����蔻��̃f�o�b�O�\���Ǝ����v�Z
//
//day:2017/06/19
//
//actor:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/

#pragma once
#include "Collision.h"
#include "..\Model\Obj3d.h"

class CollisionNode
{
protected:
    //�\����\��
    static bool m_visible;

public:
    CollisionNode() { m_bone_world = DirectX::SimpleMath::Matrix::Identity; }

    virtual void Initialize() = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;

    //�e���Z�b�g
    void SetParent(Obj3d* parent);
    //�{�[���s����Z�b�g
    void SetBoneWorld(const DirectX::SimpleMath::Matrix& world) { m_bone_world = world; }

    //�I�t�Z�b�g�̃Z�b�g
    void SetOffset(DirectX::SimpleMath::Vector3& offset) { m_offset = offset; }
    //�\���t���O�̃Z�b�g
    static void SetDebugVisible(bool flag) { m_visible = flag; };
    //�\���t���O�̃Q�b�g
    static bool GetDebugVisible() { return m_visible; };
protected:
    //�f�o�b�O�\���p�I�u�W�F�N�g
    Obj3d m_Obj;

    //�e�ƂȂ�{�[���s��
    DirectX::SimpleMath::Matrix m_bone_world;

    //�e����̃I�t�Z�b�g
    DirectX::SimpleMath::Vector3 m_offset;
};

/// <summary>
/// ���̂����蔻��m�[�h
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
    //���[�J�����a
    float m_local_radius;
};