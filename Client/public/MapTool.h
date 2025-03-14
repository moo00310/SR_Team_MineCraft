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


private:
	void GeneratePerlinNoiseTexture(int width, int height);
	void GeneratePerlinNoiseTextureColor(int width, int height);

	bool m_bMainFrame = true;
	bool m_bBasicFrame = false;
	bool m_bMapFrame = false;
	bool m_bMapHeightFrame = false;
	bool m_bMapColorFrame = false;

	//지형 관련 변수
	int mapX = 10, mapY = 10, mapZ = 10;

	LPDIRECT3DTEXTURE9 heightMapTexture = nullptr;
	LPDIRECT3DTEXTURE9 ColorMapTexture = nullptr;
	float m_fFrequency = 0.01f; //작을수록 부드러움, 클수록 더 세밀함
	int m_iOctaves = 1; //낮을수록 부드러운 지형
	int m_iSeed = 12345; //다양한 종류의 맵
public:
	static CMapTool* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END


