//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:Particle Effect Manager
//
//day:2017/11/02
//
//actor:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/

#include <vector>
#include <Windows.h>
#include <wrl\client.h>
#include <memory>

#include <d3d11.h>

#include <SimpleMath.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <CommonStates.h>
#include <string>
#include <map>

#include "..\Camera\Camera.h"
#include "..\Common\DXTKResouces.h"
#include "..\Utility\BinaryFile.h"

#pragma once

class ParticleEmitter
{
public:
	enum EmitterType
	{
		CONE,		//�R�[��
		SPHERE		//�X�t�B�A
	};
public:
	//�R���X�g���N�^
	ParticleEmitter() {}
	//�f�X�g���N�^
	~ParticleEmitter()
	{
		//swap�����ɂ�郁�����̉��
		std::vector<DirectX::VertexPositionColorTexture>().swap(m_Vertices);
		std::vector<DirectX::SimpleMath::Vector3>().swap(m_MoveVec);
		std::vector<float>().swap(m_LifeTimer);
		std::vector<float>().swap(m_GravInf);
	}

	//������
	void Initialize(
		const float duration							                //��������
		, const DirectX::SimpleMath::Vector3 pos		                //�ʒu
		, const DirectX::SimpleMath::Vector2 rotate		                //���ˊp�x
		, const wchar_t* filepath						                //�e�N�X�`���̃p�X
		, const float startrotate						                //�����p�x
		, const float endrotate							                //�ŏI�p�x
		, const float startscale = 1.0f					                //�����T�C�Y
		, const float endscale = 1.0f					                //�ŏI�T�C�Y
		, const DirectX::SimpleMath::Vector4 startcolor = { 1,1,1,1 }	//�����J���[
		, const DirectX::SimpleMath::Vector4 endcolor = { 1,1,1,1 }     //�ŏI�J���[
		, const float lifetime = 5						                //����
		, const float startspeed = 5					                //�����x
		, const float endspeed = 5						                //�ŏI���x
		, const float gravity = 0						                //�d��
		, const float angle = 25						                //���ˊp�x
		, const int ratetime = 10						                //���˃��[�g
		, const bool burst = false						                //��Ĕ���
		, const bool loop = true										//���[�v
		, const EmitterType type = EmitterType::CONE);			        //���˕��@

	//�X�V
	void Update();
	//�`��
	void Render(DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>* pb);

	/// <summary>
	/// �Z�b�^�[
	/// </summary>
	void SetPos(const DirectX::SimpleMath::Vector3& pos) { m_Pos = pos; }
	void SetRotate(const DirectX::SimpleMath::Vector2& rotate) { m_Rotate = rotate; }
	void SetChild(std::string child_name) { m_child_emitter_name = child_name; }

private:
	//�^�C�}�[
	int m_PlayBackTimer;
	//�p�[�e�B�N��
	std::vector<DirectX::VertexPositionColorTexture> m_Vertices;
	//�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_Texture;
	//�ړ�����
	std::vector<DirectX::SimpleMath::Vector3> m_MoveVec;
	//�p�[�e�B�N���X�̎c�����
	std::vector<float> m_LifeTimer;
	//�d�͉e���x
	std::vector<float> m_GravInf;

	//��������
	float m_Duration;
	//�����ʒu
	DirectX::SimpleMath::Vector3 m_Pos;
	//�ŏ��̐F
	DirectX::SimpleMath::Vector4 m_StartColor;
	//�Ō�̐F
	DirectX::SimpleMath::Vector4 m_EndColor;
	//�����p�x
	DirectX::SimpleMath::Vector2 m_Rotate;
	//�ŏ��̃p�[�e�B�N���̊p�x
	float m_StartRotate;
	//�Ō�̃p�[�e�B�N���̊p�x
	float m_EndRotate;
	//�ŏ��̑傫��
	float m_StartScale;
	//�Ō�̑傫��
	float m_EndScale;
	//����
	float m_LifeTime;
	//���[�v
	bool m_Loop;
	//�ŏ��̃p�[�e�B�N�����x
	float m_StartSpeed;
	//�Ō�̃p�[�e�B�N�����x
	float m_EndSpeed;
	//�d��
	float m_Gravity;
	//���˔͈�
	float m_Angle;
	//�o�[�X�g
	bool m_Burst;
	//���˃��[�g
	int m_Rate;
	//���˕��@
	EmitterType m_Type;
	//�q���̖��O
	std::string m_child_emitter_name;

	//�p�[�e�B�N���̐���
	void CreateParticle();
};


class ParticleEffectManager : public singleton<ParticleEffectManager>
{
public:
	static const int PARTICLE_NUM_MAX;

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	struct Constants
	{
		//���[���h�r���[�v���W�F�N�V���������s��
		DirectX::SimpleMath::Matrix WVP;
		DirectX::SimpleMath::Matrix BILLBOARD;
	};

public:
	friend class singleton<ParticleEffectManager>;

	ParticleEffectManager();
	virtual ~ParticleEffectManager();

	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Render();

	//�p�[�e�B�N���G�~�b�^�[����
	ParticleEmitter* Play(std::string name, DirectX::SimpleMath::Vector3 pos, float lifetime, std::string childname = "");

	/// <summary>
	/// �G�~�b�^�[�̑���
	/// </summary>
	void SetCamera(Camera* camera) { m_Camera = camera; }

	//�p�[�e�B�N���o�^
	void RegisterParticle(std::string name , ParticleEmitter* pe);

	//�e�N�X�`���Q
	std::map<const wchar_t*, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>	  m_TextureResources;

private:
	//���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader>                                    m_VertexShader;
	//�W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>                                  m_GeometryShader;
	//�s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader>                                     m_PixelShader;
	//�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_PrimitiveBatch;
	//���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout>                                     m_InputLayout;
	//�ėp�`��ݒ�
	std::unique_ptr<DirectX::CommonStates>                                        m_CommonStates;
	//�E�B���h�E�T�C�Y
	RECT                                                                          m_WindowSize;
	//�R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>                                          m_ConstantsBuffer;
	//�e�N�X�`���T���v��
	Microsoft::WRL::ComPtr<ID3D11SamplerState>                                    m_Sampler;
	//�u�����h�X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11BlendState>									  m_BlendState;
	//�J����
	Camera*															  m_Camera;
	//�o�^�p�[�e�B�N���Q
	std::map<std::string,ParticleEmitter*>										  m_RegisteredEmitters;
	//�X�V�p�[�e�B�N���Q
	std::vector<ParticleEmitter*>												  m_ParticleEmitters;
	//�G�~�b�^�[�̎c�����
	std::vector<float>															  m_LifeTimer;
};


//class Particle
//{
//public:
//
//private:
//	DirectX::SimpleMath::Vector2 m_MoveVec;	//�ړ�����
//};