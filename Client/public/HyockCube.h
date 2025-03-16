#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "Texture.h"
#include "VIBuffer_Cube_Only.h"
#include "Cube.h"

class CHyockCube : public CCube
{
private:
	CHyockCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CHyockCube(const CHyockCube& Prototype);
	virtual ~CHyockCube() = default;

public:
	static CHyockCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

	// CGameObject을(를) 통해 상속됨
	CGameObject* Clone(void* pArg) override;

	virtual void Free() override;

protected:
	virtual HRESULT Ready_Components();

private:
	CTexture* m_TextureDestroyCom = {nullptr};
	CTexture* m_TextureDiamondOreCom = { nullptr };
	CVIBuffer_Cube_Only* m_CubeOnlyCom = { nullptr };
	CVIBuffer_Cube_Only* m_CubeCom = {nullptr};
};

