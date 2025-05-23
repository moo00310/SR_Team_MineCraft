﻿#pragma once

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

	class CTag_Finder
	{
	private:
		std::basic_string<TCHAR> m_str;

	public:
		explicit CTag_Finder(const TCHAR* pStr)
			: m_str(pStr) 
		{
		}

		template<typename T>
		bool operator()(const T& Pair) const
		{
			return m_str == Pair.first;
		}
	};

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


	// 보간 함수
	inline D3DXVECTOR3 Lerp(const D3DXVECTOR3& start, const D3DXVECTOR3& end, float t)
	{
		return start + (end - start) * t;
	}


	inline D3DXMATRIX InterpolateMatrix(const D3DXMATRIX& mat1, const D3DXMATRIX& mat2, float t)
	{
		D3DXMATRIX result;
		D3DXMatrixIdentity(&result);

		// 위치 보간
		D3DXVECTOR3 pos1(mat1._41, mat1._42, mat1._43);
		D3DXVECTOR3 pos2(mat2._41, mat2._42, mat2._43);
		D3DXVECTOR3 interpolatedPos = Lerp(pos1, pos2, t);

		// 행렬 직접 보간
		for (int i = 0; i < 3; ++i)  // X, Y, Z 축
		{
			result.m[i][0] = mat1.m[i][0] * (1.0f - t) + mat2.m[i][0] * t;
			result.m[i][1] = mat1.m[i][1] * (1.0f - t) + mat2.m[i][1] * t;
			result.m[i][2] = mat1.m[i][2] * (1.0f - t) + mat2.m[i][2] * t;
		}

		// Orthonormalization (정규화)
		D3DXVECTOR3 xAxis(result.m[0][0], result.m[0][1], result.m[0][2]);
		D3DXVECTOR3 yAxis(result.m[1][0], result.m[1][1], result.m[1][2]);
		D3DXVECTOR3 zAxis(result.m[2][0], result.m[2][1], result.m[2][2]);

		D3DXVec3Normalize(&xAxis, &xAxis);
		D3DXVec3Cross(&yAxis, &zAxis, &xAxis);
		D3DXVec3Normalize(&yAxis, &yAxis);
		D3DXVec3Cross(&zAxis, &xAxis, &yAxis);
		D3DXVec3Normalize(&zAxis, &zAxis);

		result.m[0][0] = xAxis.x; result.m[0][1] = xAxis.y; result.m[0][2] = xAxis.z;
		result.m[1][0] = yAxis.x; result.m[1][1] = yAxis.y; result.m[1][2] = yAxis.z;
		result.m[2][0] = zAxis.x; result.m[2][1] = zAxis.y; result.m[2][2] = zAxis.z;

		// 보간된 위치 적용
		result._41 = interpolatedPos.x;
		result._42 = interpolatedPos.y;
		result._43 = interpolatedPos.z;

		return result;
	}

	inline D3DXMATRIX InterpolateMatrix_Quat(const D3DXMATRIX& mat1, const D3DXMATRIX& mat2, float t)
	{
		/// 위치 보간
		D3DXVECTOR3 pos1(mat1._41, mat1._42, mat1._43);
		D3DXVECTOR3 pos2(mat2._41, mat2._42, mat2._43);
		D3DXVECTOR3 interpPos = Lerp(pos1, pos2, t);

		// 회전 보간
		D3DXQUATERNION q1, q2, qInterp;
		D3DXQuaternionRotationMatrix(&q1, &mat1);
		D3DXQuaternionRotationMatrix(&q2, &mat2);
		D3DXQuaternionSlerp(&qInterp, &q1, &q2, t);

		// 스케일 추출
		D3DXVECTOR3 scale1, scale2;
		D3DXVECTOR3 dummyTrans;
		D3DXQUATERNION dummyRot;

		D3DXMatrixDecompose(&scale1, &dummyRot, &dummyTrans, &mat1);
		D3DXMatrixDecompose(&scale2, &dummyRot, &dummyTrans, &mat2);
		D3DXVECTOR3 interpScale = Lerp(scale1, scale2, t);

		// 최종 변환 행렬 구성
		D3DXMATRIX scaleMat, rotMat, transMat, result;
		D3DXMatrixScaling(&scaleMat, interpScale.x, interpScale.y, interpScale.z);
		D3DXMatrixRotationQuaternion(&rotMat, &qInterp);
		D3DXMatrixTranslation(&transMat, interpPos.x, interpPos.y, interpPos.z);

		result = scaleMat * rotMat * transMat;
		return result;
	}


	inline int Sign(float x) {
		return (x >= 0.f) ? 1 : -1;
	}
}

template <typename T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
	return (v < lo) ? lo : (v > hi) ? hi : v;
}

// 선형 보간 함수 (lerp)
template <typename T>
T Lerp(T a, T b, T t)
{
	return a + t * (b - a);
}


