#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
END

class CRect_Model abstract : public CGameObject
{
public:
	enum ANIM {
		IDLE, SWING, ANIM_END, NONE
	};
	typedef struct tagItemDesc
	{
		wstring TextureTag = {};
		LEVEL TextureLevel = {};
	}DESC;

protected:
	CRect_Model(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRect_Model(const CRect_Model& Prototype);
	virtual ~CRect_Model() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;


private:
	HRESULT Update_Motion(_float fTimeDelta);
	void Swing_Sword(_float fTimeDelta);
	void Idle_Sword(_float fTimeDelta);


private:
	int m_bisTPS = { -1 };
	vector<KEYFREAME> m_swing;
	vector<KEYFREAME> m_IDLE;
	ANIM m_eCurAnim = IDLE;
};

