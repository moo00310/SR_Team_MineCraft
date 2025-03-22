#pragma once

#include "Client_Defines.h"
#include "Item_Model.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
END

class CSword final : public CItem_Model
{
public:
	enum ANIM {
		IDLE, SWING, ANIM_END
	};

private:
	CSword(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSword(const CSword& Prototype);
	virtual ~CSword() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	int m_bisTPS = { -1 };
	vector<KEYFREAME> m_swing;


public:
	static CSword* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

	HRESULT Ready_Bone() override;
	HRESULT Ready_Anime() override;
};