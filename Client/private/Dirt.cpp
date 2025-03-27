#include "Dirt.h"
#include "GameInstance.h"

CDirt::CDirt(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CBreakableCube(pGraphic_Device)
{
}

CDirt::CDirt(const CDirt& Prototype)
    : CBreakableCube(Prototype)
{
}

HRESULT CDirt::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CDirt::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CDirt::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CDirt::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CDirt::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
        return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CDirt::Render()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);

    if (FAILED(m_pTextureCom->Bind_Resource(0)))
        return E_FAIL;

    if (FAILED(m_pTransformCom->Bind_Resource()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    m_pTransformCom->Bind_Resource(m_pShaderCom);
    m_pTextureCom->Bind_Resource(m_pShaderCom, "g_Texture", 1);

    m_pShaderCom->Begin(0);

    /* 정점을 그린다. */
    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    __super::Render();

    m_pShaderCom->End();
    return S_OK;
}

HRESULT CDirt::Delete_Cube(_float3 fPos)
{
    for (size_t i = 0; i < m_vecPositions.size(); ++i)
    {
        if (m_vecPositions[i].x == fPos.x &&
            m_vecPositions[i].y == fPos.y &&
            m_vecPositions[i].z == fPos.z)
        {
            if (FAILED(Delete_Component(TEXT("Com_Collider_Cube"), m_Colliders[i])))
                return E_FAIL;

            // 2. 벡터에서 해당 위치 제거
            m_vecPositions.erase(m_vecPositions.begin() + i);

            // 3. 콜라이더 제거
            Safe_Release(m_Colliders[i]);
            m_Colliders.erase(m_Colliders.begin() + i);

            // 4. 인스턴스 버퍼 업데이트
            m_pVIBufferCom->Update_InstanceBuffer(m_vecPositions);

            wchar_t layerName[100];
            swprintf(layerName, 100, L"Layer_Chunk%d", m_iMyChunk);
            if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemCube"), LEVEL_YU, layerName)))
                return E_FAIL;
            dynamic_cast<CTransform*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName)->Find_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
            dynamic_cast<CItemCube*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))->Set_ItemTypeAndBindTexture(ITEM_DIRT);

            CParticleSystem* particle = (CParticleSystem*)m_pGameInstance->PushPool(LEVEL_STATIC,
                PROTOTYPE_GAMEOBJECT_PARTICLE_SAND_DESTROY,
                LEVEL_YU,
                LAYER_PARTICLE);


            if (particle != nullptr)
            {
                particle->Replay(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
                //m_pGameInstance->Pop(particle);
            }

            return S_OK;
        }
    }

    return E_FAIL;
}

HRESULT CDirt::Ready_Components()
{
    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Dirt"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

	__super::Ready_Components();

    return S_OK;
}


CDirt* CDirt::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CDirt* pInstance = new CDirt(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CDirt");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CDirt::Clone(void* pArg)
{
    CDirt* pInstance = new CDirt(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CDirt");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CDirt::Free()
{

    __super::Free();
}
