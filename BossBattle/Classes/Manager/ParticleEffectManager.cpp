#include "ParticleEffectManager.h"
#include "..\Utility\Singleton.h"
#include <WICTextureLoader.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

const int ParticleEffectManager::PARTICLE_NUM_MAX = 30000;

const std::vector<D3D11_INPUT_ELEMENT_DESC> ParticleEffectManager::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector3) + sizeof(Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

ParticleEffectManager::ParticleEffectManager()
{
}

ParticleEffectManager::~ParticleEffectManager()
{
	std::vector<float>().swap(m_LifeTimer);

	//�p�[�e�B�N���G�~�b�^�[�̍폜
	for (auto pe : m_ParticleEmitters)
	{
		if (pe != nullptr)
		{
			delete pe;
		}
	}

	//�o�^�����G�~�b�^�[�̍폜
	for (auto pe : m_RegisteredEmitters)
	{
		if (pe.second != nullptr)
		{
			delete pe.second;
		}
	}
}

/// <summary>
/// ������
/// </summary>
void ParticleEffectManager::Initialize()
{
	DXTK::DXTKResources& dxtk = singleton<DXTK::DXTKResources>::GetInstance();
	ID3D11Device* device = dxtk.m_device;
	ID3D11DeviceContext* context = dxtk.m_context;

	m_Camera = nullptr;

	// �R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleVS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticlePS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleGS.cso");

	// ���_�V�F�[�_�쐬
	if (FAILED(device->CreateVertexShader(
		VSData.GetData(),
		VSData.GetSize(),
		NULL,
		m_VertexShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	// �s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(
		PSData.GetData(),
		PSData.GetSize(),
		NULL,
		m_PixelShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	// �W�I���g���V�F�[�_�쐬
	if (FAILED(device->CreateGeometryShader(
		GSData.GetData(),
		GSData.GetSize(),
		NULL,
		m_GeometryShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}

	// �v���~�e�B�u�o�b�`�쐬
	//  �e���_�Ɋ܂܂���񂪁A�ʒu�E�F�E�e�N�X�`��UV
	m_PrimitiveBatch =
		std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(
			context, 1, PARTICLE_NUM_MAX);

	// ���_�t�H�[�}�b�g���w�肵�ē��̓��C�A�E�g�쐬
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		INPUT_LAYOUT.size(),
		VSData.GetData(), VSData.GetSize(),
		m_InputLayout.GetAddressOf());

	D3D11_BUFFER_DESC cd;
	cd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cd.ByteWidth = sizeof(Constants);
	cd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cd.MiscFlags = 0;
	cd.StructureByteStride = 0;
	cd.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(device->CreateBuffer(&cd, nullptr, m_ConstantsBuffer.ReleaseAndGetAddressOf())))
	{
		MessageBox(0, L"CreateBuffer Failed.", NULL, MB_OK);
		return;
	}

	D3D11_BLEND_DESC desc;
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_REV_SUBTRACT;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	HRESULT ret = device->CreateBlendState(&desc, &m_BlendState);

	m_CommonStates = std::make_unique<CommonStates>(device);

	//�T���v���̍쐬
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;


	HRESULT hr = device->CreateSamplerState(&sd, m_Sampler.GetAddressOf());

	if (FAILED(hr))
	{
		MessageBox(0, L"CreateSampler Failed.", NULL, MB_OK);
		return;
	}


}

/// <summary>
/// �X�V
/// </summary>
void ParticleEffectManager::Update()
{
	//�����̌���
	for (UINT i = 0; i < m_LifeTimer.size(); i++)
	{
		//-1�Ȃ�Ζ���
		if (m_LifeTimer[i] == -1)
			continue;

		m_LifeTimer[i] -= 1 / 60.0f;
		if (m_LifeTimer[i] <= 0)
		{
			m_LifeTimer.erase(m_LifeTimer.begin() + i);
			delete m_ParticleEmitters[i];
			m_ParticleEmitters[i] = nullptr;
			m_ParticleEmitters.erase(m_ParticleEmitters.begin() + i);
		}
	}

	//�X�̃p�[�e�B�N���̍X�V
	for (auto pe : m_ParticleEmitters)
	{
		pe->Update();
	}
}

/// <summary>
/// �`��
/// </summary>
void ParticleEffectManager::Render()
{
	if (m_ParticleEmitters.empty())
		return;

	DXTK::DXTKResources& dxtk = singleton<DXTK::DXTKResources>::GetInstance();
	ID3D11Device* device = dxtk.m_device;
	ID3D11DeviceContext* context = dxtk.m_context;

	Matrix world = Matrix::Identity;
	Matrix view = m_Camera->GetViewMatrix();
	Matrix proj = m_Camera->GetProjectionMatrix();

#pragma region WVP


	//���s�ړ�
	{
		//m_pos.x += 0.1f;
		//world = Matrix::CreateTranslation(m_pos);
	}

	//��]
	{
		//m_angle.z += XM_1DIV2PI / 2;
		//world = Matrix::CreateRotationZ(m_angle.z);
	}

	//��]�ړ�
	{
		//m_pos.x += 0.1f;
		//m_angle.z += XM_1DIV2PI / 2;
		//world = Matrix::CreateRotationZ(m_angle.z) * Matrix::CreateTranslation(m_pos);
	}

	//�g�k
	{
		//m_angle.z += XM_1DIV2PI / 2;
		//world = Matrix::CreateRotationZ(m_angle.z) * Matrix::CreateScale(sinf(m_angle.z) + 1.0f);
	}

	//�~�^��
	{
		//m_angle.z -= XM_1DIV2PI / 2;

		//m_pos = Vector3(sinf(m_angle.z), cosf(m_angle.z), 0);

		//world = Matrix::CreateTranslation(m_pos);
	}

	//8�̎�
	{
		//m_angle.z -= XM_1DIV2PI / 2;

		//m_pos = Vector3(cosf(m_angle.z), sinf(m_angle.z + m_angle.z), 0);

		//world = Matrix::CreateTranslation(m_pos);
	}

#pragma endregion

	Matrix wvp = world * view * proj;

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(context->Map(m_ConstantsBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		Constants constants;

		constants.WVP = wvp;
		constants.BILLBOARD = m_Camera->GetBillboard();

		memcpy_s(pData.pData, pData.RowPitch, &constants, sizeof(constants));
		context->Unmap(m_ConstantsBuffer.Get(), 0);
	}

	//���̃R���X�^���g�o�b�t�@���ǂ̃V�F�[�_�Ɏg����
	context->VSGetConstantBuffers(0, 0, nullptr);
	context->GSSetConstantBuffers(0, 1, m_ConstantsBuffer.GetAddressOf());
	context->PSGetConstantBuffers(0, 0, nullptr);

	//�f�o�C�X�R���e�L�X�g�ɁA�`��Ɏg�p����V�F�[�_���w��
	context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_GeometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_PixelShader.Get(), nullptr, 0);

	//�`��Ɏg�p������̓��C�A�E�g���w��
	context->IASetInputLayout(m_InputLayout.Get());

	//�w�ʃJ�����O�ݒ�
	context->RSSetState(m_CommonStates->CullNone());

	////�A���t�@�u�����h�ݒ�
	//context->OMSetBlendState(m_CommonStates->NonPremultiplied(), nullptr, 0xffffff);

	//���Z�����`��
	context->OMSetBlendState(m_CommonStates->Additive(), nullptr, 0xffffff);

	////���Z�����`��
	//context->OMSetBlendState(m_blend_state.Get(), nullptr, 0xffffff);

	//�[�x�X�e���V���ݒ�
	context->OMSetDepthStencilState(m_CommonStates->DepthRead(), 0);

	//�T���v���̐ݒ�
	context->PSSetSamplers(0, 1, m_Sampler.GetAddressOf());

	for (auto particle : m_ParticleEmitters)
	{
		particle->Render(m_PrimitiveBatch.get());
	}

	//�V�F�[�_�̃N���A
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

/// <summary>
/// �p�[�e�B�N���G�~�b�^�[����
/// </summary>
/// <param name="name">�o�^�����G�~�b�^�[��</param>
/// <param name="pos">���W</param>
ParticleEmitter* ParticleEffectManager::Play(std::string name, DirectX::SimpleMath::Vector3 pos, float lifetime, std::string childname)
{
	ParticleEmitter* pe = new ParticleEmitter();

	auto emitter = m_RegisteredEmitters.find(name);

	//�o�^����Ă���Ȃ�R�s�[
	if (emitter != m_RegisteredEmitters.end())
		*pe = *emitter->second;
	else
	{
		delete pe;
		pe = nullptr;
		MessageBox(0, L"Particle Notfound.", NULL, MB_OK);
		return nullptr;
	}

	//�ꏊ�̐ݒ�
	pe->SetPos(pos);

	//�q����ݒ肷��Ȃ��
	if (childname != "")
		pe->SetChild(childname);

	m_ParticleEmitters.push_back(pe);
	m_LifeTimer.push_back(lifetime);

	return pe;
}

/// <summary>
/// �p�[�e�B�N���o�^
/// </summary>
/// <param name="pe"></param>
void ParticleEffectManager::RegisterParticle(std::string name, ParticleEmitter * pe)
{
	auto emitter = m_RegisteredEmitters.find(name);

	//�o�^����Ă��Ȃ�������o�^
	if (emitter == m_RegisteredEmitters.end())
		m_RegisteredEmitters[name] = pe;
	else
	{
		//�o�^�ς݂������̂Ńp�[�e�B�N�����폜���A�X�L�b�v
		MessageBox(0, L"Already Registered.", NULL, MB_OK);
		delete pe;
		pe = nullptr;
		return;
	}
}





//////////////////////////////////////////////////////////////////////////////////
//ParticleEmitter Class
//////////////////////////////////////////////////////////////////////////////////

void ParticleEmitter::Initialize(
	  const float duration
	, const DirectX::SimpleMath::Vector3 pos
	, const DirectX::SimpleMath::Vector2 rotate
	, const wchar_t* filepath
	, const float startrotate
	, const float endrotate
	, const float startscale
	, const float endscale
	, const DirectX::SimpleMath::Vector4 startcolor
	, const DirectX::SimpleMath::Vector4 endcolor
	, const float lifetime
	, const float startspeed
	, const float endspeed
	, const float gravity
	, const float angle
	, const int rate
	, const bool burst
	, const bool loop
	, const EmitterType type)
{
	assert((filepath != nullptr) && (lifetime >= 0.01f) && (rate > 0));

	DXTK::DXTKResources& dxtk = singleton<DXTK::DXTKResources>::GetInstance();
	ID3D11Device* device = dxtk.m_device;
	auto& pem = singleton<ParticleEffectManager>::GetInstance();

	m_Duration = duration;
	m_Pos = pos;
	m_Rotate = rotate;
	m_StartRotate = startrotate;
	m_EndRotate = endrotate;
	m_StartScale = startscale;
	m_EndScale = endscale;
	m_StartColor = startcolor;
	m_EndColor = endcolor;
	m_LifeTime = lifetime;
	m_StartSpeed = startspeed;
	m_EndSpeed = endspeed;
	m_Gravity = gravity;
	m_Angle = angle;
	m_Rate = rate;
	m_Burst = burst;
	m_Loop = loop;
	m_Type = type;

	m_PlayBackTimer = 0;

	//���\�[�X����e�N�X�`������
	auto found = pem.m_TextureResources.find(filepath);
	//��������g���܂킵�āA����������ǂݍ���
	if (found != pem.m_TextureResources.end())
	{
		m_Texture = found->second;
	}
	else
	{
		//�e�N�X�`���̓ǂݍ���
		HRESULT hr = CreateWICTextureFromFile(device, filepath, nullptr, m_Texture.GetAddressOf());

		pem.m_TextureResources[filepath] = m_Texture;
	}
}

void ParticleEmitter::Update()
{

	/// <summary>
	/// ���[�v�@�\�Ǝ������Ԃ̐ݒ�
	/// </summary>
	{
		//��������
		if ((m_PlayBackTimer <= m_Duration * 60 || m_Loop))
		{
			if (m_Burst)
			{
				//�������Ԃ��ƂɈ�Ăɔ���
				if (m_PlayBackTimer % (int)(m_Duration * 60) == 0 && (m_PlayBackTimer == 0 || m_Loop))
				{
					for (int i = 0; i < m_Rate; i++)
					{
						CreateParticle();
					}
				}
			}
			else
			{
				//���[�g���ƂɃp�[�e�B�N������
				if (m_PlayBackTimer % (60 / m_Rate) == 0 && m_PlayBackTimer != 0)
				{
					CreateParticle();
				}

			}
		}
	}

	//�p�[�e�B�N���Q���Ǘ�����^�C�}�[
	m_PlayBackTimer++;


	//�d�͉��Z
	for (UINT i = 0; i < m_GravInf.size(); i++)
	{
		m_GravInf[i] += m_Gravity / 100;
	}

	//�p�[�e�B�N���X�̑���
	if (m_Vertices.size() > 0)
	{
		for (UINT i = 0; i < m_Vertices.size();i++)
		{
			/// <summary>
			/// �v�Z���啝�C���_
			/// </summary>
			{
				m_Vertices[i].textureCoordinate.x = (m_EndScale - m_StartScale) - ((m_EndScale - m_StartScale) * (m_LifeTimer[i] / m_LifeTime)) + m_StartScale;
				m_Vertices[i].textureCoordinate.y = (m_EndRotate - m_StartRotate) - ((m_EndRotate - m_StartRotate) * (m_LifeTimer[i] / m_LifeTime)) + m_StartRotate;
				m_Vertices[i].position.x += m_MoveVec[i].x * (((m_EndSpeed - m_StartSpeed) - ((m_EndSpeed - m_StartSpeed) * (m_LifeTimer[i] / m_LifeTime)) + m_StartSpeed) / 100);
				m_Vertices[i].position.y += m_MoveVec[i].y * (((m_EndSpeed - m_StartSpeed) - ((m_EndSpeed - m_StartSpeed) * (m_LifeTimer[i] / m_LifeTime)) + m_StartSpeed) / 100) - m_GravInf[i];
				m_Vertices[i].position.z += m_MoveVec[i].z * (((m_EndSpeed - m_StartSpeed) - ((m_EndSpeed - m_StartSpeed) * (m_LifeTimer[i] / m_LifeTime)) + m_StartSpeed) / 100);
			}
			m_Vertices[i].color = static_cast<Vector4>(DirectX::XMVectorLerp(m_EndColor, m_StartColor, m_LifeTimer[i] / m_LifeTime));
		}
	}


	//�����̌���
	for (UINT i = 0; i < m_LifeTimer.size(); i++)
	{
		m_LifeTimer[i] -= 1 / 60.0f;

		//�������s������p�[�e�B�N���̊e�v�f���폜						���v�N���X��
		if (m_LifeTimer[i] <= 0)
		{
			//�q�����ݒ肳��Ă����ꍇ
			if (m_child_emitter_name != "")
			{
				auto& pem = singleton<ParticleEffectManager>::GetInstance();
				pem.Play(m_child_emitter_name, m_Vertices[i].position, 5);
			}
			m_LifeTimer.erase(m_LifeTimer.begin() + i);
			m_Vertices.erase(m_Vertices.begin() + i);
			m_MoveVec.erase(m_MoveVec.begin() + i);
			m_GravInf.erase(m_GravInf.begin() + i);
		}
	}

}

void ParticleEmitter::Render(DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>* pb)
{
	if (m_Vertices.empty())
		return;

	DXTK::DXTKResources& dxtk = singleton<DXTK::DXTKResources>::GetInstance();
	ID3D11Device* device = dxtk.m_device;
	ID3D11DeviceContext* context = dxtk.m_context;

	//�e�N�X�`���̐ݒ�
	context->PSSetShaderResources(0, 1, m_Texture.GetAddressOf());

	// ���_�f�[�^�����ׂēn���ĕ`�悷��
	pb->Begin();
	pb->Draw(
		D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
		&m_Vertices[0],
		m_Vertices.size());
	pb->End();

}

/// <summary>
/// �X�̃p�[�e�B�N���̐���
/// </summary>
void ParticleEmitter::CreateParticle()
{
		VertexPositionColorTexture vertexData;
		vertexData.position = m_Pos;
		vertexData.color = m_StartColor;
		vertexData.textureCoordinate = Vector2(m_StartScale, 1);

		m_Vertices.push_back(vertexData);

		//���_����
		Vector3 rotate = m_Rotate;

		rotate.Normalize();

		Vector3 rotation;

		//���_�������p�x����]
		rotation.x = rotate.x * cosf(XMConvertToRadians(m_Angle)) - rotate.y * sinf(XMConvertToRadians(m_Angle));
		rotation.y = rotate.x * sinf(XMConvertToRadians(m_Angle)) + rotate.y * cosf(XMConvertToRadians(m_Angle));
		rotation.Normalize();

		//���炵���x�N�g���Ǝ��_�����x�N�g���̋���
		float max_length = pow((rotation.x - rotate.x)*(rotation.x - rotate.x) + (rotation.y - rotate.y)*(rotation.y - rotate.y),0.5f);

		//�����_���Ńx�N�g���𐶐�
		Vector3 move_vec = { (float)rand() / 16383.5f - 1 ,(float)rand() / 16383.5f - 1,(float)rand() / 16383.5f - 1 };
		move_vec.Normalize();

		//���_�����܂ł̋���
		float lenght = pow((move_vec.x - rotate.x)*(move_vec.x - rotate.x) + (move_vec.y - rotate.y)*(move_vec.y - rotate.y) + (move_vec.z - rotate.z) * (move_vec.z - rotate.z), 0.5f);

		//���˕��@�̕ύX
		switch (m_Type)
		{
		case EmitterType::CONE:
			//�p�x�x�N�g���̋߂��ɔ���
			while ((lenght > max_length + 0.01f) || (lenght < max_length - 0.01f))
			{
				move_vec = Vector3((float)rand() / 16383.5f - 1, (float)rand() / 16383.5f - 1, (float)rand() / 16383.5f - 1);
				move_vec.Normalize();

				lenght = pow((move_vec.x - rotate.x)*(move_vec.x - rotate.x) + (move_vec.y - rotate.y)*(move_vec.y - rotate.y) + (move_vec.z - rotate.z) * (move_vec.z - rotate.z), 0.5f);
			}
			break;
		case EmitterType::SPHERE:
			//�͈͓��Ȃ琶��
			while (lenght > max_length)
			{
				move_vec = Vector3((float)rand() / 16383.5f - 1, (float)rand() / 16383.5f - 1, (float)rand() / 16383.5f - 1);
				move_vec.Normalize();

				lenght = pow((move_vec.x - rotate.x)*(move_vec.x - rotate.x) + (move_vec.y - rotate.y)*(move_vec.y - rotate.y) + (move_vec.z - rotate.z) * (move_vec.z - rotate.z), 0.5f);
			}

			break;
		default:
			break;
		}

		m_MoveVec.push_back(move_vec);
		m_LifeTimer.push_back(m_LifeTime);
		m_GravInf.push_back(0);
}
