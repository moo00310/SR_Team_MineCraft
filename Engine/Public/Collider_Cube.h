#pragma once

#include "Component.h"
#include "Transform.h"
BEGIN(Engine)


class ENGINE_DLL CCollider_Cube final : public CComponent
{
public:
	typedef struct tagCollisionRectDesc
	{
		_float	fRadiusX = 0.5f, fRadiusY = 0.5f,  fRadiusZ = 0.5f;
		_float	fOffSetX = 0.f, fOffSetY = 0.f,  fOffsetZ = 0.f;
		_float4x4 StateMatrix;
		class CTransform* pTransformCom = { nullptr };
	}COLLRECTDESC;

private:
	CCollider_Cube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider_Cube(const CCollider_Cube& Prototype);
	virtual ~CCollider_Cube() = default;

public:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(void* pArg) override;
	HRESULT Update_ColliderBox(const _float4x4* WorldMatrix);
	HRESULT Render_ColliderBox(_bool isHit);
	_bool	Collision_Check(CCollider_Cube* pTarget, _float3* pOutDistance = nullptr);

public:
	//void Set_Position(CTransform* pTransform) {}

public:
	COLLRECTDESC Get_CollRectDesc() { return m_StateDesc; }
	_float3						m_vPoint[8];
	_bool						m_bIsInverse = false;

public:
	void Set_IsInverse(_bool bIsInverse) { m_bIsInverse = bIsInverse; }

protected:

	COLLRECTDESC		m_StateDesc;
	//콜라이더가 트랜스폼을 들어야할지 말아야할지 모르겠음
	//일단 오브젝트한테 정보 받아오는거로 임시로 해놓을 예정
	//D3DXVECTOR3			m_vCenter;
	//D3DXVECTOR3			m_vHalfExtents;
	//D3DXVECTOR3			m_vAxisX;
	//D3DXVECTOR3			m_vAxisY;
	//D3DXVECTOR3			m_vAxisZ;
private:
	//void Calculate_TransInfo();
	class CTransform*	m_pTransform = nullptr;
	//static const _tchar*		m_pTransformTag;

protected:
	LPDIRECT3DVERTEXBUFFER9  m_pVB = { nullptr };
	_uint						m_iNumVertices = {};
	_uint						m_iStride = {}; /* 정점의 크기(byte) */
	_ulong						m_dwFVF = {};
	D3DPRIMITIVETYPE			m_ePrimitiveType = {};
	_uint						m_iNumPrimitive = {};

protected:
	LPDIRECT3DINDEXBUFFER9		m_pIB = { nullptr };
	_uint						m_iIndicesByte = {};
	D3DFORMAT					m_eIndexFormat = {};


public:
	static CCollider_Cube* Create(LPDIRECT3DDEVICE9 pGraphic_Device/*, COLLRECTDESC* Des = nullptr*/);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END