#pragma once

#include "GameObject.h"
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc
	{
		_float3			vEye = {};
		_float3			vAt = {};
		_float			fFov = {};
		_float			fNear = {};
		_float			fFar = {};		
	}CAMERA_DESC;

protected:
	CCamera(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCamera(const CCamera& Prototype);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype();	
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void					Set_Active(_bool _isActive);
	const _float3&			Get_LookAt();
	void					Set_LookAt(const _float3& vLookAt);
	class CTransform*		Get_Transform();
	_float					Get_Yaw();
	_float					Get_Pitch();
	void					Set_Yaw(_float fYaw);
	void					Set_Pitch(_float fPitch);

protected:
	_float4x4				m_ViewMatrix = {};
	_float4x4				m_ProjMatrix = {};
	_float					m_fFov = {};
	_float					m_fAspect = {};
	_float					m_fNear = {};
	_float					m_fFar = {};

	class CTransform*		m_pTransformCom = {};
	_bool					m_isActive{ true };
protected:
	HRESULT Update_VP_Matrices();

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END