#include "pch.h"
#include "Obj3d.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//�f�o�C�X
Microsoft::WRL::ComPtr<ID3D11Device> Obj3d::m_d3dDevice;

//�f�o�C�X�R���e�L�X�g
Microsoft::WRL::ComPtr<ID3D11DeviceContext>     Obj3d::m_d3dContext;

//�J����
Camera* Obj3d::m_camera;

//�ėp�`��X�e�[�g
std::unique_ptr<DirectX::CommonStates> Obj3d::m_states;

////�G�t�F�N�g�t�@�N�g��
std::unique_ptr<DirectX::EffectFactory> Obj3d::m_factory;


/// <summary>
/// ������
/// </summary>
/// <param name="d3dDevice">�f�o�C�X</param>
/// <param name="d3dContext">�f�o�C�X�R���e�L�X�g</param>
/// <param name="m_camera">�J����</param>
void Obj3d::InitializeStatic(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, Camera * camera)
{
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;
	m_camera = camera;

	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());

	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	//�e�N�X�`���̓ǂݍ��݃p�X
	m_factory->SetDirectory(L"Resources");

}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Obj3d::Obj3d()
{
	m_obj_scale = Vector3(1, 1, 1);
	m_parent = nullptr;
	m_UseQuternion = false;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
//Obj3d::~Obj3d()
//{
//}

/// <summary>
/// ���f���̃��[�h
/// </summary>
/// <param name="filename">�t�@�C���p�X</param>
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
		// ���f�����̑S���b�V������
		ModelMesh::Collection::const_iterator it_mesh = m_model->meshes.begin();
		for (; it_mesh != m_model->meshes.end(); it_mesh++)
		{
			ModelMesh* modelmesh = it_mesh->get();
			assert(modelmesh);

			// ���b�V�����̑S���b�V���p�[�c����
			std::vector<std::unique_ptr<ModelMeshPart>>::iterator it_meshpart = modelmesh->meshParts.begin();
			for (; it_meshpart != modelmesh->meshParts.end(); it_meshpart++)
			{
				ModelMeshPart* meshpart = it_meshpart->get();
				assert(meshpart);

				// ���b�V���p�[�c�ɃZ�b�g���ꂽ�G�t�F�N�g��BasicEffect�Ƃ��Ď擾
				std::shared_ptr<IEffect> ieff = meshpart->effect;
				BasicEffect* eff = dynamic_cast<BasicEffect*>(ieff.get());
				if (eff != nullptr)
				{
					// ���Ȕ������ő�l��
					eff->SetEmissiveColor(Vector3(1, 1, 1));

					// �G�t�F�N�g�Ɋ܂ޑS�Ă̕��s�������ɂ��ď�������
					for (int i = 0; i < BasicEffect::MaxDirectionalLights; i++)
					{
						// ���C�g�𖳌��ɂ���
						eff->SetLightEnabled(i, false);
					}
				}
			}
		}
	}
}

/// <summary>
/// �X�V
/// </summary>
void Obj3d::Update()
{
	Matrix scalemat = Matrix::CreateScale(m_obj_scale);
	Matrix rotmat;

	if (m_UseQuternion)
	{//�N�H�[�^�j�I���ɉ�]���v�Z

		rotmat = Matrix::CreateFromQuaternion(m_obj_rotQ);
	}
	else
	{//�I�C���[�p�ŉ�]���v�Z
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
/// �`��
/// </summary>
void Obj3d::Render()
{
	assert(m_camera);

	if(m_model)
		m_model->Draw(m_d3dContext.Get(), *m_states.get(), m_obj_world, m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());
}