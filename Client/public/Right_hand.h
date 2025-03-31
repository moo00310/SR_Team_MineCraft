#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Arm_Steve.h"


BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
END

class CRight_hand final : public CGameObject
{
public:
	typedef struct tagItemDesc
	{
		wstring TextureTag = {};
		LEVEL TextureLevel = {};
	}DESC;

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
	virtual HRESULT Render();

	CArm_Steve* m_pArm_Model;
	CGameObject* m_pCube_Model;
	CGameObject* m_pRect_Model;
};

