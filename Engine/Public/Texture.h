#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
private:
	CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTexture(const CTexture& Prototype);
	virtual ~CTexture() = default;

public:
	LPDIRECT3DBASETEXTURE9 Get_Texture(_uint iIndex) {	return m_Textures[iIndex];}

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual HRESULT Initialize(void* pArg) override;
public:
	HRESULT Bind_Resource(_uint iIndex);
	HRESULT Bind_Resource(class CShader* pShader, D3DXHANDLE hParameters, _uint iIndex);
private:
	_uint							m_iNumTextures = { };
	vector<LPDIRECT3DTEXTURE9>		m_Textures;

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END