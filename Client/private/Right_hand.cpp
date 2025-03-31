#include "Right_hand.h"
#include "UI_Mgr.h"

CRight_hand::CRight_hand(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device  }
{
}

CRight_hand::CRight_hand(const CRight_hand& Prototype)
    : CGameObject(Prototype)
{
}

HRESULT CRight_hand::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CRight_hand::Initialize(void* pArg)
{
    m_pSteve = m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Steve"));
    m_pArm_Model = m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_TPS_Arm"));
    m_pRect_Model = m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Rect_Model"));
    
    if (m_pArm_Model == nullptr &&
        m_pRect_Model == nullptr)
        return E_FAIL;

    Safe_AddRef(m_pArm_Model);
    Safe_AddRef(m_pRect_Model);


    return S_OK;
}

void CRight_hand::Priority_Update(_float fTimeDelta)
{
    if (m_pGameInstance->Key_Down(VK_F5))
    {
        if (isTPS == false)
        {
            m_pSteve->SetRender(true);
            m_pArm_Model->SetActive(false);
            m_pRect_Model->SetActive(false);
            isTPS = true;
        }
        else
        { 
            m_pSteve->SetRender(false);
            m_pArm_Model->SetActive(true);
            m_pRect_Model->SetActive(true);
            isTPS = false;
        }
         
    }
}

void CRight_hand::Update(_float fTimeDelta)
{
    
}

void CRight_hand::Late_Update(_float fTimeDelta)
{
    //ITEMNAME item =  CUI_Mgr::Get_Instance()->Get_vecItemlist()->at(0)->Get_ItemName();

    // 아이템 Enum 정리되면 함

    if (m_pGameInstance->Key_Down('P'))
    {
        m_pArm_Model->SetRender(true);
        m_pRect_Model->SetRender(false);
    }

    if (m_pGameInstance->Key_Down('O'))
    {
        m_pArm_Model->SetRender(false);
        m_pRect_Model->SetRender(true);
    }
       
}

CRight_hand* CRight_hand::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CRight_hand* pInstance = new CRight_hand(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CRight_hand");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CRight_hand::Clone(void* pArg)
{
    CRight_hand* pInstance = new CRight_hand(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CRight_hand");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CRight_hand::Free()
{
    __super::Free();

    Safe_Release(m_pArm_Model);
    Safe_Release(m_pRect_Model);
}
