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
}

void CDirt::Update(_float fTimeDelta)
{
}

void CDirt::Late_Update(_float fTimeDelta)
{
    if (m_bRenderActive) {
        if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
            return;
    }

	__super::Late_Update(fTimeDelta);
}

HRESULT CDirt::Render()
{
    if (FAILED(m_pTextureCom->Bind_Resource(0)))
        return E_FAIL;

    if (FAILED(m_pTransformCom->Bind_Resource()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    m_pTransformCom->Bind_Resource(m_pShaderCom);
    m_pTextureCom->Bind_Resource(m_pShaderCom, "g_Texture", 1);

    m_pShaderCom->Begin(0);

    /* ������ �׸���. */
    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    __super::Render();

    m_pShaderCom->End();

    return S_OK;
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
    CParticleSystem* particle = (CParticleSystem*)m_pGameInstance->Push(LEVEL_STATIC,	// ���� ��.
        PROTOTYPE_GAMEOBJECT_PARTICLE_SAND_DESTROY,	// ������ ������Ÿ��.
        LEVEL_STATIC,	// ������ ��.
        LAYER_PARTICLE_SAND_DESTROY);	// �ֵ������Ʈ�� �߰��� ���̾�

    if (particle != nullptr)
    {        
        particle->Replay(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
        //m_pGameInstance->Pop(particle);
    }

    __super::Free();
}
