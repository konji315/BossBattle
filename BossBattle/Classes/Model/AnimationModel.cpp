#include "..\Model\AnimationModel.h"
#include "..\Common\DXTKResouces.h"

#include <DirectXMath.h>
#include <DirectXHelpers.h>
#include <wchar.h>
#include <DDSTextureLoader.h>

#include <PlatformHelpers.h>
#include <BinaryReader.h>

using namespace DirectX;
using Microsoft::WRL::ComPtr;

static_assert(sizeof(VSD3DStarter::Material) == 132, "CMO Mesh structure size incorrect");
static_assert(sizeof(VSD3DStarter::SubMesh) == 20, "CMO Mesh structure size incorrect");
static_assert(sizeof(VSD3DStarter::SkinningVertex) == 32, "CMO Mesh structure size incorrect");
static_assert(sizeof(VSD3DStarter::MeshExtents) == 40, "CMO Mesh structure size incorrect");
static_assert(sizeof(VSD3DStarter::Bone) == 196, "CMO Mesh structure size incorrect");
static_assert(sizeof(VSD3DStarter::Clip) == 12, "CMO Mesh structure size incorrect");
static_assert(sizeof(VSD3DStarter::Keyframe) == 72, "CMO Mesh structure size incorrect");

//--------------------------------------------------------------------------------------
struct MaterialRecordCMO
{
    const VSD3DStarter::Material*   pMaterial;
    std::wstring                    name;
    std::wstring                    pixelShader;
    std::wstring                    texture[VSD3DStarter::MAX_TEXTURE];
    std::shared_ptr<IEffect>        effect;
    ComPtr<ID3D11InputLayout>       il;
};

static INIT_ONCE g_InitOnce = INIT_ONCE_STATIC_INIT;
static std::shared_ptr<std::vector<D3D11_INPUT_ELEMENT_DESC>> g_vbdecl;
static std::shared_ptr<std::vector<D3D11_INPUT_ELEMENT_DESC>> g_vbdeclSkinning;

// Helper for creating a D3D input layout.
static void CreateInputLayout(_In_ ID3D11Device* device, IEffect* effect, _Out_ ID3D11InputLayout** pInputLayout, bool skinning)
{
    void const* shaderByteCode;
    size_t byteCodeLength;

    effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

    if (skinning)
    {
        ThrowIfFailed(
            device->CreateInputLayout(VertexPositionNormalTangentColorTextureSkinning::InputElements,
                VertexPositionNormalTangentColorTextureSkinning::InputElementCount,
                shaderByteCode, byteCodeLength,
                pInputLayout)
        );
    }
    else
    {
        ThrowIfFailed(
            device->CreateInputLayout(VertexPositionNormalTangentColorTexture::InputElements,
                VertexPositionNormalTangentColorTexture::InputElementCount,
                shaderByteCode, byteCodeLength,
                pInputLayout)
        );
    }

    _Analysis_assume_(*pInputLayout != 0);

    SetDebugObjectName(*pInputLayout, "ModelCMO");
}

static BOOL CALLBACK InitializeDecl(PINIT_ONCE initOnce, PVOID Parameter, PVOID *lpContext)
{
    UNREFERENCED_PARAMETER(initOnce);
    UNREFERENCED_PARAMETER(Parameter);
    UNREFERENCED_PARAMETER(lpContext);

    g_vbdecl = std::make_shared<std::vector<D3D11_INPUT_ELEMENT_DESC>>(VertexPositionNormalTangentColorTexture::InputElements,
        VertexPositionNormalTangentColorTexture::InputElements + VertexPositionNormalTangentColorTexture::InputElementCount);

    g_vbdeclSkinning = std::make_shared<std::vector<D3D11_INPUT_ELEMENT_DESC>>(VertexPositionNormalTangentColorTextureSkinning::InputElements,
        VertexPositionNormalTangentColorTextureSkinning::InputElements + VertexPositionNormalTangentColorTextureSkinning::InputElementCount);
    return TRUE;
}

