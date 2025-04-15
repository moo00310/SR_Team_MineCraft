#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END


BEGIN(Client)
class CSun : public CGameObject
{
private:
	CSun(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSun(const CSun& Prototype);
	virtual ~CSun() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

	void Set_ChunkCount(int _cnt) { m_iChunkCnt = _cnt; }

	float GetBight() const;
	bool Get_Sun() const { return m_isSun; }
	void Set_bAddTime() { m_bAddTimeFast = !m_bAddTimeFast; }
	void Set_bLastWave() { m_bLastWave = !m_bLastWave; }
private:
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pTextureCom2 = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	void Orbit_Around_Earth();

private:
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
	HRESULT Ready_Components();

	int m_iChunkCnt;
	float m_fBrightPercent = -0.01f;
	int m_brightFrame = 0;

	float m_fAngle = -59.48f;
	bool m_isSun = true;

	bool m_bAddTimeFast = false;
	bool m_bLastWave = false;
public:
	static CSun* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END