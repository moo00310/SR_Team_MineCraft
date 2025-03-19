#pragma once
#include "GameObject.h"
#include "Texture.h"

BEGIN(Engine)

class ENGINE_DLL CParticleSystem abstract : public CGameObject
{
protected:
	CParticleSystem(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticleSystem(const CParticleSystem& Prototype);
	virtual ~CParticleSystem() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Initialize_Prototype() override;	
	virtual void Update(_float fTimeDelta) override;

	virtual HRESULT Render() override;
	HRESULT Bind_Buffers();

protected:
	// ������ �� ���� ����.
	virtual HRESULT PrevRender();

	// ������ �� ���� ����.
	virtual HRESULT EndRender();

private:
	// ��ƼŬ �Ӽ� ����.
	void SetParticleAttribute();

protected:
	HRESULT Create_VertexBuffer();

	// ��ƼŬ ��輱 ����.
	void SetParticleBoundingBox(ParticleBoundingBox box);

	// float �Ű������� DWORD ���·� ��ȯ.
	DWORD GetScale(float f);

	float GetRandomFloat(float lowBound, float highBound);

	// Initialize ȣ�� �� ����.	
	// ��ƼŬ���� �� �Ӽ��� ���� ���ش�.
	virtual ParticleAttribute OnSetAddParticle() = 0;

	// ��ƼŬ ��輱 ����� ȣ��
	// ��, IsBounding�� true ���¿��� ��.
	virtual void OnBoundingExit(ParticleAttribute& particle) = 0;

public:
	virtual void Free() override;

private:
	// ��ƼŬ ���׸�Ʈ �� ����.
	DWORD dwVpOffset = 0;

	// ��ƼŬ ��輱.
	ParticleBoundingBox m_boundingBox;

protected:
	_uint	m_iFVF = {};
	LPDIRECT3DVERTEXBUFFER9 m_pVB = { nullptr };
	
	// ��ƼŬ ����.
	_uint iParticleCount = 0;

	// ��ƼŬ ���׸�Ʈ �� ä�� ����.
	DWORD dwVpBatchSize;	

	// ����Ʈ ��������Ʈ ũ��.
	DWORD dwPointSize;

	// ����Ʈ ��������Ʈ �ּ� ũ��.
	DWORD dwPointSizeMin;

	// ����Ʈ ��������Ʈ �ִ� ũ��.
	DWORD dwPointSizeMax;

	// ����Ʈ ��������Ʈ �Ÿ��� ũ�� A.
	DWORD dwPointScaleA;

	// ����Ʈ ��������Ʈ �Ÿ��� ũ�� B.
	DWORD dwPointScaleB;

	// ����Ʈ ��������Ʈ �Ÿ��� ũ�� C.
	DWORD dwPointScaleC;	

	// ��ƼŬ ��輱 Ȱ��ȭ.
	bool IsBounding = false;

	// ��ƼŬ �ؽ���.
	CTexture* m_pParticleTexture = { nullptr };
	
private:
	list<ParticleAttribute> m_ListParticleAttribute;
};

END;
