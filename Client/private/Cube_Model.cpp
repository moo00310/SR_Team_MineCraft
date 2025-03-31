#include "Cube_Model.h"

CCube_Model::CCube_Model(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CRightHand_Object{ pGraphic_Device }
{
}

CCube_Model::CCube_Model(const CCube_Model& Prototype)
    : CRightHand_Object( Prototype )
{
}

HRESULT CCube_Model::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCube_Model::Initialize(void* pArg)
{
    return S_OK;
}

void CCube_Model::Priority_Update(_float fTimeDelta)
{
}

void CCube_Model::Update(_float fTimeDelta)
{
    KeyInput();
    __super::Update(fTimeDelta);
}

void CCube_Model::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

}

HRESULT CCube_Model::Render()
{
    if (FAILED(m_pTextureCom->Bind_Resource(0)))
        return E_FAIL;

    __super::Render();

    return S_OK;
}

HRESULT CCube_Model::Ready_Components()
{

    return S_OK;
}

HRESULT CCube_Model::Ready_Bone()
{
    return S_OK;
}

HRESULT CCube_Model::Ready_Animation()
{
    return S_OK;
}

void CCube_Model::Motion_Idle(_float fTimeDelta)
{
}

void CCube_Model::Motion_Swing(_float fTimeDelta)
{
}

void CCube_Model::Motion_Walk(_float fTimeDelta)
{
}

void CCube_Model::KeyInput()
{
}

CCube_Model* CCube_Model::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCube_Model* pInstance = new CCube_Model(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CCube_Model");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCube_Model::Clone(void* pArg)
{
    CCube_Model* pInstance = new CCube_Model(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CCube_Model");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCube_Model::Free()
{
    __super::Free();
}
