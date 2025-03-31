#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Arm_Steve.h"
#include "Rect_Model.h"

class CRight_hand final : public CGameObject
{
private:
	CRight_hand(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRight_hand(const CRight_hand& Prototype);
	virtual ~CRight_hand() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);

private:
	CGameObject* m_pSteve = { nullptr };
	CGameObject* m_pArm_Model = { nullptr };
	CGameObject* m_pRect_Model = { nullptr };
	//CGameObject* m_pCube_Model;

private:
	bool isTPS = true;

public:
	static CRight_hand* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

