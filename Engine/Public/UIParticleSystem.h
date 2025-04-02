#pragma once

#include "GameInstance.h"
#include "UIObject.h"
#include "Texture.h"
#include "Transform.h"

BEGIN(Engine)

// UI�� ��ƼŬ.
class ENGINE_DLL CUIParticleSystem : public CUIObject
{
protected:
	CUIParticleSystem(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUIParticleSystem(const CUIParticleSystem& Prototype);
	virtual ~CUIParticleSystem() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Initialize_Prototype() override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;

	virtual HRESULT Render() override;
	HRESULT Bind_Buffers();

	virtual void Free() override;

	// Ǯ������ Push�� ȣ��.
	virtual void OnPushPool() override;

private:
	// ��ƼŬ �Ӽ� ����.
	void SetParticleAttribute();

protected:
	// ������ �� ���� ����.
	virtual HRESULT PrevRender();

	// ������ �� ���� ����.
	virtual HRESULT EndRender();

	HRESULT Create_VertexBuffer();

	// ��ƼŬ ��輱 ����.
	void SetParticleBoundingBox(ParticleBoundingBox box);

	// float �Ű������� DWORD ���·� ��ȯ.
	DWORD GetScale(float f);

	float GetRandomFloat(float lowBound, float highBound);

	// SetParticleAttribute ȣ�� �� ����.	
	// ��ƼŬ���� �� �Ӽ��� ���� ���ش�.
	virtual ParticleAttribute OnSetAddParticle() = 0;

	virtual HRESULT Ready_Components();

	// ��ƼŬ ��輱 ����� ȣ��
	// ��, IsBounding�� true ���¿��� ��.
	virtual void OnBoundingExit(ParticleAttribute& particle) = 0;

	// rgb������ dword�� ��ȯ.
	DWORD Float3ToHex(_float3 color);

protected:
	_uint	m_iFVF = {};
	LPDIRECT3DVERTEXBUFFER9 m_pVB = { nullptr };

	// ��ƼŬ ����.
	_uint iParticleCount = 0;

	// ��ƼŬ ���׸�Ʈ �� ä�� ����.
	DWORD dwVpBatchSize;

	// ����Ʈ ��������Ʈ ũ��.
	DWORD dwPointSize;

	// ��ƼŬ ��輱 Ȱ��ȭ.
	bool IsBounding = false;

	// ��ƼŬ �ؽ���.
	CTexture* m_pParticleTexture = { nullptr };

	// ��ƼŬ Ʈ������.
	CTransform* m_pTransform = { nullptr };

private:
	// ��ƼŬ ���׸�Ʈ �� ����.
	DWORD dwVpOffset = 0;

	// ��ƼŬ ��輱.
	ParticleBoundingBox m_boundingBox;
	
	// ��ƼŬ ����Ʈ.
	list<ParticleAttribute> m_ListParticleAttribute;
};

END