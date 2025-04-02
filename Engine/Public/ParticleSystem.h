#pragma once

#include "GameInstance.h"
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
	virtual void Priority_Update(_float fTimeDelta) override;	
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;

	virtual HRESULT Render() override;
	HRESULT Bind_Buffers();

	// ��ƼŬ �Ӽ��� �ʱ�ȭ.
	void Replay(_float3 _position);

	CTransform* GetTransform() const;

protected:
	// ������ �� ���� ����.
	virtual HRESULT PrevRender();

	// ������ �� ���� ����.
	virtual HRESULT EndRender();

	virtual void OnReplay(ParticleAttribute& particle) {};

	virtual HRESULT Ready_Components();

protected:
	HRESULT Create_VertexBuffer();

private:
	// ��ƼŬ �Ӽ� ����.
	void SetParticleAttribute();	

protected:
	// ��ƼŬ ��輱 ����.
	void SetParticleBoundingBox(ParticleBoundingBox box);

	// float �Ű������� DWORD ���·� ��ȯ.
	DWORD GetScale(float f);

	float GetRandomFloat(float lowBound, float highBound);

	// SetParticleAttribute ȣ�� �� ����.	
	// ��ƼŬ���� �� �Ӽ��� ���� ���ش�.
	virtual ParticleAttribute OnSetAddParticle() = 0;

	// ��ƼŬ ��輱 ����� ȣ��
	// ��, IsBounding�� true ���¿��� ��.
	virtual void OnBoundingExit(ParticleAttribute& particle) = 0;

	// rgb������ dword�� ��ȯ.
	DWORD Float3ToHex(_float3 color);

public:
	virtual void Free() override;

	// Ǯ������ Push�� ȣ��.
	virtual void OnPushPool() override;

private:
	// ��ƼŬ ���׸�Ʈ �� ����.
	DWORD dwVpOffset = 0;

	// ��ƼŬ ��輱.
	ParticleBoundingBox m_boundingBox;

	// �ؽ��� �ִϸ��̼� ���� ������ �ð�.
	float fCurrentAnimationFrame;	

	// ��ƼŬ ���� �ð�.
	float m_fCurrentTimer = { 0.f };

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
	// ���� �������� ī�޶�� �־����� ��ƼŬ�� ũ�� ����.
	DWORD dwPointScaleA;

	// ����Ʈ ��������Ʈ �Ÿ��� ũ�� B.
	// ���� �������� ī�޶�� �־����� ��ƼŬ�� �۾ƺ��̰� ��.
	DWORD dwPointScaleB;

	// ����Ʈ ��������Ʈ �Ÿ��� ũ�� C.
	// ���� �������� ���� �Ÿ��� ���� ��ƼŬ ũ�Ⱑ ������.
	DWORD dwPointScaleC;	

	// ��ƼŬ ��輱 Ȱ��ȭ.
	bool IsBounding = false;

	// �ؽ��� �ִϸ��̼� Ȱ��ȭ.
	bool IsTextureAnimation = false;

	// �ؽ��� �ִϸ��̼� ������ �ð�.
	float fAnimationFrame;

	// ��ƼŬ ��ü Ÿ�̸� Ȱ��ȭ.
	bool IsTimer = false;	

	// ��ƼŬ ���� �ð�.
	float fEndTimer = { 0.f };

	// �ؽ��� ������ �ε���.
	_uint iTextureIndex;

	// �ؽ��� ���� �ε���.
	_uint iCurrentTextureIndex;

	// ��ƼŬ ���� �ؽ��ĵ��� ���� �ٸ��� ���� ��ų ���ΰ�?.
	// �� ����� Ȱ��ȭ ��ų��� �ش� ��ƼŬ��
	// �ν��Ͻ� ����� ������ �ȵǴ�
	// �ʹ� ���� ��ƼŬ ���ڿ��� �������� �� ��.
	bool IsParticleTexture = {false};

	// ��ƼŬ �ؽ���.
	CTexture* m_pParticleTexture = { nullptr };

	// ��ƼŬ Ʈ������.
	CTransform* m_pTransform = { nullptr };

	// ��ƼŬ ���̴�.
	CShader* m_pShader = {nullptr};
	
private:
	list<ParticleAttribute> m_ListParticleAttribute;
};

END;
