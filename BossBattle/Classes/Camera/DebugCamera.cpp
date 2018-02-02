// デバッグ用カメラクラス
#include "DebugCamera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//カメラの距離
const float DebugCamera::DEFAULT_CAMERA_DISTANCE = 5.0f;

// コンストラクタ
DebugCamera::DebugCamera(int w, int h)
	: m_y_angle(0.0f), m_y_tmp(0.0f), m_x_angle(0.0f), m_x_tmp(0.0f), m_x(0), m_y(0), m_scroll_wheel_value(0)
{
	// 画面サイズに対する相対的なスケールに調整
	m_sx = 1.0f / (float)w;
	m_sy = 1.0f / (float)h;
	m_view = DirectX::SimpleMath::Matrix::Identity;

	// マウスの初期化
	m_mouse.reset(new Mouse);
	m_mouse_tracker.reset(new Mouse::ButtonStateTracker);
}

void DebugCamera::Update()
{
	// マウス情報を取得
	m_mouse_state = m_mouse->GetState();
	m_mouse_tracker->Update(m_mouse_state);

	// マウスの左ボタンが押された
	if (m_mouse_tracker->leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
	{
		// マウスの座標を取得
		m_x = m_mouse_state.x;
		m_y = m_mouse_state.y;
	}
	else if (m_mouse_tracker->leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		// 現在の回転を保存
		m_x_angle = m_x_tmp;
		m_y_angle = m_y_tmp;
	}
	// マウスのボタンが押されていたらカメラを移動させる
	if (m_mouse_state.leftButton)
	{
		Motion(m_mouse_state.x, m_mouse_state.y);
	}

	// マウスのフォイール値を取得
	m_scroll_wheel_value = m_mouse_state.scrollWheelValue;
	if (m_scroll_wheel_value > 0)
	{
		m_scroll_wheel_value = 0;
		m_mouse->ResetScrollWheelValue();
	}

	// ビュー行列を算出する
	Matrix rotY = Matrix::CreateRotationY(m_y_tmp);
	Matrix rotX = Matrix::CreateRotationX(m_x_tmp);

	Matrix rt = rotY * rotX;

	Vector3 eye(0.0f, 0.0f, 1.0f);
	Vector3 target(0.0f, 0.0f, 0.0f);
	Vector3 up(0.0f, 1.0f, 0.0f);

	eye = Vector3::Transform(eye, rt.Invert());
	eye *= (DEFAULT_CAMERA_DISTANCE - m_scroll_wheel_value / 100);
	up = Vector3::Transform(up, rt.Invert());

	m_view = Matrix::CreateLookAt(eye, target, up);
}

void DebugCamera::Motion(int x, int y)
{
	// マウスポインタの位置のドラッグ開始位置からの変位 (相対値)
	float dx = (x - m_x) * m_sx;
	float dy = (y - m_y) * m_sy;

	if (dx != 0.0f || dy != 0.0f)
	{
		// Ｙ軸の回転
		float y_angle = dx * XM_PI;
		// Ｘ軸の回転
		float x_angle = dy * XM_PI;

		m_x_tmp = m_x_angle + x_angle;
		m_y_tmp = m_y_angle + y_angle;
	}
}