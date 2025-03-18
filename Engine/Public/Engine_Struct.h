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
	typedef struct tagVertexParticle
	{
		_float3		Position;
		D3DXCOLOR	Color;
	}VTXPARTICLE;

	// 파티클 속성 구조체.
	typedef struct tagParticleAttribute
	{
		// 파티클 포지션.
		_float3 vPosition;

		// 파티클 속도.
		_float3 vVelocity;		

		// 파티클 컬러.
		D3DXCOLOR vColor;	

		// 파티클 생존 여부.
		bool IsAlive = false;

		// 파티클 현재 지속 시간.
		float fCurrentTime;

		// 파티클 종료 시간.
		float fEndTime;

		// 파티클 타이머 활성화.
		bool IsTime = false;
	}ParticleAttribute;

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

