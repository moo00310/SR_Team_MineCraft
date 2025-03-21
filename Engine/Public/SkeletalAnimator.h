#pragma once

#include "Component.h"

BEGIN(Engine)

class CVIBuffer_Cube;

class ENGINE_DLL CSkeletalAnimator final : public CComponent
{
public:
	typedef struct tagRigidbodyDesc
	{
		int iCount = {};
		CVIBuffer_Cube* Cubes{ nullptr };

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

public:
	HRESULT Update_Bone(int BoneIndex, const Matrix& Matrix);
	HRESULT Update_Animetion(_int _type, float fTimeDelta, int boneIndex);

private:
	vector<BONE> vecBones;
	CVIBuffer_Cube* m_pVIBufferComs = nullptr;
	map<_uint, vector<KEYFREAME>> m_Animations = {};
	float fElapsedTime = 0.0f;
	int m_imeshCount;

private:
	void Update_Mesh();

public:
	static CSkeletalAnimator* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END