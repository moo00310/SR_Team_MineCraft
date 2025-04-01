#pragma once
#include "Client_Defines.h"
#include "Monster.h"
#include "ParticleEventManager.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
END

BEGIN(Client)

class CCreeper : public CMonster
{
public:
	enum ANIM_type
	{
		Swing_FF, Swing_BF, Swing_BA, Swing_FA, Swing_R, Swing_L, Attack, Dead, INIT,
	};

private:
	CCreeper(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCreeper(const CCreeper& Prototype);
	virtual ~CCreeper() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

	virtual void OnPushPool() override;

private:
	HRESULT Ready_Components();
	HRESULT Ready_Bone() override;
	HRESULT Ready_Animation() override;

private:
	void Update_State(_float fTimeDelta) override;
	void Motion_Idle(_float fTimeDelta) override;
	void Motion_Walk(_float fTimeDelta) override;
	void Motion_Attack(_float fTimeDelta) override;
	void Motion_Dead(_float fTimeDelta) override;
	void Turn(_float fTimeDelta) override;				

public:
	static CCreeper* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END