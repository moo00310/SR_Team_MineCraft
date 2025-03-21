#pragma once

#include "Component.h"

BEGIN(Engine)

class CVIBuffer_Cube;

struct BlendState
{
	bool isBlending = false;
	int fromAnim = -1;
	int toAnim = -1;
	float blendDuration = 0.3f;   // 전환 시간
	float currentTime = 0.f;
};

class ENGINE_DLL CSkeletalAnimator final : public CComponent
{
public:
	typedef struct tagRigidbodyDesc
	{
		vector<CVIBuffer_Cube*> Cubes;

	}DESC;

private:
	CSkeletalAnimator(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSkeletalAnimator(const CSkeletalAnimator& Prototype);
	virtual ~CSkeletalAnimator() = default;
public:
	HRESULT Initialize_Prototype() override;
	HRESULT Initialize(void* pArg) override;
	HRESULT Update(_float fTimeDelta, _uint iCollsionGroup);

public:
	void Add_Bone(const BONE& bone);
	void Add_Animation(_int _type, const KEYFREAME& keyframe);
	bool IsBlending() { return m_blendState.isBlending; }
	int GetCurrentAnim() { return m_CurrentAnim; }

public:
	HRESULT Update_Bone(int BoneIndex, const Matrix& Matrix);
	HRESULT Update_Animetion(_int _type, float fTimeDelta, int boneIndex, const Matrix& pTransform);
	void Start_Blend(int fromAnim, int toAnim, float duration);

private:
	vector<BONE> vecBones;
	vector<CVIBuffer_Cube*> m_pVIBufferComs;
	map<_uint, vector<KEYFREAME>> m_Animations = {};
	float fElapsedTime = 0.0f;
	float animElapsedTime = 0.0f;
	int m_imeshCount = 0;


	int m_CurrentAnim = { 0 };
	BlendState m_blendState = {};

private:
	void Update_Mesh();
	Matrix CalcCurrentMatrix(int animType, int boneIndex);

public:
	static CSkeletalAnimator* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END