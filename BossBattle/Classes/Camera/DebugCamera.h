//～/～/～//～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/
//Content:デバックカメラクラス
//
//day:2017/5/11
//
//actor:Koshida Kenji
//～/～/～//～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/～/

#include <d3d11.h>
#include <SimpleMath.h>
#include <Mouse.h>

class DebugCamera
{
    // カメラの距離
    static const float DEFAULT_CAMERA_DISTANCE;

    // マウス関係
    DirectX::Mouse::State m_mouse_state;
    std::unique_ptr<DirectX::Mouse> m_mouse;
    std::unique_ptr<DirectX::Mouse::ButtonStateTracker> m_mouse_tracker;

    // 横回転
    float m_y_angle, m_y_tmp;

    // 縦回転
    float m_x_angle, m_x_tmp;

    // ドラッグされた座標
    int m_x, m_y;

    float m_sx, m_sy;

    // 生成されたビュー行列
    DirectX::SimpleMath::Matrix m_view;

    // スクロールフォイール値
    int m_scroll_wheel_value;

private:

    void Motion(int x, int y);

public:
    // コンストラクタ
    DebugCamera(int w, int h);

    // 更新
    void Update();

    //カメラ行列の取得
    DirectX::SimpleMath::Matrix GetCameraMatrix(){return m_view;}
};

