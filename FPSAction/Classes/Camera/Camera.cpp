#include "Camera.h"

using namespace DirectX;

using namespace SimpleMath;

/// <summary>
/// �R���X�g���N�^
/// <param name="width">�E�B���h�E�̉���</param>
/// <param name="height">�E�B���h�E�̏c��</param>
/// </summary>
Camera::Camera(int width,int height)
{
	m_upvec = Vector3(0, 1.0f, 0);

	m_fovY = XMConvertToRadians(60.0f);
	m_aspect = (float)width / height;
	m_nearClip = 0.1f;
	m_farClip = 1000.0f;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Camera::~Camera()
{
}

/// <summary>
/// �X�V����
/// </summary>
void Camera::Update()
{
	m_camera_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);

	m_camera_proj = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_nearClip, m_farClip);
}

/// <summary>
/// �J�����̃r���[�s��̎擾
/// </summary>
/// <returns>�J�����̃r���[�s��</returns>
const DirectX::SimpleMath::Matrix& Camera::GetViewMatrix()
{
	return m_camera_view;
}

/// <summary>
/// �J�����̎ˉe�s��̎擾
/// </summary>
/// <returns>�J�����̎ˉe�s��</returns>
const DirectX::SimpleMath::Matrix& Camera::GetProjectionMatrix()
{
	return m_camera_proj;
}

/// <summary>
/// ���_�̕ύX
/// </summary>
/// <param name="eyepos">���_</param>
void Camera::SetEyeSet(const DirectX::SimpleMath::Vector3& eyepos)
{
	m_eyepos = eyepos;
}

/// <summary>
/// �����_�̕ύX
/// </summary>
/// <param name="refpos">�����_</param>
void Camera::SetRefSet(const DirectX::SimpleMath::Vector3& refpos)
{
	m_refpos = refpos;
}

/// <summary>
/// �J�����̏�����̕ύX
/// </summary>
/// <param name="upvec">����</param>
void Camera::SetUpvecSet(const DirectX::SimpleMath::Vector3& upvec)
{
	m_upvec = upvec;
}

/// <summary>
/// �c��������p�̕ύX
/// </summary>
/// <param name="fovY">�c��������p</param>
void Camera::SetFovY(float fovY)
{
	m_fovY = fovY;
}

/// <summary>
/// �A�X�y�N�g��̕ύX
/// </summary>
/// <param name="aspect">�A�X�y�N�g��</param>
void Camera::SetAspect(float aspect)
{
	m_aspect = aspect;
}

/// <summary>
/// �j�A�\�N���b�v�̕ύX
/// </summary>
/// <param name="nearClip">�j�A�\�N���b�v</param>
void Camera::SetNearClip(float nearClip)
{
	m_nearClip = nearClip;
}

/// <summary>
/// �t�@�[�N���b�v�̕ύX
/// </summary>
/// <param name="farClip">�t�@�[�N���b�v</param>
void Camera::SetfarClip(float farClip)
{
	m_farClip = farClip;
}


///// <summary>
///// �R�c���Q�c���W�ϊ�
///// ���[���h���W����X�N���[�����W���v�Z����
///// </summary>
///// <param name="pos2d"></param>
///// <returns>����</returns>
//bool Camera::Project(const Vector3& worldPos, Vector2* screenPos)
//{
//	Vector4 clipPos;
//	Vector4 worldPosV4(worldPos.x, worldPos.y, worldPos.z, 1.0f);
//
//	// �r���[�ϊ�
//	clipPos = Vector4::Transform(worldPosV4, m_camera_view);
//
//	// �ˉe�ϊ�
//	clipPos = Vector4::Transform(clipPos, m_camera_proj);
//
//	// �r���[�|�[�g�̎擾
//	D3D11_VIEWPORT viewport = DX::DeviceResources::GetInstance()->GetScreenViewport();
//
//	// �j�A�N���b�v���O���ɂ��Ȃ���Όv�Z�ł��Ȃ�
//	if (clipPos.w <= 1.0e-5) return false;
//
//	// �r���[�|�[�g�ϊ�
//	float ndcX = clipPos.x / clipPos.w;
//	float ndcY = -clipPos.y / clipPos.w;
//
//	screenPos->x = (ndcX + 1.0f) * 0.5f * viewport.Width + viewport.TopLeftX;
//	screenPos->y = (ndcY + 1.0f) * 0.5f * viewport.Height + viewport.TopLeftY;
//
//	return true;
//}