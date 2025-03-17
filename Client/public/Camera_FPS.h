#pragma once
#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_FPS final : public CCamera
{
public:
	typedef struct tagCameraFPSDesc : public CCamera::CAMERA_DESC
	{
		_float			fMouseSensor{};
		CGameObject*	pTarget = { nullptr };
		//CTransform*		pTargetTransformCom{ nullptr };
	} CAMERA_FPS_DESC;
private:
	CCamera_FPS(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCamera_FPS(const CCamera_FPS& Prototype);
	virtual ~CCamera_FPS() = default;

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
	static CCamera_FPS* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END

