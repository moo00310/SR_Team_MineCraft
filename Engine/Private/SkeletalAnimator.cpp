#include "SkeletalAnimator.h"
#include "VIBuffer_Cube.h"

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
    m_imeshCount = Desc.iCount;
#pragma push_macro("new")
#undef new
    void* rawMemory = operator new(sizeof(CVIBuffer_Cube) * Desc.iCount);
    CVIBuffer_Cube* m_pVIBufferComs = static_cast<CVIBuffer_Cube*>(rawMemory);

    if (m_pVIBufferComs == nullptr) return E_FAIL;

    memcpy(m_pVIBufferComs, Desc.Cubes, Desc.iCount * sizeof(CVIBuffer_Cube));
#pragma pop_macro("new")


    return S_OK;
}

HRESULT CSkeletalAnimator::Update(_float fTimeDelta, _uint iCollsionGroup)
{
    return S_OK;
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
    // ���� bone�� ��ȯ�� �θ� ��ȯ�� ���ϱ�
    vecBones[boneIndex].worldTransform = vecBones[boneIndex].localTransform * parentTransform;

    // ��� �ڽ� bone�� ���� ��������� ������Ʈ
    for (int i = 0; i < 7; i++)
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
    for (int i = 0; i < 6; i++)
    {
        m_pVIBufferComs[i].SetMatrix(vecBones[i + 1].Correction * vecBones[i + 1].worldTransform);
    }
}

HRESULT CSkeletalAnimator::Update_Animetion(_int _type, float fTimeDelta, int boneIndex)
{
    if (fElapsedTime >= m_Animations[_type].back().fTime)
    {
        fElapsedTime = 0.0f;  // ó������ ���
    }

    // Ű������ ã��
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

    // ���� ���� ��� (0~1 ���� ��)
    float t = (fElapsedTime - key1.fTime) / (key2.fTime - key1.fTime);
    // ��� ����
    D3DXMATRIX interpolatedMatrix = InterpolateMatrix(key1.matTransform, key2.matTransform, t);

    Update_Bone(boneIndex,interpolatedMatrix);

    return S_OK;
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
    // �̰� ����ؾ���
    __super::Free();
    for (int i = 0; i < m_imeshCount; i++)
    {
        m_pVIBufferComs[i].Release();
    }
}
