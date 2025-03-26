#include "SkeletalAnimator.h"
#include "VIBuffer_Cube.h"
#include "Transform.h"
#include <iostream>

CSkeletalAnimator::CSkeletalAnimator(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CComponent{ pGraphic_Device }
{
}

CSkeletalAnimator::CSkeletalAnimator(const CSkeletalAnimator& Prototype)
    : CComponent(Prototype)
{
}

HRESULT CSkeletalAnimator::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSkeletalAnimator::Initialize(void* pArg)
{
    DESC Desc = { *static_cast<DESC*>(pArg) };

    for (int i = 0; i < Desc.Cubes.size(); ++i)
    {
        m_pVIBufferComs.push_back(Desc.Cubes[i]);
        if (m_pVIBufferComs[i])
            m_pVIBufferComs[i]->AddRef();
    }

    return S_OK;
}

HRESULT CSkeletalAnimator::Update(_float fTimeDelta, _uint iCollsionGroup)
{
    return S_OK;
}

bool CSkeletalAnimator::is_AnimtionEND()
{
    if (fElapsedTime >= m_Animations[m_CurrentAnim].back().fTime)
    {
        fElapsedTime = 0.0f;  // 처음부터 재생
        return true;
    }
    else
    {
        return false;
    }
}

void CSkeletalAnimator::IkLookAt(float fTimeDelta, int boneIndex, int targetInex)
{
    _float3 vLook = vecBones[targetInex].localTransform.Get_State(Matrix::STATE_LOOK);
    float fCamYaw = atan2f(vLook.x, vLook.z);

    _float3 vSpineLook = vecBones[boneIndex].localTransform.Get_State(Matrix::STATE_LOOK);
    float fSpineYaw = atan2f(vSpineLook.x, vSpineLook.z);

    // yaw 차이 비교
    float fYawDelta = fCamYaw - fSpineYaw;

    // -PI ~ +PI 사이로 정규화
    while (fYawDelta > PI) fYawDelta -= 2 * PI;
    while (fYawDelta < -PI) fYawDelta += 2 * PI;

    float fThreshold = D3DXToRadian(25.f); 

    float fFollowSpeed = 5.f; // 회전 속도
    float fTargetYaw = fSpineYaw + (fYawDelta - Sign(fYawDelta) * fThreshold); // 따라갈 각도

    // 보간
    float fNewYaw = fSpineYaw + fFollowSpeed * fTimeDelta * (fTargetYaw - fSpineYaw);

    // Spine 회전 행렬 갱신
    Matrix matNewSpine;
    D3DXMatrixRotationY(&matNewSpine, fNewYaw);

    // 타겟 보정 행렬
    Matrix matNewTarget;
    D3DXMatrixRotationY(&matNewTarget, -fNewYaw);

    vecBones[targetInex].localTransform = vecBones[targetInex].localTransform * matNewTarget;
    vecBones[boneIndex].localTransform = matNewSpine * vecBones[boneIndex].baseTransform;
}

void CSkeletalAnimator::Add_Bone(const BONE& bone)
{
    vecBones.push_back(bone);
}

void CSkeletalAnimator::Add_Animation(_int _type, const KEYFREAME& keyframe)
{
    m_Animations[_type].push_back(keyframe);
}

HRESULT CSkeletalAnimator::Update_Bone(int boneIndex, const Matrix& parentTransform)
{
    // 본의 최종 월드 행렬 = 현재 본의 로컬 행렬 × 부모의 월드 행렬
    vecBones[boneIndex].worldTransform = vecBones[boneIndex].localTransform * parentTransform;

    // 모든 자식 bone에 대해 재귀적으로 업데이트
    for (int i = 0; i < vecBones.size(); i++)
    {
        if (vecBones[i].parent == boneIndex)
        {
            Update_Bone(i, vecBones[boneIndex].worldTransform);
        }
    }

    Update_Mesh();

    return S_OK;
}

void CSkeletalAnimator::Update_Mesh()
{
    // 최종적으로 본의 위치와 매시의 중심을 보정 해준다.
    if (m_pVIBufferComs.size() == 1)
    {
		m_pVIBufferComs[0]->SetMatrix(vecBones[0].Correction * vecBones[0].worldTransform);
    }
    else
    {
        for (int i = 0; i < 6; i++)
        {
            m_pVIBufferComs[i]->SetMatrix(vecBones[i + 1].Correction * vecBones[i + 1].worldTransform);
        }
    }  
}

void CSkeletalAnimator::Blend_Animations(float fTimeDelta, int boneIndex)
{
    m_blendState.currentTime += fTimeDelta;

    float blendT = m_blendState.currentTime / m_blendState.blendDuration;
    if (blendT > 1.f)  blendT = 1.f;
    else if (blendT < 0.f)  blendT = 0.f;

    // 현재 애니메이션 결과
    Matrix fromMat = CalcCurrentMatrix(m_blendState.fromAnim, boneIndex);

    // 다음 애니메이션 첫 프레임 또는 현재 시간 보간
    Matrix toMat = CalcCurrentMatrix(m_blendState.toAnim, boneIndex);

    // 보간해서 최종 행렬 만들기
    Matrix blended = InterpolateMatrix_Quat(fromMat, toMat, blendT);
    vecBones[boneIndex].localTransform = blended * vecBones[boneIndex].baseTransform;

    if (blendT >= 1.f)
    {
        // 블렌딩 완료 → 다음 애니메이션으로 전환
        m_blendState.isBlending = false;
        m_CurrentAnim = m_blendState.toAnim;
        fElapsedTime = 0.f;
    }
}

HRESULT CSkeletalAnimator::Update_Animetion(_int _type, float fTimeDelta, int boneIndex)
{
    if (m_blendState.isBlending)
    {
        Blend_Animations(fTimeDelta, boneIndex);
		return S_OK;
    }

    m_CurrentAnim = _type;
    fElapsedTime += fTimeDelta;

    if (m_Animations[_type].size() < 2)
    {
        // 단일 키프레임만 있을 경우 그냥 그거 하나 적용
        vecBones[boneIndex].localTransform = m_Animations[_type][0].matTransform * vecBones[boneIndex].baseTransform;
        return S_OK;
    }

    // 키프레임 찾기
    KEYFREAME key1, key2;
    for (size_t i = 0; i < m_Animations[_type].size() - 1; ++i)
    {
        if (fElapsedTime >= m_Animations[_type][i].fTime && fElapsedTime <= m_Animations[_type][i + 1].fTime)
        {
            key1 = m_Animations[_type][i];
            key2 = m_Animations[_type][i + 1];
            break;
        }
    }

    // 보간 비율 계산 (0~1 사이 값)
    float t = (fElapsedTime - key1.fTime) / (key2.fTime - key1.fTime);
    cout << t << " " << key2.fTime << " " << key1.fTime << endl;
    Matrix interpolatedMatrix = InterpolateMatrix_Quat(key1.matTransform , key2.matTransform, t);
    vecBones[boneIndex].localTransform = interpolatedMatrix * vecBones[boneIndex].baseTransform;

    return S_OK;
}

HRESULT CSkeletalAnimator::Update_RootBone(const Matrix& matrix)
{
    if (FAILED(Update_Bone(0, matrix)))
        return E_FAIL;
 
    return S_OK;
}

void CSkeletalAnimator::Start_Blend(int fromAnim, int toAnim, float duration)
{
    m_blendState.isBlending = true;
    m_blendState.fromAnim = fromAnim;
    m_blendState.toAnim = toAnim; 
    m_blendState.blendDuration = duration;
    m_blendState.currentTime = 0.f;
}

void CSkeletalAnimator::Set_BoneLocalMatrix(int boneIndex, D3DMATRIX& mat)
{
    vecBones[boneIndex].localTransform = mat * vecBones[boneIndex].baseTransform;
}

void CSkeletalAnimator::DeBugBone(int BoneIndex)
{
    Matrix mat = vecBones[BoneIndex].worldTransform;

    cout << mat._41 << " " << mat._42 << " " << mat._43 << endl;
}


CSkeletalAnimator* CSkeletalAnimator::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CSkeletalAnimator* pInstance = new CSkeletalAnimator(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CSkeletalAnimator");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CSkeletalAnimator::Clone(void* pArg)
{
    CSkeletalAnimator* pInstance = new CSkeletalAnimator(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CSkeletalAnimator");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSkeletalAnimator::Free()
{
    __super::Free();

    for (auto& buffer : m_pVIBufferComs)
        Safe_Release(buffer);

    m_pVIBufferComs.clear();

}


Matrix CSkeletalAnimator::CalcCurrentMatrix(int animType, int boneIndex)
{
    const auto& anim = m_Animations[animType];

    // 예외 처리
    if (anim.size() == 0)
        return Matrix(); // 빈 애니메이션이면 단위 행렬

    if (anim.size() == 1)
        return anim[0].matTransform; // 키 하나면 그대로 리턴

    // 루프 처리
    if (animElapsedTime >= anim.back().fTime)
        animElapsedTime = 0.f;

    // 보간 키 찾기
    KEYFREAME key1, key2;
    for (size_t i = 0; i < anim.size() - 1; ++i)
    {
        if (animElapsedTime >= anim[i].fTime && animElapsedTime <= anim[i + 1].fTime)
        {
            key1 = anim[i];
            key2 = anim[i + 1];
            break;
        }
    }

    float t = (animElapsedTime - key1.fTime) / (key2.fTime - key1.fTime);
    return InterpolateMatrix(key1.matTransform, key2.matTransform, t);
}