#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Anim_Cube;
class CSkeletalAnimator;
class CShader;
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

public:
	void ChangeTexture(int Num);
	void Set_Bright(float _bright) { m_bright = _bright; }

protected:
	CTexture* m_pTextureCom = { nullptr };
	vector<CVIBuffer_Anim*> m_pVIBufferComs = { nullptr };
	CSkeletalAnimator* m_pSkeletalAnimator = { nullptr };
	CShader* m_pShaderCom = { nullptr };

protected:
	int m_RederID = {};
	bool isAttack = { false };
	ANIM m_eCurAnim = { INIT };
	float m_bright = { 1 };

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

protected:
	int m_TextrueNum = 0;

public:
	virtual CGameObject* Clone(void* pArg) =0;
	virtual void Free();
};