//アニメーション有りモデルの生成
std::unique_ptr<AnimationModel> AnimationModel::AnimCreateFromCMO(ID3D11Device * d3dDevice, const uint8_t * meshData, size_t dataSize, DirectX::IEffectFactory & fxFactory, bool ccw, bool pmalpha)
{
    if (!InitOnceExecuteOnce(&g_InitOnce, InitializeDecl, nullptr, nullptr))
        throw std::exception("One-time initialization failed");

    if (!d3dDevice || !meshData)
        throw std::exception("Device and meshData cannot be null");

    auto fxFactoryDGSL = dynamic_cast<DGSLEffectFactory*>(&fxFactory);

    // Meshes
    auto nMesh = reinterpret_cast<const UINT*>(meshData);
    size_t usedSize = sizeof(UINT);
    if (dataSize < usedSize)
        throw std::exception("End of file");

    if (!*nMesh)
        throw std::exception("No meshes found");

    std::unique_ptr<AnimationModel> model(new AnimationModel());

    for (UINT meshIndex = 0; meshIndex < *nMesh; ++meshIndex)
    {
        // Mesh name
        auto nName = reinterpret_cast<const UINT*>(meshData + usedSize);
        usedSize += sizeof(UINT);
        if (dataSize < usedSize)
            throw std::exception("End of file");

        auto meshName = reinterpret_cast<const wchar_t*>(meshData + usedSize);

        usedSize += sizeof(wchar_t)*(*nName);
        if (dataSize < usedSize)
            throw std::exception("End of file");

        auto mesh = std::make_shared<ModelMesh>();
        mesh->name.assign(meshName, *nName);
        mesh->ccw = ccw;
        mesh->pmalpha = pmalpha;

        // Materials
        auto nMats = reinterpret_cast<const UINT*>(meshData + usedSize);
        usedSize += sizeof(UINT);
        if (dataSize < usedSize)
            throw std::exception("End of file");

        std::vector<MaterialRecordCMO> materials;
        materials.reserve(*nMats);
        for (UINT j = 0; j < *nMats; ++j)
        {
            MaterialRecordCMO m;

            // Material name
            nName = reinterpret_cast<const UINT*>(meshData + usedSize);
            usedSize += sizeof(UINT);
            if (dataSize < usedSize)
                throw std::exception("End of file");

            auto matName = reinterpret_cast<const wchar_t*>(meshData + usedSize);

            usedSize += sizeof(wchar_t)*(*nName);
            if (dataSize < usedSize)
                throw std::exception("End of file");

            m.name.assign(matName, *nName);

            // Material settings
            auto matSetting = reinterpret_cast<const VSD3DStarter::Material*>(meshData + usedSize);
            usedSize += sizeof(VSD3DStarter::Material);
            if (dataSize < usedSize)
                throw std::exception("End of file");

            m.pMaterial = matSetting;

            // Pixel shader name
            nName = reinterpret_cast<const UINT*>(meshData + usedSize);
            usedSize += sizeof(UINT);
            if (dataSize < usedSize)
                throw std::exception("End of file");

            auto psName = reinterpret_cast<const wchar_t*>(meshData + usedSize);

            usedSize += sizeof(wchar_t)*(*nName);
            if (dataSize < usedSize)
                throw std::exception("End of file");

            m.pixelShader.assign(psName, *nName);

            for (UINT t = 0; t < VSD3DStarter::MAX_TEXTURE; ++t)
            {
                nName = reinterpret_cast<const UINT*>(meshData + usedSize);
                usedSize += sizeof(UINT);
                if (dataSize < usedSize)
                    throw std::exception("End of file");

                auto txtName = reinterpret_cast<const wchar_t*>(meshData + usedSize);

                usedSize += sizeof(wchar_t)*(*nName);
                if (dataSize < usedSize)
                    throw std::exception("End of file");

                m.texture[t].assign(txtName, *nName);
            }

            materials.emplace_back(m);
        }

        assert(materials.size() == *nMats);

        // Skeletal data?
        auto bSkeleton = reinterpret_cast<const BYTE*>(meshData + usedSize);
        usedSize += sizeof(BYTE);
        if (dataSize < usedSize)
            throw std::exception("End of file");

        // Submeshes
        auto nSubmesh = reinterpret_cast<const UINT*>(meshData + usedSize);
        usedSize += sizeof(UINT);
        if (dataSize < usedSize)
            throw std::exception("End of file");

        if (!*nSubmesh)
            throw std::exception("No submeshes found\n");

        auto subMesh = reinterpret_cast<const VSD3DStarter::SubMesh*>(meshData + usedSize);
        usedSize += sizeof(VSD3DStarter::SubMesh) * (*nSubmesh);
        if (dataSize < usedSize)
            throw std::exception("End of file");

        // Index buffers
        auto nIBs = reinterpret_cast<const UINT*>(meshData + usedSize);
        usedSize += sizeof(UINT);
        if (dataSize < usedSize)
            throw std::exception("End of file");

        if (!*nIBs)
            throw std::exception("No index buffers found\n");

        struct IBData
        {
            size_t          nIndices;
            const USHORT*   ptr;
        };

        std::vector<IBData> ibData;
        ibData.reserve(*nIBs);

        std::vector<ComPtr<ID3D11Buffer>> ibs;
        ibs.resize(*nIBs);

        for (UINT j = 0; j < *nIBs; ++j)
        {
            auto nIndexes = reinterpret_cast<const UINT*>(meshData + usedSize);
            usedSize += sizeof(UINT);
            if (dataSize < usedSize)
                throw std::exception("End of file");

            if (!*nIndexes)
                throw std::exception("Empty index buffer found\n");

            size_t ibBytes = sizeof(USHORT) * (*(nIndexes));

            auto indexes = reinterpret_cast<const USHORT*>(meshData + usedSize);
            usedSize += ibBytes;
            if (dataSize < usedSize)
                throw std::exception("End of file");

            IBData ib;
            ib.nIndices = *nIndexes;
            ib.ptr = indexes;
            ibData.emplace_back(ib);

            D3D11_BUFFER_DESC desc = {};
            desc.Usage = D3D11_USAGE_DEFAULT;
            desc.ByteWidth = static_cast<UINT>(ibBytes);
            desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

            D3D11_SUBRESOURCE_DATA initData = {};
            initData.pSysMem = indexes;

            ThrowIfFailed(
                d3dDevice->CreateBuffer(&desc, &initData, &ibs[j])
            );

            SetDebugObjectName(ibs[j].Get(), "ModelCMO");
        }

        assert(ibData.size() == *nIBs);
        assert(ibs.size() == *nIBs);

        // Vertex buffers
        auto nVBs = reinterpret_cast<const UINT*>(meshData + usedSize);
        usedSize += sizeof(UINT);
        if (dataSize < usedSize)
            throw std::exception("End of file");

        if (!*nVBs)
            throw std::exception("No vertex buffers found\n");

        struct VBData
        {
            size_t                                          nVerts;
            const VertexPositionNormalTangentColorTexture*  ptr;
            const VSD3DStarter::SkinningVertex*             skinPtr;
        };

        std::vector<VBData> vbData;
        vbData.reserve(*nVBs);
        for (UINT j = 0; j < *nVBs; ++j)
        {
            auto nVerts = reinterpret_cast<const UINT*>(meshData + usedSize);
            usedSize += sizeof(UINT);
            if (dataSize < usedSize)
                throw std::exception("End of file");

            if (!*nVerts)
                throw std::exception("Empty vertex buffer found\n");

            size_t vbBytes = sizeof(VertexPositionNormalTangentColorTexture) * (*(nVerts));

            auto verts = reinterpret_cast<const VertexPositionNormalTangentColorTexture*>(meshData + usedSize);
            usedSize += vbBytes;
            if (dataSize < usedSize)
                throw std::exception("End of file");

            VBData vb;
            vb.nVerts = *nVerts;
            vb.ptr = verts;
            vb.skinPtr = nullptr;
            vbData.emplace_back(vb);
        }

        assert(vbData.size() == *nVBs);

        // Skinning vertex buffers
        auto nSkinVBs = reinterpret_cast<const UINT*>(meshData + usedSize);
        usedSize += sizeof(UINT);
        if (dataSize < usedSize)
            throw std::exception("End of file");

        if (*nSkinVBs)
        {
            if (*nSkinVBs != *nVBs)
                throw std::exception("Number of VBs not equal to number of skin VBs");

            for (UINT j = 0; j < *nSkinVBs; ++j)
            {
                auto nVerts = reinterpret_cast<const UINT*>(meshData + usedSize);
                usedSize += sizeof(UINT);
                if (dataSize < usedSize)
                    throw std::exception("End of file");

                if (!*nVerts)
                    throw std::exception("Empty skinning vertex buffer found\n");

                if (vbData[j].nVerts != *nVerts)
                    throw std::exception("Mismatched number of verts for skin VBs");

                size_t vbBytes = sizeof(VSD3DStarter::SkinningVertex) * (*(nVerts));

                auto verts = reinterpret_cast<const VSD3DStarter::SkinningVertex*>(meshData + usedSize);
                usedSize += vbBytes;
                if (dataSize < usedSize)
                    throw std::exception("End of file");

                vbData[j].skinPtr = verts;
            }
        }

        // Extents
        auto extents = reinterpret_cast<const VSD3DStarter::MeshExtents*>(meshData + usedSize);
        usedSize += sizeof(VSD3DStarter::MeshExtents);
        if (dataSize < usedSize)
            throw std::exception("End of file");

        mesh->boundingSphere.Center.x = extents->centerX;
        mesh->boundingSphere.Center.y = extents->centerY;
        mesh->boundingSphere.Center.z = extents->centerZ;
        mesh->boundingSphere.Radius = extents->Radius;

        XMVECTOR min = XMVectorSet(extents->minX, extents->minY, extents->minZ, 0.f);
        XMVECTOR max = XMVectorSet(extents->maxX, extents->maxY, extents->maxZ, 0.f);
        BoundingBox::CreateFromPoints(mesh->boundingBox, min, max);

        // Animation data
        if (*bSkeleton)
        {
            // Bones
            auto nBones = reinterpret_cast<const UINT*>(meshData + usedSize);
            usedSize += sizeof(UINT);
            if (dataSize < usedSize)
                throw std::exception("End of file");

            if (!*nBones)
                throw std::exception("Animation bone data is missing\n");

            for (UINT j = 0; j < *nBones; ++j)
            {
                // Bone name
                nName = reinterpret_cast<const UINT*>(meshData + usedSize);
                usedSize += sizeof(UINT);
                if (dataSize < usedSize)
                    throw std::exception("End of file");

                auto boneName = reinterpret_cast<const wchar_t*>(meshData + usedSize);

                usedSize += sizeof(wchar_t)*(*nName);
                if (dataSize < usedSize)
                    throw std::exception("End of file");

                // TODO - What to do with bone name?
                boneName;

                // Bone settings
                auto bones = reinterpret_cast<const VSD3DStarter::Bone*>(meshData + usedSize);
                usedSize += sizeof(VSD3DStarter::Bone);
                if (dataSize < usedSize)
                    throw std::exception("End of file");

                // TODO - What to do with bone data?
                bones;
                model->m_bones.push_back(*(const_cast<VSD3DStarter::Bone*>(bones)));
            }

            // Animation Clips
            auto nClips = reinterpret_cast<const UINT*>(meshData + usedSize);
            usedSize += sizeof(UINT);
            if (dataSize < usedSize)
                throw std::exception("End of file");

            for (UINT j = 0; j < *nClips; ++j)
            {
                // Clip name
                nName = reinterpret_cast<const UINT*>(meshData + usedSize);
                usedSize += sizeof(UINT);
                if (dataSize < usedSize)
                    throw std::exception("End of file");

                auto clipName = reinterpret_cast<const wchar_t*>(meshData + usedSize);

                usedSize += sizeof(wchar_t)*(*nName);
                if (dataSize < usedSize)
                    throw std::exception("End of file");

                // TODO - What to do with clip name?
                clipName;

                auto anim_name = wcsstr(clipName, L"|");

                anim_name = anim_name + 1;

                auto clip = reinterpret_cast<const VSD3DStarter::Clip*>(meshData + usedSize);
                usedSize += sizeof(VSD3DStarter::Clip);
                if (dataSize < usedSize)
                    throw std::exception("End of file");

                if (!clip->keys)
                    throw std::exception("Keyframes missing in clip");

                for (UINT i = 0; i < clip->keys; i++)
                {
                    auto keys = reinterpret_cast<const VSD3DStarter::Keyframe*>(meshData + usedSize);
                    usedSize += sizeof(VSD3DStarter::Keyframe);
                    if (dataSize < usedSize)
                        throw std::exception("End of file");

                    // TODO - What to do with keys and clip->startTime, clip->endTime?
                    keys;
                    model->m_keys[anim_name].push_back(*(const_cast<VSD3DStarter::Keyframe*>(keys)));
                }
                model->m_clips.insert(std::make_pair(anim_name, *(const_cast<VSD3DStarter::Clip*>(clip))));
            }
        }

        bool enableSkinning = (*nSkinVBs) != 0;

        // Build vertex buffers
        std::vector<ComPtr<ID3D11Buffer>> vbs;
        vbs.resize(*nVBs);

        const size_t stride = enableSkinning ? sizeof(VertexPositionNormalTangentColorTextureSkinning)
            : sizeof(VertexPositionNormalTangentColorTexture);

        for (UINT j = 0; j < *nVBs; ++j)
        {
            size_t nVerts = vbData[j].nVerts;

            size_t bytes = stride * nVerts;

            D3D11_BUFFER_DESC desc = {};
            desc.Usage = D3D11_USAGE_DEFAULT;
            desc.ByteWidth = static_cast<UINT>(bytes);
            desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

            if (fxFactoryDGSL && !enableSkinning)
            {
                // Can use CMO vertex data directly
                D3D11_SUBRESOURCE_DATA initData = {};
                initData.pSysMem = vbData[j].ptr;

                ThrowIfFailed(
                    d3dDevice->CreateBuffer(&desc, &initData, &vbs[j])
                );
            }
            else
            {
                std::unique_ptr<uint8_t[]> temp(new uint8_t[bytes + (sizeof(UINT) * nVerts)]);

                auto visited = reinterpret_cast<UINT*>(temp.get() + bytes);
                memset(visited, 0xff, sizeof(UINT) * nVerts);

                assert(vbData[j].ptr != 0);

                if (enableSkinning)
                {
                    // Combine CMO multi-stream data into a single stream
                    auto skinptr = vbData[j].skinPtr;
                    assert(skinptr != 0);

                    uint8_t* ptr = temp.get();

                    auto sptr = vbData[j].ptr;

                    for (size_t v = 0; v < nVerts; ++v)
                    {
                        *reinterpret_cast<VertexPositionNormalTangentColorTexture*>(ptr) = *sptr;
                        ++sptr;

                        auto skinv = reinterpret_cast<VertexPositionNormalTangentColorTextureSkinning*>(ptr);
                        skinv->SetBlendIndices(*reinterpret_cast<const XMUINT4*>(skinptr[v].boneIndex));
                        skinv->SetBlendWeights(*reinterpret_cast<const XMFLOAT4*>(skinptr[v].boneWeight));

                        ptr += stride;
                    }
                }
                else
                {
                    memcpy(temp.get(), vbData[j].ptr, bytes);
                }

                if (!fxFactoryDGSL)
                {
                    // Need to fix up VB tex coords for UV transform which is not supported by basic effects
                    for (UINT k = 0; k < *nSubmesh; ++k)
                    {
                        auto& sm = subMesh[k];

                        if (sm.vertexBufferIndex != j)
                            continue;

                        if ((sm.indexBufferIndex >= *nIBs)
                            || (sm.materialIndex >= *nMats))
                            throw std::exception("Invalid submesh found\n");

                        XMMATRIX uvTransform = XMLoadFloat4x4(&materials[sm.materialIndex].pMaterial->UVtransform);

                        auto ib = ibData[sm.indexBufferIndex].ptr;

                        size_t count = ibData[sm.indexBufferIndex].nIndices;

                        for (size_t q = 0; q < count; ++q)
                        {
                            size_t v = ib[q];

                            if (v >= nVerts)
                                throw std::exception("Invalid index found\n");

                            auto verts = reinterpret_cast<VertexPositionNormalTangentColorTexture*>(temp.get() + (v * stride));
                            if (visited[v] == UINT(-1))
                            {
                                visited[v] = sm.materialIndex;

                                XMVECTOR t = XMLoadFloat2(&verts->textureCoordinate);

                                t = XMVectorSelect(g_XMIdentityR3, t, g_XMSelect1110);

                                t = XMVector4Transform(t, uvTransform);

                                XMStoreFloat2(&verts->textureCoordinate, t);
                            }
                            else if (visited[v] != sm.materialIndex)
                            {
#ifdef _DEBUG
                                XMMATRIX uv2 = XMLoadFloat4x4(&materials[visited[v]].pMaterial->UVtransform);

                                if (XMVector4NotEqual(uvTransform.r[0], uv2.r[0])
                                    || XMVector4NotEqual(uvTransform.r[1], uv2.r[1])
                                    || XMVector4NotEqual(uvTransform.r[2], uv2.r[2])
                                    || XMVector4NotEqual(uvTransform.r[3], uv2.r[3]))
                                {
                                    DebugTrace("WARNING: %ls - mismatched UV transforms for the same vertex; texture coordinates may not be correct\n", mesh->name.c_str());
                                }
#endif
                            }
                        }
                    }
                }

                // Create vertex buffer from temporary buffer
                D3D11_SUBRESOURCE_DATA initData = {};
                initData.pSysMem = temp.get();

                ThrowIfFailed(
                    d3dDevice->CreateBuffer(&desc, &initData, &vbs[j])
                );
            }

            SetDebugObjectName(vbs[j].Get(), "ModelCMO");
        }

        assert(vbs.size() == *nVBs);

        // Create Effects
        for (UINT j = 0; j < *nMats; ++j)
        {
            auto& m = materials[j];

            if (fxFactoryDGSL)
            {
                DGSLEffectFactory::DGSLEffectInfo info;
                info.name = m.name.c_str();
                info.specularPower = m.pMaterial->specularPower;
                info.perVertexColor = true;
                info.enableSkinning = enableSkinning;
                info.alpha = m.pMaterial->diffuse.w;
                info.ambientColor = XMFLOAT3(m.pMaterial->ambient.x, m.pMaterial->ambient.y, m.pMaterial->ambient.z);
                info.diffuseColor = XMFLOAT3(m.pMaterial->diffuse.x, m.pMaterial->diffuse.y, m.pMaterial->diffuse.z);
                info.specularColor = XMFLOAT3(m.pMaterial->specular.x, m.pMaterial->specular.y, m.pMaterial->specular.z);
                info.emissiveColor = XMFLOAT3(m.pMaterial->emissive.x, m.pMaterial->emissive.y, m.pMaterial->emissive.z);
                info.diffuseTexture = m.texture[0].empty() ? nullptr : m.texture[0].c_str();
                info.specularTexture = m.texture[1].empty() ? nullptr : m.texture[1].c_str();
                info.normalTexture = m.texture[2].empty() ? nullptr : m.texture[2].c_str();
                info.pixelShader = m.pixelShader.c_str();

                const int offset = DGSLEffectFactory::DGSLEffectInfo::BaseTextureOffset;
                for (int i = 0; i < (DGSLEffect::MaxTextures - offset); ++i)
                {
                    info.textures[i] = m.texture[i + offset].empty() ? nullptr : m.texture[i + offset].c_str();
                }

                m.effect = fxFactoryDGSL->CreateDGSLEffect(info, nullptr);

                auto dgslEffect = static_cast<DGSLEffect*>(m.effect.get());
                dgslEffect->SetUVTransform(XMLoadFloat4x4(&m.pMaterial->UVtransform));
            }
            else
            {
                EffectFactory::EffectInfo info;
                info.name = m.name.c_str();
                info.specularPower = m.pMaterial->specularPower;
                info.perVertexColor = true;
                info.enableSkinning = enableSkinning;
                info.alpha = m.pMaterial->diffuse.w;
                info.ambientColor = XMFLOAT3(m.pMaterial->ambient.x, m.pMaterial->ambient.y, m.pMaterial->ambient.z);
                info.diffuseColor = XMFLOAT3(m.pMaterial->diffuse.x, m.pMaterial->diffuse.y, m.pMaterial->diffuse.z);
                info.specularColor = XMFLOAT3(m.pMaterial->specular.x, m.pMaterial->specular.y, m.pMaterial->specular.z);
                info.emissiveColor = XMFLOAT3(m.pMaterial->emissive.x, m.pMaterial->emissive.y, m.pMaterial->emissive.z);
                info.diffuseTexture = m.texture[0].c_str();

                m.effect = fxFactory.CreateEffect(info, nullptr);
            }

            CreateInputLayout(d3dDevice, m.effect.get(), &m.il, enableSkinning);
        }

        // Build mesh parts
        for (UINT j = 0; j < *nSubmesh; ++j)
        {
            auto& sm = subMesh[j];

            if ((sm.indexBufferIndex >= *nIBs)
                || (sm.vertexBufferIndex >= *nVBs)
                || (sm.materialIndex >= *nMats))
                throw std::exception("Invalid submesh found\n");

            auto& mat = materials[sm.materialIndex];

            auto part = new ModelMeshPart();

            if (mat.pMaterial->diffuse.w < 1)
                part->isAlpha = true;

            part->indexCount = sm.primCount * 3;
            part->startIndex = sm.startIndex;
            part->vertexStride = static_cast<UINT>(stride);
            part->inputLayout = mat.il;
            part->indexBuffer = ibs[sm.indexBufferIndex];
            part->vertexBuffer = vbs[sm.vertexBufferIndex];
            part->effect = mat.effect;
            part->vbDecl = enableSkinning ? g_vbdeclSkinning : g_vbdecl;

            mesh->meshParts.emplace_back(part);
        }

        model->meshes.emplace_back(mesh);
    }

    return model;
}

