//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:3D�I�u�W�F�N�g�̃N���X
//
//day:2017/04/29
//
//acotr:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
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
	//�ÓI�����o�ϐ��̏�����
	static void InitializeStatic(
		Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext,
		Camera* camera);

private:
	//�ÓI�����o�ϐ�
	//�f�o�C�X
	static Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;

	//�f�o�C�X�R���e�L�X�g
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;

	//�J����
	static Camera* m_camera;

	//�ėp�`��X�e�[�g
	static std::unique_ptr<DirectX::CommonStates> m_states;

	////�G�t�F�N�g�t�@�N�g��
	static std::unique_ptr<DirectX::EffectFactory> m_factory;



public:
	Obj3d();
	//virtual ~Obj3d();

	//���f�������[�h
	void LoadModel(const wchar_t* filename);

	//�I�u�W�F�N�g�̃��C�e�B���O�𖳌���
	void DisableLighting();

	//�X�V
	void Update();

	//�`��
	void Render();

	//�X�P�[�����O(XYZ)
	void SetScaling(const DirectX::SimpleMath::Vector3& scale) {m_obj_scale = scale;};
	void SetScaling(const float radius) { m_obj_scale = DirectX::SimpleMath::Vector3(radius); }
	//���s�ړ�(XYZ)
	void SetPos(const DirectX::SimpleMath::Vector3& pos) {m_obj_pos = pos;};
	//�e�̂RD�I�u�W�F�N�g�̃Z�b�g
	void SetParent(Obj3d* parent) {m_parent = parent;};
	//��]�p(XYZ)
	void SetAngle(const DirectX::SimpleMath::Vector3& angle) { m_obj_rot = angle; m_UseQuternion = false;};
	//��]�p(�N�H�[�^�j�I��)
	void SetAngleQ(const DirectX::SimpleMath::Quaternion& angle) { m_obj_rotQ = angle; m_UseQuternion = true; };
	//���[���h���W
	void SetWorld(const DirectX::SimpleMath::Matrix& world) { m_obj_world = world; }


	//���[���h�s��
	const DirectX::SimpleMath::Matrix& GetWorld() {return m_obj_world;};
	//�X�P�[�����O(XYZ)
	const DirectX::SimpleMath::Vector3& GetScale() {return m_obj_scale;};
	//��]�p(XYZ)
	const DirectX::SimpleMath::Vector3& GetAngle() {return m_obj_rot;};
	//���s�ړ�(XYZ)
	const DirectX::SimpleMath::Vector3& GetPos() {return m_obj_pos;};
	//�e�I�u�W�F�N�g�̎擾
	Obj3d* GetParent() {return m_parent;};
	////���[���h�̎擾
	//const DirectX::SimpleMath::Matrix& GetWorld() { return m_obj_world; }

private:
	//�����o�ϐ�

	//3D���f��
	std::unique_ptr<DirectX::Model> m_model;
	//���[���h�s��
	DirectX::SimpleMath::Matrix m_obj_world;
	//�X�P�[�����O(XYZ)
	DirectX::SimpleMath::Vector3 m_obj_scale;
	//��]�p(XYZ:�I�C���[�p)
	DirectX::SimpleMath::Vector3 m_obj_rot;
	//��]�p�i�N�H�[�^�j�I���j
	DirectX::SimpleMath::Quaternion m_obj_rotQ;
	//���s�ړ�(XYZ)
	DirectX::SimpleMath::Vector3 m_obj_pos;
	//�e��3D�I�u�W�F�N�g�̃|�C���^
	Obj3d* m_parent;
	//��]���N�H�[�^�j�I���Ŏ����ǂ���
	bool m_UseQuternion;
};

