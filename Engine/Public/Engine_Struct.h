#pragma once

namespace Engine
{
	typedef struct tagEngineDesc 
	{
		HWND			hWnd;
		_bool			isWindowed;
		_uint			iWinSizeX;
		_uint			iWinSizeY;
		_uint			iNumLevels;
		_uint			iNumCollisionGroups;
	}ENGINE_DESC;

	/*D3DDECLUSAGE*/

	typedef struct tagVertexPositionTexcoord	
	{		
		D3DXVECTOR3		vPosition;
		D3DXVECTOR2		vTexcoord;	
	}VTXPOSTEX;

	typedef struct tagVertexPositionNormalTexcoord
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vNormal;
		D3DXVECTOR2		vTexcoord;
	}VTXNORTEX;


	typedef struct tagCreateCube
	{
		D3DXVECTOR2 Imagesize;
		D3DXVECTOR3 BoxSIze;
		D3DXVECTOR2 Pixelpos;

	}CUBE;
	
	typedef struct tagCreateCubeOnly
	{
		D3DXVECTOR3 BoxSIze;
	}CUBE_ONLY;

	// 파티클 구조체.
	typedef struct tagParticle
	{
		_float3		Position;
		D3DXCOLOR	Color;
	}PARTICLE;

#pragma region Collider_Cube
	typedef struct tagVertexCubeTexture
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vTexture;
	}VTXCUBETEX;

	typedef struct FaceIndices16
	{
		unsigned short _0, _1, _2;
	}FACEINDICES16;
#pragma endregion

	typedef struct tagbone
	{
		const char* name;
		int index;
		int parent;
		Matrix transform;
	}BONE;

#pragma region Ray_Cast
	typedef struct tagColorVertex
	{
		D3DXVECTOR3 vPosition;
		D3DXCOLOR   vColor;
	}VTXPOSCOL;
#pragma endregion

}

