#pragma once

namespace Engine
{
	typedef struct tagEngineDesc 
	{
		HWND			hWnd;
		bool			isWindowed;
		unsigned int	iWinSizeX;
		unsigned int	iWinSizeY;
		unsigned int	iNumLevels;		
	}ENGINE_DESC;

	/*D3DDECLUSAGE*/

	typedef struct tagVertexPositionTexcoord	
	{		
		D3DXVECTOR3		vPosition;
		D3DXVECTOR2		vTexcoord;	
	}VTXPOSTEX;

	typedef struct tagCreateCube
	{
		D3DXVECTOR2 Imagesize;
		D3DXVECTOR3 BoxSIze;
		D3DXVECTOR2 Pixelpos;

	}CUBE;

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
		D3DXMATRIX transform;
	}BONE;

#pragma region Ray_Cast
	typedef struct tagColorVertex
	{
		D3DXVECTOR3 vPosition;
		D3DXCOLOR   vColor;
	}VTXPOSCOL;
#pragma endregion

}

