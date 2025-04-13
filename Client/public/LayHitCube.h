#pragma once
#include "Cube.h"
#include "VIBuffer_Cube_Only.h"

BEGIN(Client)

// 금 가는 효과를 가진 큐브.
class CLayHitCube : public CCube
{
private:
	CLayHitCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLayHitCube(const CLayHitCube& Prototype);
	virtual ~CLayHitCube() = default;

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

	static CLayHitCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

	CTransform* GetTransform() const;

protected:
	CVIBuffer_Cube_Only* m_pVIBufferOnlyCom = { nullptr };

};

END