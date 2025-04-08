#include "SkeletalAnimator.h"
#include "VIBuffer_Cube.h"
#include "ViBuffer_Anim.h"
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

    for (int i = 0; i < 20; i++) {
        m_curFream[i] = 999;
    }

    return S_OK;
}

HRESULT CSkeletalAnimator::Update(_float fTimeDelta, _uint iCollsionGroup)
{


    return S_OK;
}

bool CSkeletalAnimator::is_AnimtionEND(int type)
{
    if (fElapsedTime[type] >= m_Animations[type].back().fTime)
    {
        fElapsedTime[type] = 0.0f;  // 처음부터 재생
        return true;
    }
    else
    {
        return false;
    }
}

const Matrix& CSkeletalAnimator::GetBoneWorldMatrix(int index)
{
    return vecBones[index].worldTransform;
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
    while (fYawDelta > PI) fYawDelta -= static_cast < _float>(2 * PI);
    while (fYawDelta < -PI) fYawDelta += static_cast<_float>(2 * PI);

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

// 월드 좌표를 받아서 해당 본의 월드만큼 로컬 매트릭스를 더해준다
void CSkeletalAnimator::LookAt(_float3 vPos, int BoneIndex)
{
    _float3 boneWorldPos = vecBones[BoneIndex].worldTransform.Get_State(Matrix::STATE_POSITION);
    _float3 lookDir = vPos - boneWorldPos;
    D3DXVec3Normalize(&lookDir, &lookDir);

    _float3 upDir = { 0.f, 1.f, 0.f };
    _float3 right = {};
    D3DXVec3Cross(&right, &upDir, &lookDir);
    D3DXVec3Normalize(&right, &right);

    _float3 up = {};
    D3DXVec3Cross(&up, &lookDir, &right);
    D3DXVec3Normalize(&up, &up);
    
    Matrix animLocal = vecBones[BoneIndex].localTransform; // 애니메이션이 적용된 기존 본 행렬
    Matrix parentWorld = vecBones[vecBones[BoneIndex].parent].worldTransform;    // 부모 월드 행렬
    Matrix animWorld = animLocal * parentWorld;     // 현재 애니메이션 기준 월드 행렬

    Matrix targetWorld = {};
    targetWorld.Set_State(Matrix::STATE_POSITION, animWorld.Get_State(Matrix::STATE_POSITION));
    targetWorld.Set_State(Matrix::STATE_LOOK, lookDir);
    targetWorld.Set_State(Matrix::STATE_RIGHT, right);
    targetWorld.Set_State(Matrix::STATE_UP, up);

    // 애니메이션 기준에서의 보정 회전 (로컬로 환산)
    Matrix invParentWorld = parentWorld.Invert();
    Matrix correctedLocal = targetWorld * invParentWorld;

    correctedLocal.Set_State(Matrix::STATE_POSITION, animLocal.Get_State(Matrix::STATE_POSITION));

    // 적용
    vecBones[BoneIndex].localTransform = correctedLocal;
}

void CSkeletalAnimator::LookAt_Anim(_float3 vTargetPos, int BoneIndex)
{
    // 1. 뼈의 월드 위치
    _float3 boneWorldPos = vecBones[BoneIndex].worldTransform.Get_State(Matrix::STATE_POSITION);

    // 2. LookAt 회전 계산
    _float3 lookDir = vTargetPos - boneWorldPos;
    if (D3DXVec3Length(&lookDir) < 0.001f) return;
    D3DXVec3Normalize(&lookDir, &lookDir);

    _float3 upDir = { 0.f, 1.f, 0.f };
    _float3 right = {};
    D3DXVec3Cross(&right, &upDir, &lookDir);
    D3DXVec3Normalize(&right, &right);

    _float3 up = {};
    D3DXVec3Cross(&up, &lookDir, &right);
    D3DXVec3Normalize(&up, &up);

    // 3. 회전만 포함된 보정 월드 행렬 (위치는 없음)
    Matrix correctionWorld;
    correctionWorld.Set_State(Matrix::STATE_RIGHT, right);
    correctionWorld.Set_State(Matrix::STATE_UP, up);
    correctionWorld.Set_State(Matrix::STATE_LOOK, lookDir);
    correctionWorld.Set_State(Matrix::STATE_POSITION, { 0.f, 0.f, 0.f }); // 위치 없음!

    // 4. 부모의 월드 행렬
    Matrix parentWorld = {};
    int parentIndex = vecBones[BoneIndex].parent;
    if (parentIndex >= 0)
        parentWorld = vecBones[parentIndex].worldTransform;

    // 5. 보정 회전을 로컬로 변환
    Matrix invParentWorld = parentWorld.Invert();
    Matrix correctionLocal = correctionWorld * invParentWorld;

    // 6. 기존 애니메이션 localTransform과 곱해서 최종 회전 적용
    Matrix animLocal = vecBones[BoneIndex].localTransform;

    // 위치는 애니메이션 위치 유지
    _float3 originalPos = animLocal.Get_State(Matrix::STATE_POSITION);

    Matrix finalLocal = correctionLocal * animLocal;
    finalLocal.Set_State(Matrix::STATE_POSITION, originalPos);

    // 7. 최종 적용
    vecBones[BoneIndex].localTransform = finalLocal;
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
        for (int i = 0; i < m_pVIBufferComs.size(); i++)
        {
            m_pVIBufferComs[i]->SetMatrix(vecBones[i + 1].Correction * vecBones[i + 1].worldTransform);
        }
    }  
}

