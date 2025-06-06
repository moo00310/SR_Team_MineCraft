#pragma once
#include "Client_Defines.h"
#include "RightHand_Object.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Anim_Cube;
class CSkeletalAnimator;
END

class CArm_Steve : public CRightHand_Object
{
public:
	typedef struct tagArmesc 
	{
		CGameObject* pTarget = { nullptr };
	} DESC;

private:
	CArm_Steve(LPDIRECT3DDEVICE9 pGraphic_Device);
	CArm_Steve(const CArm_Steve& Prototype);
	virtual ~CArm_Steve() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	HRESULT Ready_Components();
	HRESULT Ready_Bone() override;
	HRESULT Ready_Animation() override;

private:
	void Update_State(_float fTimeDelta)override;
	void Motion_Idle(_float fTimeDelta)override;
	void Motion_Swing(_float fTimeDelta)override;
	void Motion_Walk(_float fTimeDelta)override;
	void Motion_Run(_float fTimeDelta)override;
	void KeyInput()override;

public:
	static CArm_Steve* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};