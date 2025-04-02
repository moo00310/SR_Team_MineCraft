#include "Right_hand.h"
#include "UI_Mgr.h"
#include <iostream>

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
    m_pArm_Model = static_cast<CRightHand_Object*>(m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_RightHand"),0));
    m_pRect_Model = static_cast<CRightHand_Object*>(m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_RightHand"),1));
    m_pCube_Model = static_cast<CRightHand_Object*>(m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_RightHand"),2));

 
    if (m_pArm_Model == nullptr ||
        m_pRect_Model == nullptr ||
        m_pSteve == nullptr ||
        m_pCube_Model == nullptr)
        return E_FAIL;

    Safe_AddRef(m_pSteve);
    Safe_AddRef(m_pArm_Model);
    Safe_AddRef(m_pRect_Model);
    Safe_AddRef(m_pCube_Model);


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
            m_pCube_Model->SetActive(false);
            isTPS = true;
        }
        else
        { 
            m_pSteve->SetRender(false);
            m_pArm_Model->SetActive(true);
            m_pRect_Model->SetActive(true);
            m_pCube_Model->SetActive(true);
            isTPS = false;
        }
         
    }
}

void CRight_hand::Update(_float fTimeDelta)
{
    
}

void CRight_hand::Late_Update(_float fTimeDelta)
{

    // Arm
    if (m_pGameInstance->Key_Down('P'))
    {
        m_pArm_Model->SetRender(true);
        m_pRect_Model->SetRender(false);
        m_pCube_Model->SetRender(false);
    }

    // Rect
    if (m_pGameInstance->Key_Down('O'))
    {
        m_pArm_Model->SetRender(false);
        m_pRect_Model->SetRender(true);
        m_pCube_Model->SetRender(false);
    }

    // Cube
    if (m_pGameInstance->Key_Down('I'))
    {
        m_pArm_Model->SetRender(false);
        m_pRect_Model->SetRender(false);
        m_pCube_Model->SetRender(true);
    }

    if (m_pGameInstance->Key_Down('1'))
        Chage_RightHand(0);
    if (m_pGameInstance->Key_Down('2'))
        Chage_RightHand(1);
    if (m_pGameInstance->Key_Down('3'))
        Chage_RightHand(2);
    if (m_pGameInstance->Key_Down('4'))
        Chage_RightHand(3);
    if (m_pGameInstance->Key_Down('5'))
        Chage_RightHand(4);
    if (m_pGameInstance->Key_Down('6'))
        Chage_RightHand(5);
    if (m_pGameInstance->Key_Down('7'))
        Chage_RightHand(6);
    if (m_pGameInstance->Key_Down('8'))
        Chage_RightHand(7);
    if (m_pGameInstance->Key_Down('9'))
        Chage_RightHand(8);
}

void CRight_hand::Chage_RightHand(int slot)
{
    // 현재 슬롯의 아이템 타입을 받아 띄우는 함수임
    // 아무것도 없으면 스티브 주먹

    ITEMNAME temp = CUI_Mgr::Get_Instance()->Get_vecSlotInfolist()->at(slot)->Get_ItemName();
    cout << temp << endl;

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

    Safe_Release(m_pSteve);
    Safe_Release(m_pArm_Model);
    Safe_Release(m_pRect_Model);
    Safe_Release(m_pCube_Model);
}
