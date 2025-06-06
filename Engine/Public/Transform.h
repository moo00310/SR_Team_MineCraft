#pragma once

#include "Component.h"

/* 월드변환을 위한 월드 행렬을 보관한다. */
/* 월드행렬의 변환을 수행한다. == 월드공간에서의 상태를 변환한다 == 모델의 월드공간에서의 움직임을 제어한다. */


BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum TRANSFORMSTATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION };

	typedef struct tagTransformDesc
	{
		_float		fSpeedPerSec;
		_float		fRotationPerSec;
	}TRANSFORM_DESC;
private:
	CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTransform(const CTransform& Prototype);
	virtual ~CTransform() = default;

public:
	_float3 Get_State(TRANSFORMSTATE eState) const {
		return *reinterpret_cast<const _float3*>(&m_WorldMatrix.m[eState][0]);
	}

	const _float4x4* Get_WorldMatrix_Inverse() {
	
		D3DXMatrixInverse(&m_WorldMatrixInverse, nullptr, &m_WorldMatrix);
		return &m_WorldMatrixInverse;
	}

	const _float4x4* Get_WorldMatrix() {

		return &m_WorldMatrix;
	}

	void Set_State(TRANSFORMSTATE eState, const _float3& vState) {
		memcpy(&m_WorldMatrix.m[eState][0], &vState, sizeof vState);
	}

	void MultiplyMatrix(const D3DXMATRIX& mat) {
		D3DXMatrixMultiply(&m_WorldMatrix, &m_WorldMatrix, &mat); 
	}

	void Set_Matrix(const D3DXMATRIX& mat)
	{
		memcpy(&m_WorldMatrix, &mat, sizeof mat);
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Bind_Resource();
	HRESULT Bind_Resource(class CShader* pShader);
	HRESULT Bind_Resource_Scan(class CShader* pShader, _float _fScanRange);
	HRESULT Bind_Resource_WardenWave(class CShader* pShader, _float _fWaveRange);
	HRESULT Bind_Resource_BlockType(class CShader* pShader, _uint _iBlockType);
	HRESULT Bind_Resource_WardenPosition(class CShader* pShader, const _float4* _vWardenPosition);

public:
	_float3 Compute_Scaled() const;
	void Scaling(_float fX, _float fY, _float fZ);
	void Go_Straight(_float fTimeDelta);
	void Go_Backward(_float fTimeDelta);
	void Go_Left(_float fTimeDelta);
	void Go_Right(_float fTimeDelta);
	void Go_Up(_float fTimeDelta);
	void Go_Down(_float fTimeDelta);

	// 해당 방향으로 감.
	void Go_Direction(const _float3& _direction, _float fTimeDelta);

#pragma region COLLIDER_MOVE
	void Go_Direction(class CCollider* pCollider, _uint iGroupIndex, const _float3& vDirection, _float fTimeDelta);
	void Go_Straight(class CCollider* pCollider, _uint iGroupIndex, _float fTimeDelta);
	void Go_Backward(class CCollider* pCollider, _uint iGroupIndex, _float fTimeDelta);
	void Go_Left(class CCollider* pCollider, _uint iGroupIndex, _float fTimeDelta);
	void Go_Right(class CCollider* pCollider, _uint iGroupIndex, _float fTimeDelta);
	void Chase(class CCollider* pCollider, _uint iGroupIndex, const _float3& vTargetPos, _float fTimeDelta, _float fMinDistance = 0.1f);
#pragma endregion

	void LookAt(const _float3& vTargetPos);
	void LookAt_XZ(const _float3& vTargetPos);
	void Chase(const _float3& vTargetPos, _float fTimeDelta, _float fMinDistance = 0.1f);
	void Turn(const _float3& vAxis, _float fTimeDelta);
	void Rotation(const _float3& vAxis, _float fRadian);

	void Quaternion_Turn(const _float3& vAngle);
	void Quaternion_Rotation(const _float3& vAngle);
	void Quaternion_Revolution(const _float3& vAxis, const _float3& vCenter, _float fAngle);
	void Quaternion_Revolution_Pos(const _float3& vAxis, const _float3& vCenter, _float fAngle);
	_float3 RotateVectorByQuaternion(const _float3& v, const D3DXQUATERNION& q);
	void TurnByAngle(const _float3& vAxis, _float fAngle);

	// 월드 좌표를 스크린 좌표료 변환.
	_float2 WorldToScreen();

public:
	void Set_SpeedperSec(_float fSpeed) { m_fSpeedPerSec = fSpeed; }

private:
	_float4x4				m_WorldMatrix = {};
	_float4x4				m_WorldMatrixInverse = {};

	_float					m_fSpeedPerSec = {};
	_float					m_fRotationPerSec = {};

public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END