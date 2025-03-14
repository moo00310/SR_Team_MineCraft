#pragma once
#include "Tool.h"

BEGIN(Client)

class CMapTool : public CTool
{
protected:
	CMapTool(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMapTool(const CMapTool& Prototype);
	virtual ~CMapTool() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

	void SetFrameOnOff();

private:
	bool m_bFrameOn = true;


	//지형 관련 변수
	int mapX = 10, mapY = 10, mapZ = 10;


public:
	static CMapTool* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END