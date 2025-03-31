#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Anim_Cube;
class CSkeletalAnimator;
END

class CArm_Steve : public CGameObject
{
public:
	enum ANIM {
		IDLE, SWING, WALK ,ANIM_END
	};

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
	CTexture* m_pTextureCom = { nullptr };
	vector<CVIBuffer_Anim*> m_pVIBufferComs = { nullptr };
	CSkeletalAnimator* m_pSkeletalAnimator = { nullptr };
	
private:
	bool isAttack = { false }; 
	ANIM m_eCurAnim = { IDLE };
	int m_bisTPS = { -1 };

private:
	HRESULT Ready_Components();
	HRESULT Ready_Bone();
	HRESULT Ready_Animation();

private:
	HRESULT Update_Root(_float fTimeDelta);
	void Update_State(_float fTimeDelta);
	void Motion_Idle(_float fTimeDelta);
	void Motion_Swing(_float fTimeDelta);
	void Motion_Walk(_float fTimeDelta);
	void KeyInput();

public:
	static CArm_Steve* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};