HRESULT CSkeletalAnimator::Update_Animetion(_int _type, float fTimeDelta, int boneIndex)
{
    m_CurrentAnim = _type;
    fElapsedTime[_type] += fTimeDelta;

    if (m_Animations[_type].size() < 2)
    {
        // 단일 키프레임만 있을 경우 그냥 그거 하나 적용
        vecBones[boneIndex].localTransform = m_Animations[_type][0].matTransform * vecBones[boneIndex].baseTransform;
        return S_OK;
    }

    // 키프레임 찾기
    KEYFREAME key1{}, key2{};
    bool found = false;

    for (_uint i = 0; i < m_Animations[_type].size() - 1; ++i)
    {
        if (fElapsedTime[_type] >= m_Animations[_type][i].fTime && fElapsedTime[_type] <= m_Animations[_type][i + 1].fTime)
        {
            if (m_curFream[_type] != i)
            {
                m_curFream[_type] = i;
                CallBack_Fream(_type, m_curFream[_type]); // 콜백 함수
            }

            key1 = m_Animations[_type][i];
            key2 = m_Animations[_type][i + 1];
            found = true;
            break;
        }
    }

    if (!found) return S_OK;

    // 보간 비율 계산 (0~1 사이 값)
    float t = (fElapsedTime[_type] - key1.fTime) / (key2.fTime - key1.fTime);
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

void CSkeletalAnimator::Set_BoneLocalMatrix(int boneIndex, D3DMATRIX& mat)
{
    vecBones[boneIndex].localTransform = mat * vecBones[boneIndex].baseTransform;
}


void CSkeletalAnimator::DeBugBone(int BoneIndex)
{
    Matrix mat = vecBones[BoneIndex].worldTransform;

}

void CSkeletalAnimator::InitBone()
{
    for (auto& bone : vecBones)
    {
        bone.localTransform = bone.baseTransform;
    }
}

float CSkeletalAnimator::RotateRootByNeckDelta(int neckIndex, int rootIndex, float fTimeDelta)
{
    // 월드 기준으로 방향을 비교해야 정확
    _float3 neckLookDir = vecBones[neckIndex].worldTransform.Get_State(Matrix::STATE_LOOK);
    _float3 rootLookDir = vecBones[rootIndex].worldTransform.Get_State(Matrix::STATE_LOOK);

    float neckYaw = atan2f(neckLookDir.x, neckLookDir.z);
    float rootYaw = atan2f(rootLookDir.x, rootLookDir.z);

    float deltaYaw = neckYaw - rootYaw;

    // 정규화
    while (deltaYaw > PI) deltaYaw -= static_cast<_float>(2 * PI);
    while (deltaYaw < -PI) deltaYaw += static_cast<_float>(2 * PI);

    float influence = 0.3f; // 얼마나 따라올지
    float targetYaw = rootYaw + deltaYaw * influence;

    float followSpeed = 5.f; // rad/sec
    float newYaw = rootYaw + followSpeed * fTimeDelta * (targetYaw - rootYaw); 

    return newYaw - rootYaw; // 회전량(델타) 반환
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