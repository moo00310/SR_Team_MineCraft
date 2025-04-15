#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Pawn.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
class CShader;
END

BEGIN(Client)
class CBossUI :public CGameObject
{
private:
	CBossUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBossUI(CBossUI& Prototype);
	virtual ~CBossUI() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	HRESULT Ready_Components();
	void Compout_Billboard(_float3& vPos, float fWidth, float fHeight);
	HRESULT Render_UI();


private:
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pTextureCom_Back = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CPawn* m_pPawn = { nullptr };


public:
	static CBossUI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END;

