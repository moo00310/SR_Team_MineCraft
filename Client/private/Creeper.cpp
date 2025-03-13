#include "Creeper.h"

#include "Transform.h"
#include "GameInstance.h"
#include "Creeper_Head.h"
#include "Creeper_Body.h"
#include "Creeper_Foot.h"

CCreeper::CCreeper(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CCreeper::CCreeper(const CCreeper& Prototype)
    : CGameObject(Prototype)
{
}

HRESULT CCreeper::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCreeper::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CCreeper::Priority_Update(_float fTimeDelta)
{
}

void CCreeper::Update(_float fTimeDelta)
{
    //_float3 a = { 1.f, 1.f, 0.f };
    //dynamic_cast<CCreeper_Head*>(m_pGameInstance->Get_Object(LEVEL_GAMEPLAY, TEXT("Creeper_Head"), 0))->TRcom()->Turn(a, fTimeDelta);
}

void CCreeper::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this)))
        return;
}

HRESULT CCreeper::Render()
{
  
    return S_OK;
}

HRESULT CCreeper::Ready_Components()
{
    /* For.Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

   // Model
  if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_MOO, TEXT("Prototype_GameObject_Creeper_Head"),
      LEVEL_MOO, TEXT("Creeper_Head"))))
      return E_FAIL;

   if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_MOO, TEXT("Prototype_GameObject_Creeper_Body"),
       LEVEL_MOO, TEXT("Creeper_Body"))))
       return E_FAIL;
   
   if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_MOO, TEXT("Prototype_GameObject_Creeper_Foot"),
       LEVEL_MOO, TEXT("Creeper_Foot_1"))))
       return E_FAIL;
   if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_MOO, TEXT("Prototype_GameObject_Creeper_Foot"),
       LEVEL_MOO, TEXT("Creeper_Foot_2"))))
       return E_FAIL;
     if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_MOO, TEXT("Prototype_GameObject_Creeper_Foot"),
         LEVEL_MOO, TEXT("Creeper_Foot_3"))))
       return E_FAIL;
    if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_MOO, TEXT("Prototype_GameObject_Creeper_Foot"),
        LEVEL_MOO, TEXT("Creeper_Foot_4"))))
      return E_FAIL;
   

    if (FAILED(Ready_Bone())) 
        return E_FAIL;
   

    return S_OK;
}

HRESULT CCreeper::Ready_Bone()
{
   _float3 temp = { 0.f, 22.f / 24.f, 0.f };
    dynamic_cast<CCreeper_Head*>(m_pGameInstance->Get_Object(LEVEL_MOO, TEXT("Creeper_Head"), 0))->SetPos(temp);
   
   temp = { 0.f, 12.f / 24.f, 0.f };
    static_cast<CCreeper_Body*>(m_pGameInstance->Get_Object(LEVEL_MOO, TEXT("Creeper_Body"), 0))->SetPos(temp);
   
   temp = { 2.f / 24.f, 3.f / 24.f,  -4.f / 24.f };
   static_cast<CCreeper_Foot*>(m_pGameInstance->Get_Object(LEVEL_MOO, TEXT("Creeper_Foot_1"), 0))->SetPos(temp);
   
   temp = { -2.f / 24.f, 3.f / 24.f, -4.f / 24.f };
   static_cast<CCreeper_Foot*>(m_pGameInstance->Get_Object(LEVEL_MOO, TEXT("Creeper_Foot_2"), 0))->SetPos(temp);
   
   temp = { 2.f / 24.f, 3.f / 24.f, 4.f / 24.f };
   static_cast<CCreeper_Foot*>(m_pGameInstance->Get_Object(LEVEL_MOO, TEXT("Creeper_Foot_3"), 0))->SetPos(temp);
   
   temp = { -2.f / 24.f, 3.f / 24.f, 4.f / 24.f };
   static_cast<CCreeper_Foot*>(m_pGameInstance->Get_Object(LEVEL_MOO, TEXT("Creeper_Foot_4"), 0))->SetPos(temp);
    return S_OK;
}

CCreeper* CCreeper::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCreeper* pInstance = new CCreeper(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CCreeper");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCreeper::Clone(void* pArg)
{
    CCreeper* pInstance = new CCreeper(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CCreeper");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCreeper::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);

}
