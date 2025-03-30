#pragma once

#include "Client_Defines.h"
#include "Pawn.h"
#include "ParticleSystem.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
class CSkeletalAnimator;
END

class CSteve final : public CPawn
{
public:
	enum ANIM_type
	{
		Swing_FF, Swing_BF, Swing_BA, Swing_FA, Swing_R, Swing_L, Attack ,INIT,
	};

private:
	CSteve(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSteve(const CSteve& Prototype);
	virtual ~CSteve() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	void SetPos(_float3 v3);
	_float3 GetPos();
	void Knock_back(const _float3& vforce) override;

private:
	_float   m_fMouseSensor = { 0.03f };
	int m_bisTPS = { 1 };
	bool isAttack = { false };

private:
	void	Input_Key(_float fTimeDelta);
	void	Move(_float fTimeDelta);

private:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT Ready_Bone() override;
	virtual HRESULT Ready_Animation() override;

private:
	void Update_State(_float fTimeDelta) override;
	void Motion_Idle(_float fTimeDelta) override;
	void Motion_Walk(_float fTimeDelta) override;
	void Turn(_float fTimeDelta) override;

public:
	static CSteve* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

};




