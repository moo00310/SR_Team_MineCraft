#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class Camera_Cutscene final : public CCamera
{
//이 클래스 안씀 이제!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//이 클래스 안씀 이제!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//이 클래스 안씀 이제!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//이 클래스 안씀 이제!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//이 클래스 안씀 이제!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//이 클래스 안씀 이제!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//이 클래스 안씀 이제!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//이 클래스 안씀 이제!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//이 클래스 안씀 이제!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//이 클래스 안씀 이제!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//혹시 몰라 냅둔거임(플레이어 카메라에 기능 통합-> 관리 용이)
public:
	typedef struct tagCameraFreeDesc : public CCamera::CAMERA_DESC
	{
		//_float			fMouseSensor{};
	} CAMERA_FREE_DESC;
private:
	Camera_Cutscene(LPDIRECT3DDEVICE9 pGraphic_Device);
	Camera_Cutscene(const Camera_Cutscene& Prototype);
	virtual ~Camera_Cutscene() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	void	Start_Cutscene(_float3 vPos);
	void	End_Cutscene();

private:
	_float3 m_vCutScene_LookPos{ 0.f, 0.f, 0.f };
	_float	m_fSpeed{ 1.f };

	//누적 각도
	_float	m_fAngle{ 0.f };
private:
	HRESULT Ready_Components();



public:
	static Camera_Cutscene* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END