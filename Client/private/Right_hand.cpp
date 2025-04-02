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

            m_pArm_Model->Set_isTps(true);
            m_pRect_Model->Set_isTps(true);
            m_pCube_Model->Set_isTps(true);
            isTPS = true;
        }
        else
        { 
            m_pSteve->SetRender(false);

            m_pArm_Model->Set_isTps(false);
            m_pRect_Model->Set_isTps(false);
            m_pCube_Model->Set_isTps(false);
            isTPS = false;
        }

        Chage_RightHand();
    }
}

void CRight_hand::Update(_float fTimeDelta)
{

  
}

void CRight_hand::Late_Update(_float fTimeDelta)
{
 
    for (char key = '1'; key <= '9'; ++key)
    {
        if (m_pGameInstance->Key_Down(key))
        {
            Chage_RightHand();
            break;
        }
    }

}

void CRight_hand::Chage_RightHand()
{
    ITEMNAME Name = CUI_Mgr::Get_Instance()->GetItemTypeName();

    Select_Render(Name);
    Change_Texture(Name);
}

void CRight_hand::Select_Render(ITEMNAME name)
{
    int index = -1;

    if (name == 999)
    {
        Render_Arm();
        return;
    }
    else if (name < 100)
    {
        Render_Cube();
        index = name;
        m_pCube_Model->ChangeTexture(index);
        return;
    }
    else
    {
        Render_Rect();
        index = name - 100;
        m_pRect_Model->ChangeTexture(index);
        return;
    }
}

void CRight_hand::Change_Texture(ITEMNAME name)
{
    // ¸Ô±â°¡´É
    //if(name == ITEMNAME_APPLE)
        //m_pRect_Model.
}

void CRight_hand::Render_Arm()
{
    if (isTPS)
        m_pArm_Model->SetRender(false);
    else
        m_pArm_Model->SetRender(true);
    m_pRect_Model->SetRender(false);
    m_pCube_Model->SetRender(false);
}

void CRight_hand::Render_Rect()
{
    m_pArm_Model->SetRender(false);
    m_pRect_Model->SetRender(true);
    m_pCube_Model->SetRender(false);
}

void CRight_hand::Render_Cube()
{
    m_pArm_Model->SetRender(false);
    m_pRect_Model->SetRender(false);
    m_pCube_Model->SetRender(true);
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
