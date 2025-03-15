#pragma once

#include "Engine_Defines.h"

namespace Engine
{
    struct Matrix : public _float4x4
    {
        enum TRANSFORMSTATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION };

        Matrix() { D3DXMatrixIdentity(this); }  // 기본 생성자 (단위 행렬)
        Matrix(const D3DXMATRIX& mat) { memcpy(this, &mat, sizeof(D3DXMATRIX)); } // 복사 생성자
        Matrix(
            FLOAT _11, FLOAT _12, FLOAT _13, FLOAT _14,
            FLOAT _21, FLOAT _22, FLOAT _23, FLOAT _24,
            FLOAT _31, FLOAT _32, FLOAT _33, FLOAT _34,
            FLOAT _41, FLOAT _42, FLOAT _43, FLOAT _44)
        {
            m[0][0] = _11; m[0][1] = _12; m[0][2] = _13; m[0][3] = _14;
            m[1][0] = _21; m[1][1] = _22; m[1][2] = _23; m[1][3] = _24;
            m[2][0] = _31; m[2][1] = _32; m[2][2] = _33; m[2][3] = _34;
            m[3][0] = _41; m[3][1] = _42; m[3][2] = _43; m[3][3] = _44;
        }

        // 행렬 곱셈 오버로딩
        _float4x4 operator*(const _float4x4& rhs)
        {
            _float4x4 result;
            D3DXMatrixMultiply(&result, this, &rhs);
            return result;
        }

        _float4 operator*(const _float4& vec) const
        {
            _float4 result;
            D3DXVec4Transform(&result, &vec, this);
            return result;
        }

        _float4x4& operator=(const D3DXMATRIX& mat)
        {
            memcpy(this, &mat, sizeof(D3DXMATRIX));
            return *this;
        }
        const _float4 operator=(const D3DXMATRIX& rhs) const { return _float4(rhs); }

        _float4x4* operator&() { return this; }
        const _float4x4* operator&() const { return this; }

        operator D3DXMATRIX* () { return reinterpret_cast<D3DXMATRIX*>(this); }
        operator const D3DXMATRIX* () const { return reinterpret_cast<const D3DXMATRIX*>(this); }

        D3DXVECTOR4& operator[](int index)
        {
            return *reinterpret_cast<D3DXVECTOR4*>(m[index]);
        }

        const D3DXVECTOR4& operator[](int index) const
        {
            return *reinterpret_cast<const D3DXVECTOR4*>(m[index]);
        }

        _float4x4 operator*(const D3DXMATRIX& rhs) const
        {
            _float4x4 result;
            D3DXMatrixMultiply(&result, this, &rhs);
            return result;
        }

        friend _float4x4 operator*(const D3DXMATRIX& lhs, const _float4x4& rhs)
        {
            _float4x4 result;
            D3DXMatrixMultiply(&result, &lhs, &rhs);
            return result;
        }

    public:
        _float3 Get_State(TRANSFORMSTATE eState) const
        {
            return _float3(m[eState][0], m[eState][1], m[eState][2]);
        }
        void Set_State(TRANSFORMSTATE eState, const _float3& vState) {
            m[eState][0] = vState.x;
            m[eState][1] = vState.y;
            m[eState][2] = vState.x;
        }

        void Go_Straight(_float fTimeDelta, _float fSpeedPerSec)
        {
            _float3		vPosition = Get_State(STATE_POSITION);
            _float3		vLook = Get_State(STATE_LOOK);

            vPosition += *D3DXVec3Normalize(&vLook, &vLook) * fSpeedPerSec * fTimeDelta;

            Set_State(STATE_POSITION, vPosition);
        }

        _float4x4 Turn(const _float3& vAxis, _float fTimeDelta, _float fSpeedPerSec)
        {
            _float3			vRight = Get_State(STATE_RIGHT);
            _float3			vUp = Get_State(STATE_UP);
            _float3			vLook = Get_State(STATE_LOOK);

            _float4x4		RotationMatrix;
            D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, fSpeedPerSec * fTimeDelta);

            D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
            D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
            D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

            Set_State(STATE_RIGHT, vRight);
            Set_State(STATE_UP, vUp);
            Set_State(STATE_LOOK, vLook);

            return *this;
        }

    };
}