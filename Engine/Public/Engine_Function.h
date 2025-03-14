#pragma once

namespace Engine
{
	template<typename T>
	void Safe_Delete(T& Pointer)
	{
		if (nullptr != Pointer)
		{
			delete Pointer;
			Pointer = nullptr;
		}
	}

	template<typename T>
	void Safe_Delete_Array(T& Pointer)
	{
		if (nullptr != Pointer)
		{
			delete[] Pointer;
			Pointer = nullptr;
		}
	}

	template<typename T>
	unsigned int Safe_AddRef(T& Instance)
	{
		unsigned int iRefCnt = { 0 };

		if (nullptr != Instance)
			iRefCnt = Instance->AddRef();

		return iRefCnt;
	}

	template<typename T>
	unsigned int Safe_Release(T& Instance)
	{
		unsigned int iRefCnt = { 0 };

		if (nullptr != Instance)
		{
			iRefCnt = Instance->Release();
			if (0 == iRefCnt)
				Instance = nullptr;
		}
		return iRefCnt;
	}

	inline D3DXMATRIX MAtrixTranslation(float x, float y, float z) {
		D3DXMATRIX mat;
		D3DXMatrixIdentity(&mat);
		D3DXMatrixTranslation(&mat, x, y, z);
		return mat;
	}

	inline D3DXVECTOR3 MAtrix_Trasform(const D3DXMATRIX& mat) {
		D3DXVECTOR3 temp = { mat._41, mat._42, mat._43 };
		return temp;
	}

}
