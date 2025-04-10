#pragma once

#include "Client_Defines.h"
#include "RightHand_Object.h"

class CLeft_Rect_Model final : public CRightHand_Object
{
protected:
	CLeft_Rect_Model(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLeft_Rect_Model(const CLeft_Rect_Model& Prototype);
	virtual ~CLeft_Rect_Model() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components();
	HRESULT Ready_Bone() override;
	HRESULT Ready_Animation() override;

private:
	void Update_State(_float fTimeDelta) override;
	void Motion_Idle(_float fTimeDelta) override;
	void Motion_Swing(_float fTimeDelta) override;
	void Motion_Walk(_float fTimeDelta) override;
	void Motion_Run(_float fTimeDelta) override;
	virtual void KeyInput() override;


public:
	static CLeft_Rect_Model* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();


};