std::unique_ptr<AnimationModel> AnimationModel::AnimCreateFromCMO(ID3D11Device * d3dDevice, const wchar_t * szFileName, DirectX::IEffectFactory & fxFactory, bool ccw, bool pmalpha)
{
    size_t dataSize = 0;
    std::unique_ptr<uint8_t[]> data;
    HRESULT hr = BinaryReader::ReadEntireFile(szFileName, data, &dataSize);
    if (FAILED(hr))
    {
        DebugTrace("CreateFromCMO failed (%08X) loading '%ls'\n", hr, szFileName);
        throw std::exception("CreateFromCMO");
    }

    auto model = AnimCreateFromCMO(d3dDevice, data.get(), dataSize, fxFactory, ccw, pmalpha);

    model->name = szFileName;

    return model;
}

AnimationModel::~AnimationModel()
{
}

/// <summary>
/// 親子関係の構築
/// </summary>
/// <param name="current_boneindex">ボーン番号</param>
/// <param name="skinning_info">ボーン情報</param>
/// <param name="bone_world_transform">ボーンのワールド座標</param>
void AnimationModel::CombineTransforms(UINT current_boneindex, std::vector<VSD3DStarter::Bone> const & skinning_info, std::vector<DirectX::XMFLOAT4X4>& bone_world_transform)
{
    auto bone = skinning_info[current_boneindex];
    if (m_is_transform_combined[current_boneindex] || bone.parentIndex < 0 || bone.parentIndex == static_cast<int>(current_boneindex))
    {
        m_is_transform_combined[current_boneindex] = true;
        return;
    }

    CombineTransforms(bone.parentIndex, skinning_info, bone_world_transform);

    XMMATRIX left_mat = XMLoadFloat4x4(&bone_world_transform[current_boneindex]);
    XMMATRIX right_mat = XMLoadFloat4x4(&bone_world_transform[bone.parentIndex]);

    XMMATRIX ret = left_mat * right_mat;

    XMStoreFloat4x4(&bone_world_transform[current_boneindex], ret);

    m_is_transform_combined[current_boneindex] = true;
}

