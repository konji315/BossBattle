//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:’n–Ê‚Ì”»’è@–ö‹Æ“àì¬–
//
//day:
//
//actor:
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/

#pragma once

#include <vector>
#include <memory>
#include <d3d11_1.h>
#include <SimpleMath.h>
#include "Classes\Collider\Collision.h"

// ’nŒ`“–‚½‚èƒf[ƒ^
class LandShapeData
{
public:
    friend class LandShape;

    // ’¸“_î•ñ‚Ì\‘¢‘Ìiƒtƒ@ƒCƒ‹‚©‚ç“Ç‚İ‚Şƒf[ƒ^j
    struct VERTEX_LANDSHAPE_FILE
    {
        DirectX::SimpleMath::Vector3	Pos;		// À•W
    };

    // ’¸“_î•ñ‚Ì\‘¢‘Ì
    struct VERTEX_LANDSHAPE
    {
        DirectX::SimpleMath::Vector3	Pos;		// À•W
        DirectX::SimpleMath::Vector3	Normal;		// –@ü
    };

    // ƒm[ƒhî•ñ
    struct NodeInfo
    {
        // í—Ş (0:bone 1:mesh)
        int kind;

        // –¼‘O
        char name[32];

        // ŒZ’í‚Ìƒm[ƒh”Ô†
        int sibling;

        // q‹Ÿ‚Ìƒm[ƒh”Ô†
        int child;

        // Šî–{p¨
        DirectX::SimpleMath::Vector3 translation;
        DirectX::SimpleMath::Quaternion rotation;
        DirectX::SimpleMath::Vector3 scale;
    };

    // –¼‘O
    std::wstring name;

    // ’¸“_ƒf[ƒ^”z—ñ
    std::vector<VERTEX_LANDSHAPE> m_Vertices;
    // ƒCƒ“ƒfƒbƒNƒXƒf[ƒ^”z—ñ
    std::vector<USHORT> m_Indices;
    // OŠpŒ`ƒf[ƒ^”z—ñ
    std::vector<Triangle> m_Triangles;

    // ƒ‚ƒfƒ‹‚ğ“Ç‚İ‚İ
    static std::unique_ptr<LandShapeData> CreateFromData(const char* meshData);
    static std::unique_ptr<LandShapeData> CreateFromFile(const wchar_t* szFileName);
    // À•W•ÏŠ·
    static void UpdateNodeMatrices(int index, const NodeInfo* nodeInfo_array, DirectX::SimpleMath::Matrix* pParentMatrix, std::vector<std::vector<VERTEX_LANDSHAPE>>& vertexArray);
};