#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Anim_Cube;
class CSkeletalAnimator;
END

class CRightHand_Object abstract : public CGameObject
{
public:
	enum ANIM {
		INIT, SWING, WALK, EAT, ANIM_END
	};

protected:
	CRightHand_Object(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRightHand_Object(const CRightHand_Object& Prototype);
	virtual ~CRightHand_Object() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

protected:
	CTexture* m_pTextureCom = { nullptr };
	vector<CVIBuffer_Anim*> m_pVIBufferComs = { nullptr };
	CSkeletalAnimator* m_pSkeletalAnimator = { nullptr };

protected:
	bool isAttack = { false };
	ANIM m_eCurAnim = { INIT };

protected:
	virtual HRESULT Ready_Components();
	virtual void Update_State(_float fTimeDelta);
	virtual HRESULT Update_Root(_float fTimeDelta);

protected:
	virtual HRESULT Ready_Bone() = 0;
	virtual HRESULT Ready_Animation() = 0;

protected:
	virtual void Motion_Idle(_float fTimeDelta) = 0;
	virtual void Motion_Swing(_float fTimeDelta) = 0;
	virtual void Motion_Walk(_float fTimeDelta) = 0;
	virtual void KeyInput();

public:
	virtual CGameObject* Clone(void* pArg) =0;
	virtual void Free();
};
