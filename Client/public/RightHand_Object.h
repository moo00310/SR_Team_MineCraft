#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Steve.h"

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
		INIT, SWING, WALK, EAT, RUN, 
		ATTACK_1, ATTACK_2, ANIM_END
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
	void Set_isTps(bool isTps) { m_isTPS = isTps;  }

public:
	void Set_TPSMatrix(const Matrix& mat) { m_TPS_mat = mat; }
	void Set_FPSMatrix(const Matrix& mat) { m_FPS_mat = mat; }

public:
	void Set_RunAnimation();

protected:
	CTexture* m_pTextureCom = { nullptr };
	vector<CVIBuffer_Anim*> m_pVIBufferComs = { nullptr };
	CSkeletalAnimator* m_pSkeletalAnimator = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CSteve* m_pSteve = { nullptr };

protected:
	int m_RederID = {};
	ANIM m_eCurAnim = { INIT };
	bool m_isTPS = { true };

protected:
	virtual HRESULT Ready_Components();
	virtual HRESULT Update_Root(_float fTimeDelta);

protected:
	virtual HRESULT Ready_Bone() = 0;
	virtual HRESULT Ready_Animation() = 0;

protected:
	virtual void  Update_State(_float fTimeDelta) = 0;
	virtual void Motion_Idle(_float fTimeDelta) = 0;
	virtual void Motion_Swing(_float fTimeDelta) = 0;
	virtual void Motion_Walk(_float fTimeDelta) = 0;
	virtual void Motion_Run(_float fTimeDelta) = 0;
	virtual void KeyInput() = 0;


protected:
	int m_TextrueNum = 0;
	Matrix m_TPS_mat = {};
	Matrix m_FPS_mat = {};
	Matrix m_RunMatrix = {};

public:
	virtual CGameObject* Clone(void* pArg) =0;
	virtual void Free();
};
