//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:‹…‚ÌÕ“Ë”»’è *(ö‹Æ“àì¬)*
//
//day:2017/06/15
//
//acotr:Koshida Kenji
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/

#pragma once
#include <d3d11_1.h>
#include <SimpleMath.h>

class Sphere;
class Triangle;
class Segment;

bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, DirectX::SimpleMath::Vector3* inter = nullptr);

void ComputeTriangle(const DirectX::SimpleMath::Vector3 & _p0, const DirectX::SimpleMath::Vector3 & _p1, const DirectX::SimpleMath::Vector3 & _p2, Triangle * _triangle);

bool CheckPoint2Triangle(const DirectX::SimpleMath::Vector3& _point, const Triangle& _triangle);

bool CheckSphere2Triangle(const Sphere& _sphere, const Triangle& _tri, DirectX::SimpleMath::Vector3* _inter);

bool CheckSegment2Triangle(const Segment& _segment, const Triangle& _triangle, DirectX::SimpleMath::Vector3 *_inter);

void ClosestPtPoint2Triangle(const DirectX::SimpleMath::Vector3& _point, const Triangle& _tri, DirectX::SimpleMath::Vector3* pos);

//‹…
class Sphere
{
public:
    DirectX::SimpleMath::Vector3 center;	//’†SÀ•W
    float radius;							//”¼Œa

                                            //ƒRƒ“ƒXƒgƒ‰ƒNƒ^
    Sphere()
    {
        radius = 1.0f;
    }
};

//ü•ª
class Segment
{
public:
    DirectX::SimpleMath::Vector3 start;		//n“_À•W
    DirectX::SimpleMath::Vector3 end;		//I“_À•W
};

//OŠpŒ`
class Triangle
{
public:
    DirectX::SimpleMath::Vector3	P0;
    DirectX::SimpleMath::Vector3	P1;
    DirectX::SimpleMath::Vector3	P2;
    DirectX::SimpleMath::Vector3	Normal;	// –@üƒxƒNƒgƒ‹
};

