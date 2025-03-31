#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Anim;
class CSkeletalAnimator;
END

class CPawn abstract : public CGameObject
{
public:
	enum ANIM
	{
		IDLE, RUN, WALK, ATTACK, DEAD, ANIM_END
	};

protected:
	CPawn(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPawn(const CPawn& Prototype);
	virtual ~CPawn() = default;

public:
	virtual HRESULT Initialize_Prototype() = 0;
	virtual HRESULT Initialize(void* pArg) = 0;
	virtual void Priority_Update(_float fTimeDelta) = 0;
	virtual void Update(_float fTimeDelta) = 0;
	virtual void Late_Update(_float fTimeDelta) = 0;
	virtual HRESULT Render() = 0;

public:
	CTransform* Get_Transform() { return m_pTransformCom; }
	CRigidbody* Get_RigidBody() { return m_pRigidbodyCom; }
	void		Set_Animation(ANIM etype) { m_eCurAnim = etype; }
	void		Set_Hp(float hp) { m_Hp = hp; }

	void		Add_Hp(float damage) { m_Hp += damage; }
	_float		Get_Hp() { return m_Hp; }

public:
	void Reset_Ainmation();
	virtual void Knock_back(const _float3& vforce) = 0;

protected:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CSkeletalAnimator* m_skelAnime = { nullptr };
	vector<CVIBuffer_Anim*> m_pVIBufferComs;

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

protected:
	_float3 m_Coll_Size = {};
	_float3 m_Coll_Offset = {};
	_bool isDead = { false };


protected:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free();

};

