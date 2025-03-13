#include "Cube.h"
#include "GameInstance.h"

CCube::CCube(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CCube::CCube(const CCube& Prototype)
    : CGameObject(Prototype)
{
}

HRESULT CCube::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCube::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CCube::Priority_Update(_float fTimeDelta)
{
}

void CCube::Update(_float fTimeDelta)
{
}

void CCube::Late_Update(_float fTimeDelta)
{
}

HRESULT CCube::Render()
{

    return S_OK;
}

HRESULT CCube::Ready_Components()
{

    return S_OK;
}


CCube* CCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCube* pInstance = new CCube(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CCube");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCube::Clone(void* pArg)
{
    CCube* pInstance = new CCube(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CCube");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCube::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);

}
