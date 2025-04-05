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
    m_pLeft_Rect_Model = static_cast<CRightHand_Object*>(m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_RightHand"), 3));

 
    if (m_pArm_Model == nullptr ||
        m_pRect_Model == nullptr ||
        m_pSteve == nullptr ||
        m_pCube_Model == nullptr ||
        m_pLeft_Rect_Model == nullptr)
        return E_FAIL;

    Safe_AddRef(m_pSteve);
    Safe_AddRef(m_pArm_Model);
    Safe_AddRef(m_pRect_Model);
    Safe_AddRef(m_pCube_Model);
    Safe_AddRef(m_pLeft_Rect_Model);

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
            m_pLeft_Rect_Model->Set_isTps(true);
            isTPS = true;
        }
        else
        { 
            m_pSteve->SetRender(false);

            m_pArm_Model->Set_isTps(false);
            m_pRect_Model->Set_isTps(false);
            m_pCube_Model->Set_isTps(false);
            m_pLeft_Rect_Model->Set_isTps(false);
            isTPS = false;
        }

        Chage_RightHand();
        Chage_LeftHand();
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
            Chage_LeftHand();

            break;
        }
    }
}

void CRight_hand::Chage_RightHand()
{
    ITEMNAME Name = CUI_Mgr::Get_Instance()->GetItemTypeName();
    Select_Render_Texture(Name);
}

void CRight_hand::Chage_LeftHand()
{
    ITEMNAME Name = CUI_Mgr::Get_Instance()->GetItemTypeName_Left();

    if (Name == ITEMNAME_TORCH)
        Render_Left_Rect();
    else
        m_pLeft_Rect_Model->SetRender(false);
}

void CRight_hand::Select_Render_Texture(ITEMNAME name)
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
      
        Change_Matrix(name);
        return;
    }
}

void CRight_hand::Change_Matrix(ITEMNAME name)
{
    Matrix TPS_mat = {};
    Matrix FPS_mat = {};

    if (name == ITEMNAME_TORCH)
    {
        TPS_mat.Turn_Radian(_float3(0.f, 0.f, 1.f), D3DXToRadian(-45));
    }
    else if (name == ITEM_WEPON_1)
    {
        TPS_mat.Scaling(4.f, 7.f, 4.f);
        TPS_mat.Turn_Radian_Safe_Scale(_float3(0.f, 0.f, 1.f), D3DXToRadian(-45));
        TPS_mat.Turn_Radian_Safe_Scale(_float3(0.f, 1.f, 0.f), D3DXToRadian(-25));

        FPS_mat.Scaling(4.f, 7.f, 4.f);
        FPS_mat.Turn_Radian_Safe_Scale(_float3(1.f, 0.f, 0.f), D3DXToRadian(-45));
        FPS_mat.Set_State(FPS_mat.STATE_POSITION, _float3(-1.5f, -3.5f, -1.f));
    }
        
    m_pRect_Model->Set_TPSMatrix(TPS_mat);
    m_pRect_Model->Set_FPSMatrix(FPS_mat);

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

void CRight_hand::Render_Left_Rect()
{
    int  num = ITEMNAME_TORCH - 100;
    m_pLeft_Rect_Model->SetRender(true);
    m_pLeft_Rect_Model->ChangeTexture(num);
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
    Safe_Release(m_pLeft_Rect_Model);
}
