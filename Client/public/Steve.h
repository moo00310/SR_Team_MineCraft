#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
class CSkeletalAnimator;
END

class CSteve : public CGameObject
{
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

	void SetPos(_float3 v3);
	_float3 GetPos();

private:
	CTransform* m_pTransformCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Cube* m_pVIBufferCom[6];

	vector<BONE> vecBones;
	CCollider_Cube* m_pCollider_CubeCom{ nullptr };
	CRigidbody*		m_pRigidbodyCom{ nullptr };
	//CSkeletalAnimator* m_skelAnime = { nullptr };

	_float              m_fMouseSensor = { 0.03f };
	int m_bisTPS = { 1 };

private:
	void	Input_Key(_float fTimeDelta);
	void	Move(_float fTimeDelta);
	void	Turn(_float fTimeDelta);
private:
	HRESULT Ready_Components();
	HRESULT Ready_Bone();
	void UpdateBoneTransforms(int boneIndex, Matrix parentTransform);
	void Update_Mesh();


public:
	static CSteve* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};


