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

	//パーティクルエミッターの削除
	for (auto pe : m_ParticleEmitters)
	{
		if (pe != nullptr)
		{
			delete pe;
		}
	}

	//登録したエミッターの削除
	for (auto pe : m_RegisteredEmitters)
	{
		if (pe.second != nullptr)
		{
			delete pe.second;
		}
	}
}

/// <summary>
/// 初期化
/// </summary>
void ParticleEffectManager::Initialize()
{
	DXTK::DXTKResources& dxtk = singleton<DXTK::DXTKResources>::GetInstance();
	ID3D11Device* device = dxtk.m_device;
	ID3D11DeviceContext* context = dxtk.m_context;

	m_Camera = nullptr;

	// コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleVS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticlePS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleGS.cso");

	// 頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(
		VSData.GetData(),
		VSData.GetSize(),
		NULL,
		m_VertexShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	// ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(
		PSData.GetData(),
		PSData.GetSize(),
		NULL,
		m_PixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	// ジオメトリシェーダ作成
	if (FAILED(device->CreateGeometryShader(
		GSData.GetData(),
		GSData.GetSize(),
		NULL,
		m_GeometryShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}

	// プリミティブバッチ作成
	//  各頂点に含まれる情報が、位置・色・テクスチャUV
	m_PrimitiveBatch =
		std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(
			context, 1, PARTICLE_NUM_MAX);

	// 頂点フォーマットを指定して入力レイアウト作成
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

	//サンプラの作成
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
/// 更新
/// </summary>
void ParticleEffectManager::Update()
{
	//寿命の減少
	for (UINT i = 0; i < m_LifeTimer.size(); i++)
	{
		//-1ならば無視
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

	//個々のパーティクルの更新
	for (auto pe : m_ParticleEmitters)
	{
		pe->Update();
	}
}

/// <summary>
/// 描画
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


	//平行移動
	{
		//m_pos.x += 0.1f;
		//world = Matrix::CreateTranslation(m_pos);
	}

	//回転
	{
		//m_angle.z += XM_1DIV2PI / 2;
		//world = Matrix::CreateRotationZ(m_angle.z);
	}

	//回転移動
	{
		//m_pos.x += 0.1f;
		//m_angle.z += XM_1DIV2PI / 2;
		//world = Matrix::CreateRotationZ(m_angle.z) * Matrix::CreateTranslation(m_pos);
	}

	//拡縮
	{
		//m_angle.z += XM_1DIV2PI / 2;
		//world = Matrix::CreateRotationZ(m_angle.z) * Matrix::CreateScale(sinf(m_angle.z) + 1.0f);
	}

	//円運動
	{
		//m_angle.z -= XM_1DIV2PI / 2;

		//m_pos = Vector3(sinf(m_angle.z), cosf(m_angle.z), 0);

		//world = Matrix::CreateTranslation(m_pos);
	}

	//8の字
	{
		//m_angle.z -= XM_1DIV2PI / 2;

		//m_pos = Vector3(cosf(m_angle.z), sinf(m_angle.z + m_angle.z), 0);

		//world = Matrix::CreateTranslation(m_pos);
	}

#pragma endregion

	Matrix wvp = world * view * proj;

	//シェーダーのコンスタントバッファに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(context->Map(m_ConstantsBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		Constants constants;

		constants.WVP = wvp;
		constants.BILLBOARD = m_Camera->GetBillboard();

		memcpy_s(pData.pData, pData.RowPitch, &constants, sizeof(constants));
		context->Unmap(m_ConstantsBuffer.Get(), 0);
	}

	//このコンスタントバッファをどのシェーダに使うか
	context->VSGetConstantBuffers(0, 0, nullptr);
	context->GSSetConstantBuffers(0, 1, m_ConstantsBuffer.GetAddressOf());
	context->PSGetConstantBuffers(0, 0, nullptr);

	//デバイスコンテキストに、描画に使用するシェーダを指定
	context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_GeometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_PixelShader.Get(), nullptr, 0);

	//描画に使用する入力レイアウトを指定
	context->IASetInputLayout(m_InputLayout.Get());

	//背面カリング設定
	context->RSSetState(m_CommonStates->CullNone());

	////アルファブレンド設定
	//context->OMSetBlendState(m_CommonStates->NonPremultiplied(), nullptr, 0xffffff);

	//加算合成描画
	context->OMSetBlendState(m_CommonStates->Additive(), nullptr, 0xffffff);

	////減算合成描画
	//context->OMSetBlendState(m_blend_state.Get(), nullptr, 0xffffff);

	//深度ステンシル設定
	context->OMSetDepthStencilState(m_CommonStates->DepthRead(), 0);

	//サンプラの設定
	context->PSSetSamplers(0, 1, m_Sampler.GetAddressOf());

	for (auto particle : m_ParticleEmitters)
	{
		particle->Render(m_PrimitiveBatch.get());
	}

	//シェーダのクリア
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

/// <summary>
/// パーティクルエミッター生成
/// </summary>
/// <param name="name">登録したエミッター名</param>
/// <param name="pos">座標</param>
ParticleEmitter* ParticleEffectManager::Play(std::string name, DirectX::SimpleMath::Vector3 pos, float lifetime, std::string childname)
{
	ParticleEmitter* pe = new ParticleEmitter();

	auto emitter = m_RegisteredEmitters.find(name);

	//登録されているならコピー
	if (emitter != m_RegisteredEmitters.end())
		*pe = *emitter->second;
	else
	{
		delete pe;
		pe = nullptr;
		MessageBox(0, L"Particle Notfound.", NULL, MB_OK);
		return nullptr;
	}

	//場所の設定
	pe->SetPos(pos);

	//子供を設定するならば
	if (childname != "")
		pe->SetChild(childname);

	m_ParticleEmitters.push_back(pe);
	m_LifeTimer.push_back(lifetime);

	return pe;
}

/// <summary>
/// パーティクル登録
/// </summary>
/// <param name="pe"></param>
void ParticleEffectManager::RegisterParticle(std::string name, ParticleEmitter * pe)
{
	auto emitter = m_RegisteredEmitters.find(name);

	//登録されていなかったら登録
	if (emitter == m_RegisteredEmitters.end())
		m_RegisteredEmitters[name] = pe;
	else
	{
		//登録済みだったのでパーティクルを削除し、スキップ
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

	//リソースからテクスチャ検索
	auto found = pem.m_TextureResources.find(filepath);
	//あったら使いまわして、無かったら読み込み
	if (found != pem.m_TextureResources.end())
	{
		m_Texture = found->second;
	}
	else
	{
		//テクスチャの読み込み
		HRESULT hr = CreateWICTextureFromFile(device, filepath, nullptr, m_Texture.GetAddressOf());

		pem.m_TextureResources[filepath] = m_Texture;
	}
}

void ParticleEmitter::Update()
{

	/// <summary>
	/// ループ機能と持続時間の設定
	/// </summary>
	{
		//発生条件
		if ((m_PlayBackTimer <= m_Duration * 60 || m_Loop))
		{
			if (m_Burst)
			{
				//持続時間ごとに一斉に発射
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
				//レートごとにパーティクル生成
				if (m_PlayBackTimer % (60 / m_Rate) == 0 && m_PlayBackTimer != 0)
				{
					CreateParticle();
				}

			}
		}
	}

	//パーティクル群を管理するタイマー
	m_PlayBackTimer++;


	//重力加算
	for (UINT i = 0; i < m_GravInf.size(); i++)
	{
		m_GravInf[i] += m_Gravity / 100;
	}

	//パーティクル個々の操作
	if (m_Vertices.size() > 0)
	{
		for (UINT i = 0; i < m_Vertices.size();i++)
		{
			/// <summary>
			/// 計算式大幅修正点
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


	//寿命の減少
	for (UINT i = 0; i < m_LifeTimer.size(); i++)
	{
		m_LifeTimer[i] -= 1 / 60.0f;

		//寿命が尽きたらパーティクルの各要素を削除						←要クラス化
		if (m_LifeTimer[i] <= 0)
		{
			//子供が設定されていた場合
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

	//テクスチャの設定
	context->PSSetShaderResources(0, 1, m_Texture.GetAddressOf());

	// 頂点データをすべて渡して描画する
	pb->Begin();
	pb->Draw(
		D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
		&m_Vertices[0],
		m_Vertices.size());
	pb->End();

}

/// <summary>
/// 個々のパーティクルの生成
/// </summary>
void ParticleEmitter::CreateParticle()
{
		VertexPositionColorTexture vertexData;
		vertexData.position = m_Pos;
		vertexData.color = m_StartColor;
		vertexData.textureCoordinate = Vector2(m_StartScale, 1);

		m_Vertices.push_back(vertexData);

		//視点方向
		Vector3 rotate = m_Rotate;

		rotate.Normalize();

		Vector3 rotation;

		//視点方向を角度分回転
		rotation.x = rotate.x * cosf(XMConvertToRadians(m_Angle)) - rotate.y * sinf(XMConvertToRadians(m_Angle));
		rotation.y = rotate.x * sinf(XMConvertToRadians(m_Angle)) + rotate.y * cosf(XMConvertToRadians(m_Angle));
		rotation.Normalize();

		//ずらしたベクトルと視点方向ベクトルの距離
		float max_length = pow((rotation.x - rotate.x)*(rotation.x - rotate.x) + (rotation.y - rotate.y)*(rotation.y - rotate.y),0.5f);

		//ランダムでベクトルを生成
		Vector3 move_vec = { (float)rand() / 16383.5f - 1 ,(float)rand() / 16383.5f - 1,(float)rand() / 16383.5f - 1 };
		move_vec.Normalize();

		//視点方向までの距離
		float lenght = pow((move_vec.x - rotate.x)*(move_vec.x - rotate.x) + (move_vec.y - rotate.y)*(move_vec.y - rotate.y) + (move_vec.z - rotate.z) * (move_vec.z - rotate.z), 0.5f);

		//発射方法の変更
		switch (m_Type)
		{
		case EmitterType::CONE:
			//角度ベクトルの近くに発射
			while ((lenght > max_length + 0.01f) || (lenght < max_length - 0.01f))
			{
				move_vec = Vector3((float)rand() / 16383.5f - 1, (float)rand() / 16383.5f - 1, (float)rand() / 16383.5f - 1);
				move_vec.Normalize();

				lenght = pow((move_vec.x - rotate.x)*(move_vec.x - rotate.x) + (move_vec.y - rotate.y)*(move_vec.y - rotate.y) + (move_vec.z - rotate.z) * (move_vec.z - rotate.z), 0.5f);
			}
			break;
		case EmitterType::SPHERE:
			//範囲内なら生成
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