/// <summary>
/// アニメーションの更新
/// </summary>
/// <param name="clip_name">アニメーションクリップ名</param>
void AnimationModel::UpdateAnimation(std::wstring clip_name)
{
    struct aligned_deleter { void operator()(void* p) { _aligned_free(p); } };

    std::unique_ptr<XMMATRIX[], aligned_deleter> bones(
        reinterpret_cast<XMMATRIX*>(_aligned_malloc(
            sizeof(XMMATRIX) * m_anim_state.m_bone_world_transforms.size(), 16)));

    m_anim_state.animTime += 1 / 60.0f;

    //ボーンの更新
    for (UINT i = 0; i < m_bones.size(); i++)
    {
        m_anim_state.m_bone_world_transforms[i] = m_bones[i].localTransform;
    }

    m_anim_state.end = false;

    //キーフレームの取得
    auto found = m_clips.find(clip_name);
    if (found != m_clips.end())
    {
        const auto& kf = m_keys[clip_name];
        for (const auto& frame : kf)
        {
            if (frame.time > m_anim_state.animTime)
            {
                break;
            }

            m_anim_state.m_bone_world_transforms[frame.boneIndex] = frame.transform;
        }

        //位置情報の更新
        m_is_transform_combined.assign(m_bones.size(), false);

        for (UINT i = 0; i < m_bones.size(); i++)
        {
            CombineTransforms(i, m_bones, m_anim_state.m_bone_world_transforms);
        }

        //ワールド座標からの差分を計算
        for (UINT i = 0; i < m_bones.size(); i++)
        {
            DirectX::XMMATRIX left_mat = XMLoadFloat4x4(&m_bones[i].invBindPos);
            DirectX::XMMATRIX right_mat = XMLoadFloat4x4(&m_anim_state.m_bone_world_transforms[i]);

            DirectX::XMMATRIX ret = left_mat * right_mat;

            bones[i] = ret;
        }

        //アニメーション時間の更新
        if (m_anim_state.animTime > found->second.endTime)
        {
            m_anim_state.end = true;
            m_anim_state.animTime = found->second.startTime + (m_anim_state.animTime - found->second.endTime);
        }
    }

    for (std::shared_ptr<ModelMesh> mesh : meshes)
    {
        for (UINT i = 0; i < mesh->meshParts.size(); i++)
        {
            auto effect = dynamic_cast<SkinnedEffect*>(mesh->meshParts[i]->effect.get());

            effect->SetBoneTransforms(bones.get(), m_anim_state.m_bone_world_transforms.size());
        }
    }
}