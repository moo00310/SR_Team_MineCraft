#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Anim;
class CSkeletalAnimator;
class CShader;
END

BEGIN(Client)
class CPawn abstract : public CGameObject
{
public:
	enum ANIM
	{
		IDLE, RUN, WALK, ATTACK, DEAD, ANIM_END
	};

	enum RENDERCOLOR {
		RENDERORIGIN=1, RENDERATTACKED=5, RENDERBOOM=6, RENDERCOLOR_END
	};

protected:
	CPawn(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPawn(const CPawn& Prototype);
	virtual ~CPawn() = default;

public:
	virtual HRESULT Initialize_Prototype() = 0;
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta) = 0;
	virtual HRESULT Render();

public:
	CTransform* Get_Transform() { return m_pTransformCom; }
	CRigidbody* Get_RigidBody() { return m_pRigidbodyCom; }
	CCollider*	Get_Collider() { return m_pCollider_CubeCom; }
	void		Set_Animation(ANIM etype) { m_eCurAnim = etype; }
	void		Set_Hp(float hp) { m_Hp = hp; }

	void		Add_Hp(float damage) { m_Hp += damage; }
	_float		Get_Hp() { return m_Hp; }

	ANIM Get_ANIM() { return m_eCurAnim; }
public:
	void Reset_Ainmation();
	virtual void Knock_back(const _float3& vforce) = 0;

protected:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CSkeletalAnimator* m_skelAnime = { nullptr };
	vector<CVIBuffer_Anim*> m_pVIBufferComs;
	CShader* m_pShaderCom = { nullptr };
	class CMCTerrain* m_pTerrain = { nullptr };

protected:
	CCollider_Cube* m_pCollider_CubeCom = { nullptr };
	CRigidbody* m_pRigidbodyCom = { nullptr };
protected:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_Bone() = 0;
	virtual HRESULT Ready_Animation() = 0;

protected:
	virtual void Update_State(_float fTimeDelta) = 0;
	virtual void Motion_Idle(_float fTimeDelta) = 0;
	virtual void Motion_Walk(_float fTimeDelta) = 0;
	virtual void Turn(_float fTimeDelta) = 0;

protected:
	ANIM m_eCurAnim = { ANIM_END };
	_float m_fSpeed = {};
	_float m_MaxHp = {};
	_float m_Hp = {};
	_float m_sound = {};
protected:
	_float3 m_Coll_Size = {};
	_float3 m_Coll_Offset = {};
	_bool isDead = { false };

	RENDERCOLOR m_eColor = RENDERORIGIN;
	int m_iGetHitFrame = 0;
	bool m_bGetHit = false;

protected:
	// 추상클래스 
	virtual void FrameCallback(int animType, int frame) = 0;

protected:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free();

};
END
