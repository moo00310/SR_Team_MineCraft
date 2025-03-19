#include "Sword.h"

CSword::CSword(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CItem_Model{ pGraphic_Device }
{
}

CSword::CSword(const CSword& Prototype)
    : CItem_Model(Prototype)
{
}

HRESULT CSword::Initialize_Prototype()
{ 
  
    return S_OK;
}

HRESULT CSword::Initialize(void* pArg)
{
    // 텍스쳐는 객체마다 달라지기 때문에 찾을 택스쳐 태그를
   // 부모 매개변수에 저장해야함
    m_TextureTag = TEXT("Prototype_Component_Texture_Pickaxe");
    m_TextureLevel = LEVEL_MOO;
    __super::Initialize(pArg);

    return S_OK;
}

void CSword::Priority_Update(_float fTimeDelta)
{
}

void CSword::Update(_float fTimeDelta)
{
    // 휘두르는 모션
    __super::Update(fTimeDelta);

}

void CSword::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this)))
        return;
}

HRESULT CSword::Render()
{
    __super::Render();
    return S_OK;
}

CSword* CSword::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CSword* pInstance = new CSword(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CSword");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSword::Clone(void* pArg)
{
    CSword* pInstance = new CSword(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CSword");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSword::Free()
{
    __super::Free();
}
