//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:AnimationModel
//
//day:2017/10/10
//
//acotr:Koshida Kenji
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
#pragma once

#include <Model.h>
#include <VertexTypes.h>
#include <map>
#include <Effects.h>

#include "..\Camera\Camera.h"

namespace VSD3DStarter
{
#pragma pack(push,1)

    //ƒ}ƒeƒŠƒAƒ‹î•ñ
    struct Material
    {
        DirectX::XMFLOAT4   ambient;
        DirectX::XMFLOAT4   diffuse;
        DirectX::XMFLOAT4   specular;
        float               specularPower;
        DirectX::XMFLOAT4   emissive;
        DirectX::XMFLOAT4X4 UVtransform;
    };

    const uint32_t MAX_TEXTURE = 8;

    //ƒƒbƒVƒ…î•ñ
    struct SubMesh
    {
        UINT materialIndex;
        UINT indexBufferIndex;
        UINT vertexBufferIndex;
        UINT startIndex;
        UINT primCount;
    };

    const uint32_t NUM_BONE_INFLUENCES = 4;

    //‚»‚Ì‘¼‚ÌƒƒbƒVƒ…î•ñ
    struct MeshExtents
    {
        float centerX, centerY, centerZ;
        float Radius;

        float minX, minY, minZ;
        float maxX, maxY, maxZ;
    };

    //ƒ{[ƒ“î•ñ
    struct Bone
    {
        INT parentIndex;
        DirectX::XMFLOAT4X4 invBindPos;
        DirectX::XMFLOAT4X4 bindPos;
        DirectX::XMFLOAT4X4 localTransform;

        Bone::Bone(const Bone &o)
        {
            parentIndex = o.parentIndex;
            invBindPos = o.invBindPos;
            bindPos = o.bindPos;
            localTransform = o.localTransform;
        }
    };

    struct Clip
    {
        float startTime;
        float endTime;
        UINT  keys;

        Clip::Clip(const Clip &o)
        {
            startTime = o.startTime;
            endTime = o.endTime;
            keys = o.keys;
        }
    };

    struct Keyframe
    {
        UINT boneIndex;
        float time;
        DirectX::XMFLOAT4X4 transform;

        Keyframe::Keyframe(const Keyframe &o)
        {
            boneIndex = o.boneIndex;
            time = o.time;
            transform = o.transform;
        }
    };


    /// <summary>
    /// ƒAƒjƒ[ƒVƒ‡ƒ“‘JˆÚ
    /// </summary>
    struct AnimationState
    {
        AnimationState()
        {
            animTime = 0;
            end = false;
        }

        std::vector<DirectX::XMFLOAT4X4> m_bone_world_transforms;
        float animTime;
        bool end;
    };

    #define NUM_BONE_INFLUENCES 4
    struct SkinningVertex
    {
        UINT boneIndex[NUM_BONE_INFLUENCES];
        float boneWeight[NUM_BONE_INFLUENCES];
    };

#pragma pack(pop)

}; // namespace


class AnimationModel : public DirectX::Model
{
public:
    static ID3D11Device*                              m_device;
    static ID3D11DeviceContext*                       m_context;
public:
    virtual ~AnimationModel();

    std::vector<VSD3DStarter::Bone>                                          m_bones;					//ƒ{[ƒ“
    std::map<const std::wstring, VSD3DStarter::Clip>                         m_clips;					//ƒAƒjƒ[ƒVƒ‡ƒ“ƒNƒŠƒbƒv
    std::map < const std::wstring, std::vector<VSD3DStarter::Keyframe>>      m_keys;					//ƒL[ƒtƒŒ[ƒ€
    std::vector<bool>                                                        m_is_transform_combined;	//eqŠÖŒW‚Ì\’z‚Ì—L–³

    VSD3DStarter::AnimationState                                             m_anim_state;				//ƒAƒjƒ[ƒVƒ‡ƒ“‘JˆÚ

    //eqŠÖŒW‚Ì\’z
    void CombineTransforms(UINT current_boneindex, std::vector<VSD3DStarter::Bone> const& skinning_info, std::vector<DirectX::XMFLOAT4X4>& bone_world_transform);

    //CMO“Ç‚İ‚İ
    static std::unique_ptr<AnimationModel> AnimCreateFromCMO(_In_ ID3D11Device* d3dDevice, _In_reads_bytes_(dataSize) const uint8_t* meshData, size_t dataSize,
        _In_ DirectX::IEffectFactory& fxFactory, bool ccw = true, bool pmalpha = false);
    static std::unique_ptr<AnimationModel> AnimCreateFromCMO(_In_ ID3D11Device* d3dDevice, _In_z_ const wchar_t* szFileName,
        _In_ DirectX::IEffectFactory& fxFactory, bool ccw = true, bool pmalpha = false);

    //ƒAƒjƒ[ƒVƒ‡ƒ“‚ÌXV
    void UpdateAnimation(std::wstring clip_name = L"Take 001");
};

