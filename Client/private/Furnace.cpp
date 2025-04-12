#include "Furnace.h"
#include "GameInstance.h"
#include "UI_Mgr.h"

CFurnace::CFurnace(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CBreakableCube(pGraphic_Device)
{
}

CFurnace::CFurnace(const CFurnace& Prototype)
    : CBreakableCube(Prototype)
{
}

HRESULT CFurnace::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CFurnace::Initialize(void* pArg)
{

    if (FAILED(Ready_Components()))
        return E_FAIL;

    __super::Initialize(pArg);

    m_fHardness = 1.5;

    m_itemName = ITEMNAME_FURANCE;

    return S_OK;
}

void CFurnace::Priority_Update(_float fTimeDelta)
{
    vector<CSlotInfo*> _vecSlotList = CUI_Mgr::Get_Instance()->Get_vecSlotInfoOriginlist();
    if (m_bFurnaceBurn) {
        m_iCoalTime--;

        if (_vecSlotList[56]->Get_ItemName() == ITEMNAME_RAWIRON) 
            m_iIronTime--;

        // 석탄이 없으면 burn false
        if (m_iCoalTime <= 0) {
            m_bFurnaceBurn = false;
        }

        // 철이 다 구워지면 
        if (m_iIronTime <= 0) {
            m_iIronTime = 1000;
            if (_vecSlotList[56]->Get_ItemName() == ITEMNAME_RAWIRON) {
                _vecSlotList[56]->Set_ItemCountAdd(-1);
                
                // 철이 하나도 안구워져 있으면
                if (_vecSlotList[58]->Get_ItemName() == ITEMNAME_END) {
                    _vecSlotList[58]->Set_ItemName(ITEMNAME_IRON);
                    _vecSlotList[58]->Set_ItemCount(1);
                    _vecSlotList[58]->Set_ItemCountRender(true);
                }
                else {
                    _vecSlotList[58]->Set_ItemCountAdd(1);
                }
                
            }
        }

        if (m_bParticle == false)
        {
            _float3 posFire = m_vecPositions[0];
            posFire.y -= 0.4f;
            posFire.z += 0.55f;

            _float3 posCloud = m_vecPositions[0];
            posCloud.y -= 0.3f;
            posCloud.z += 0.55f;

            CParticleEventManager::Get_Instance()->OnParticle(
                PROTOTYPE_GAMEOBJECT_PARTICLE_FURNACE,
                posFire
            );

            CParticleEventManager::Get_Instance()->OnParticle(
                PROTOTYPE_GAMEOBJECT_PARTICLE_FURNACE_CLOUD,
                posCloud
            );

            m_bParticle = true;
        }        
    }

    if (!m_bFurnaceBurn) {
        if ((_vecSlotList[56]->Get_ItemName() == ITEMNAME_RAWIRON) && (_vecSlotList[57]->Get_ItemName() == ITEMNAME_COAL)) {
            _vecSlotList[57]->Set_ItemCountAdd(-1);
            m_bFurnaceBurn = true;
            m_iIronTime = 1000;
            m_iCoalTime = 8000;
        }

        m_bParticle = false;
    }

    __super::Priority_Update(fTimeDelta);
}

void CFurnace::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CFurnace::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
        return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CFurnace::Render()
{
    if (m_bFurnaceBurn) {
        if (FAILED(m_pTextureCom2->Bind_Resource(0)))
            return E_FAIL;
        m_pTextureCom2->Bind_Resource(m_pShaderCom, "g_Texture", 1);
    }
    else {
        if (FAILED(m_pTextureCom->Bind_Resource(0)))
            return E_FAIL;
        m_pTextureCom->Bind_Resource(m_pShaderCom, "g_Texture", 1);
    }


    if (FAILED(m_pTransformCom->Bind_Resource()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    m_pTransformCom->Bind_Resource(m_pShaderCom);

    m_pShaderCom->Begin(0);

    /* 정점을 그린다. */
    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    m_pShaderCom->End();

    return S_OK;
}

HRESULT CFurnace::Drop_Item_OnDestroy(const _float3& vPos)
{
    wchar_t layerName[100];
    swprintf(layerName, 100, L"Layer_Chunk%d", m_iMyChunk);

    if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemCube"), LEVEL_YU, layerName)))
        return E_FAIL;

    if (CItemCube* _copy = dynamic_cast<CItemCube*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))) {
        _copy->SetPos(vPos);
        _copy->Set_Bright(vPos.y);
        _copy->Set_ItemTypeAndBindTexture(ITEMNAME_FURANCE);
    }

    //m_pGameInstance->Play_Sound(TEXT("Stone_dig2"), SOUND_BLOCK_DIG, this, 1.f, vPos);


    return S_OK;
}

HRESULT CFurnace::Play_Destroy_Effect(const _float3& fPos)
{
    CParticleEventManager::Get_Instance()->OnParticle(
        PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_DESTROY,
        fPos
    );

    return S_OK;
}



HRESULT CFurnace::Ready_Components()
{
    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_FurnaceOff"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_FurnaceOn"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom2))))
        return E_FAIL;

    __super::Ready_Components();

    return S_OK;
}


CFurnace* CFurnace::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CFurnace* pInstance = new CFurnace(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CFurnace");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CFurnace::Clone(void* pArg)
{
    CFurnace* pInstance = new CFurnace(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CFurnace");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CFurnace::Free()
{
    __super::Free();
    Safe_Release(m_pTextureCom2);
}
