#pragma once

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

        // * 곱셉_연산
        Matrix operator*(const Matrix& rhs)
        {
            Matrix result;
            D3DXMatrixMultiply(&result, this, &rhs);
            return result;
        }
        Matrix operator*(const D3DXMATRIX& rhs) const
        {
            Matrix result;
            D3DXMatrixMultiply(&result, this, &rhs);
            return result;
        }
        friend Matrix operator*(const D3DXMATRIX& lhs, const Matrix& rhs)
        {
            Matrix result;
            D3DXMatrixMultiply(&result, &lhs, &rhs);
            return result;
        }

        //  = 대입연산
        Matrix& operator=(const D3DXMATRIX& mat)
        {
            memcpy(this, &mat, sizeof(D3DXMATRIX));
            return *this;
        }
        const Matrix operator=(const D3DXMATRIX& rhs) const { return Matrix(rhs); }

        // *주소 접근연산자
        Matrix* operator&() { return this; }
        const Matrix* operator&() const { return this; }

        // 형변환 연산자 (Matrix 객체를 D3DXMATRIX* 타입으로 변환할 수 있도록 해주는 형 변환 연산자)
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

        
        
    public:
        _float3 Get_State(TRANSFORMSTATE eState) const
        {
            return _float3(m[eState][0], m[eState][1], m[eState][2]);
        }

        void Set_State(TRANSFORMSTATE eState, const _float3& vState) {
            m[eState][0] = vState.x;
            m[eState][1] = vState.y;
            m[eState][2] = vState.z;
        }

        void Go_Straight(_float fTimeDelta, _float fSpeedPerSec)
        {
            _float3		vPosition = Get_State(STATE_POSITION);
            _float3		vLook = Get_State(STATE_LOOK);

            vPosition += *D3DXVec3Normalize(&vLook, &vLook) * fSpeedPerSec * fTimeDelta;

            Set_State(STATE_POSITION, vPosition);
        }

        void Go_Vector(TRANSFORMSTATE eState, _float fTimeDelta, _float fSpeedPerSec)
        {
            _float3		vPosition = Get_State(STATE_POSITION);
            _float3		vVector3 = Get_State(eState);

            vPosition += *D3DXVec3Normalize(&vVector3, &vVector3) * fSpeedPerSec * fTimeDelta;

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

        _float4x4 Turn_Radian(const _float3& vAxis, _float Radian)
        {
            _float3			vRight = Get_State(STATE_RIGHT);
            _float3			vUp = Get_State(STATE_UP);
            _float3			vLook = Get_State(STATE_LOOK);

            _float4x4		RotationMatrix;
            D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, Radian);

            D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
            D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
            D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

            Set_State(STATE_RIGHT, vRight);
            Set_State(STATE_UP, vUp);
            Set_State(STATE_LOOK, vLook);

            return *this;
        }
        _float4x4 Turn_Radian_Safe_Scale(const _float3& vAxis, _float Radian)
        {
            // 현재 방향 벡터 추출
            _float3 vRight = Get_State(STATE_RIGHT);
            _float3 vUp = Get_State(STATE_UP);
            _float3 vLook = Get_State(STATE_LOOK);

            // 스케일 추출 (각 축 벡터의 길이)
            _float fScaleX = D3DXVec3Length(&vRight);
            _float fScaleY = D3DXVec3Length(&vUp);
            _float fScaleZ = D3DXVec3Length(&vLook);

            // 방향 벡터 정규화
            D3DXVec3Normalize(&vRight, &vRight);
            D3DXVec3Normalize(&vUp, &vUp);
            D3DXVec3Normalize(&vLook, &vLook);

            // 회전 행렬 생성
            _float4x4 RotationMatrix;
            D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, Radian);

            // 회전 적용
            D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
            D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
            D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

            // 원래 스케일 재적용
            vRight *= fScaleX;
            vUp *= fScaleY;
            vLook *= fScaleZ;

            // 상태 갱신
            Set_State(STATE_RIGHT, vRight);
            Set_State(STATE_UP, vUp);
            Set_State(STATE_LOOK, vLook);

            return *this;
        }

        _float4x4 Scaling(_float fX, _float fY, _float fZ)
        {
            _float3		vRight = Get_State(STATE_RIGHT);
            _float3		vUp = Get_State(STATE_UP);
            _float3		vLook = Get_State(STATE_LOOK);

            Set_State(STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * fX);
            Set_State(STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * fY);
            Set_State(STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * fZ);

            return *this;
        }
    };
}