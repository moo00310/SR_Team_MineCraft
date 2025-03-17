#pragma once
#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_TPS final : public CCamera
{
public:
	typedef struct tagCameraTPSDesc : public CCamera::CAMERA_DESC
	{
		_float			fMouseSensor{};
		CGameObject*	pTarget = { nullptr };
		//CTransform*		pTargetTransformCom{ nullptr };
	} CAMERA_TPS_DESC;
private:
	CCamera_TPS(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCamera_TPS(const CCamera_TPS& Prototype);
	virtual ~CCamera_TPS() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	POINT				m_ptOldMousePos = {};
	_float				m_fMouseSensor = {};
	CTransform*			m_pTargetTransformCom = { nullptr };
	_float				m_fYaw{};
	_float				m_fPitch{};
	_float				m_fRoll{};
private:
	HRESULT Ready_Components();



public:
	static CCamera_TPS* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END

