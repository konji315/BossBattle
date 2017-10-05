#include "Camera.h"

using namespace DirectX;

using namespace SimpleMath;

/// <summary>
/// コンストラクタ
/// <param name="width">ウィンドウの横幅</param>
/// <param name="height">ウィンドウの縦幅</param>
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
/// デストラクタ
/// </summary>
Camera::~Camera()
{
}

/// <summary>
/// 更新処理
/// </summary>
void Camera::Update()
{
	m_camera_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);

	m_camera_proj = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_nearClip, m_farClip);
}

/// <summary>
/// カメラのビュー行列の取得
/// </summary>
/// <returns>カメラのビュー行列</returns>
const DirectX::SimpleMath::Matrix& Camera::GetViewMatrix()
{
	return m_camera_view;
}

/// <summary>
/// カメラの射影行列の取得
/// </summary>
/// <returns>カメラの射影行列</returns>
const DirectX::SimpleMath::Matrix& Camera::GetProjectionMatrix()
{
	return m_camera_proj;
}

/// <summary>
/// 視点の変更
/// </summary>
/// <param name="eyepos">視点</param>
void Camera::SetEyeSet(const DirectX::SimpleMath::Vector3& eyepos)
{
	m_eyepos = eyepos;
}

/// <summary>
/// 注視点の変更
/// </summary>
/// <param name="refpos">注視点</param>
void Camera::SetRefSet(const DirectX::SimpleMath::Vector3& refpos)
{
	m_refpos = refpos;
}

/// <summary>
/// カメラの上方向の変更
/// </summary>
/// <param name="upvec">方向</param>
void Camera::SetUpvecSet(const DirectX::SimpleMath::Vector3& upvec)
{
	m_upvec = upvec;
}

/// <summary>
/// 縦方向視野角の変更
/// </summary>
/// <param name="fovY">縦方向視野角</param>
void Camera::SetFovY(float fovY)
{
	m_fovY = fovY;
}

/// <summary>
/// アスペクト比の変更
/// </summary>
/// <param name="aspect">アスペクト比</param>
void Camera::SetAspect(float aspect)
{
	m_aspect = aspect;
}

/// <summary>
/// ニア―クリップの変更
/// </summary>
/// <param name="nearClip">ニア―クリップ</param>
void Camera::SetNearClip(float nearClip)
{
	m_nearClip = nearClip;
}

/// <summary>
/// ファークリップの変更
/// </summary>
/// <param name="farClip">ファークリップ</param>
void Camera::SetfarClip(float farClip)
{
	m_farClip = farClip;
}


///// <summary>
///// ３Ｄ→２Ｄ座標変換
///// ワールド座標からスクリーン座標を計算する
///// </summary>
///// <param name="pos2d"></param>
///// <returns>成否</returns>
//bool Camera::Project(const Vector3& worldPos, Vector2* screenPos)
//{
//	Vector4 clipPos;
//	Vector4 worldPosV4(worldPos.x, worldPos.y, worldPos.z, 1.0f);
//
//	// ビュー変換
//	clipPos = Vector4::Transform(worldPosV4, m_camera_view);
//
//	// 射影変換
//	clipPos = Vector4::Transform(clipPos, m_camera_proj);
//
//	// ビューポートの取得
//	D3D11_VIEWPORT viewport = DX::DeviceResources::GetInstance()->GetScreenViewport();
//
//	// ニアクリップより前方にいなければ計算できない
//	if (clipPos.w <= 1.0e-5) return false;
//
//	// ビューポート変換
//	float ndcX = clipPos.x / clipPos.w;
//	float ndcY = -clipPos.y / clipPos.w;
//
//	screenPos->x = (ndcX + 1.0f) * 0.5f * viewport.Width + viewport.TopLeftX;
//	screenPos->y = (ndcY + 1.0f) * 0.5f * viewport.Height + viewport.TopLeftY;
//
//	return true;
//}