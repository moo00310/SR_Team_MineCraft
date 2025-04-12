#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Arm_Steve.h"
#include "RightHand_Object.h"
#include "Pawn.h"
#include "Camera_Player.h"

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

public:
	void OffRightHands();

private:
	CCamera_Player* m_pCamera = { nullptr };
	CPawn* m_pSteve = { nullptr };
	CRightHand_Object* m_pArm_Model = { nullptr };
	CRightHand_Object* m_pRect_Model = { nullptr };
	CRightHand_Object* m_pCube_Model = { nullptr };
	CRightHand_Object* m_pLeft_Rect_Model = { nullptr };

private:
	//bool isTPS = true;

private:
	void Chage_RightHand();
	void Chage_LeftHand();
	void Select_Render_Texture(ITEMNAME name);
	void Change_Matrix(ITEMNAME name);
	void Render_Arm();
	void Render_Rect();
	void Render_Cube();
	void Render_Left_Rect();

public:
	static CRight_hand* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

