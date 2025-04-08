#pragma once

#include "Component.h"

BEGIN(Engine)

class CVIBuffer_Cube;
class CVIBuffer_Anim;

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
#define AnimTimeCount 20

	typedef struct tagRigidbodyDesc
	{
		vector<CVIBuffer_Anim*> Cubes;

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
	int GetCurrentAnim() { return m_CurrentAnim; }
	bool is_AnimtionEND(int type);
	void Reset_fElapsedTime(int index, int targetIndex ) { fElapsedTime[index] = fElapsedTime[targetIndex]; }
	void Set_ZeroAnimTime() { for (int i = 0; i < AnimTimeCount; i++) { fElapsedTime[i] = 0.f; } }
	const Matrix& GetBoneWorldMatrix(int index);

public:
	void IkLookAt(float fTimeDelta, int boneIndex, int targetInex);
	void LookAt(_float3 vPos, int BoneIndex);

public:
	void Add_Bone(const BONE& bone);
	void Add_Animation(_int _type, const KEYFREAME& keyframe);

public:
	HRESULT Update_Animetion(_int _type, float fTimeDelta, int boneIndex);
	HRESULT Update_Bone(int BoneIndex, const Matrix& Matrix);
	HRESULT Update_RootBone(const Matrix& matrix);
	void Set_BoneLocalMatrix(int boneIndex, D3DMATRIX& mat);


public:
	void DeBugBone(int BoneIndex);
	void InitBone();
	float RotateRootByNeckDelta(int neckIndex, int rootIndex, float fTimeDelta);

private:
	vector<BONE> vecBones;
	vector<CVIBuffer_Anim*> m_pVIBufferComs;
	map<_uint, vector<KEYFREAME>> m_Animations = {};
	float fElapsedTime[AnimTimeCount] = {};
	float animElapsedTime = {};
	int m_imeshCount = 0;

	int m_CurrentAnim = { 0 };
	BlendState m_blendState = {};

private:
	int m_curFream[AnimTimeCount] = {0};
	bool m_isChangeFrame{ false };
private:
	void Update_Mesh();
	Matrix CalcCurrentMatrix(int animType, int boneIndex);

/// <summary>
/// 콜백함수
/// </summary>
public:
	using FrameCallback = std::function<void(int animType, int frame)>;

	void SetFrameCallback(FrameCallback callback) 
	{
		m_pFrameCallback = callback;
	}

	void CallBack_Fream(int animType, int frame) 
	{
		if (m_pFrameCallback) {
			m_pFrameCallback(animType, frame);
		}
	}

private:
	FrameCallback m_pFrameCallback = nullptr;

public:
	static CSkeletalAnimator* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END