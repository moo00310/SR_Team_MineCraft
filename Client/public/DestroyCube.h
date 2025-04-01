#pragma once
#include "Cube.h"
#include "VIBuffer_Cube_Only.h"

BEGIN(Client)

// 금 가는 효과를 가진 큐브.
class CDestroyCube : public CCube
{
private:
	CDestroyCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CDestroyCube(const CDestroyCube& Prototype);
	virtual ~CDestroyCube() = default;

	HRESULT PrevRender();
	HRESULT EndRender();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

	HRESULT Ready_Components() override;

	static CDestroyCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

protected:
	CVIBuffer_Cube_Only* m_pVIBufferOnlyCom = {nullptr};
};

END