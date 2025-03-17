#include "BreakableCube.h"
#include "Tree.h"
#include "Texture.h"

CTree::CTree(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CTree::CTree(const CTree& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CTree::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTree::Initialize(void* pArg)
{
    /*DESC* pDesc = static_cast<DESC*>(pArg);
    m_iHeight = pDesc->iHeight;*/

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_Objects(5)))
        return E_FAIL; 

   
    if (FAILED(Ready_Pos(5)))
        return E_FAIL;

    return S_OK;
}

void CTree::Priority_Update(_float fTimeDelta)
{
}

void CTree::Update(_float fTimeDelta)
{
}

void CTree::Late_Update(_float fTimeDelta)
{
    // 위치이동
}

HRESULT CTree::Render()
{
  

    return S_OK;
}

HRESULT CTree::Ready_Components()
{
    /* For.Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CTree::Ready_Objects(int height)
{
    // Wood
    for (int i = 0; i < height; i++)
    {
        if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_MOO, TEXT("Prototype_GameObject_Wood"),
            LEVEL_MOO, TEXT("Layer_Tree"))))
            return E_FAIL;
    }
    // Leaf
    for (int i = 0; i <= 48; i++)
    {
        if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_MOO, TEXT("Prototype_GameObject_Leaf"),
            LEVEL_MOO, TEXT("Layer_Tree"))))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CTree::Ready_Pos(int height)
{
    Matrix mat = {};

    // 줄기 배치
    for (int i = 0; i < height + 1; i++)  // height + 1까지 줄기 배치
    {
        CGameObject* temp = m_pGameInstance->Get_Object(LEVEL_MOO, TEXT("Layer_Tree"), index);
        CBreakableCube* pCube = dynamic_cast<CBreakableCube*>(temp);
        if (pCube == nullptr) return E_FAIL;

        pCube->SetPos(_float3(0.f, 0.5f + i, 0.f));  // 줄기 위치 설정
        index++;
    }

    // 잎 배치
      // 잎(leaf) 생성
    int leafIndex = 0; // 각 잎 블록을 가져오기 위한 별도 인덱스
    for (int j = 0; j < 20; j++)
    {
        CGameObject* temp = m_pGameInstance->Get_Object(LEVEL_MOO, TEXT("Layer_Tree"), index);
        CBreakableCube* pCube = dynamic_cast<CBreakableCube*>(temp);
        if (pCube == nullptr) return E_FAIL;

        pCube->SetPos(_float3(leaf[j].x, 0.5f + height - 2, leaf[j].y));
        index++;
    }

    for (int j = 0; j < 20; j++)
    {
        CGameObject* temp = m_pGameInstance->Get_Object(LEVEL_MOO, TEXT("Layer_Tree"), index);
        CBreakableCube* pCube = dynamic_cast<CBreakableCube*>(temp);
        if (pCube == nullptr) return E_FAIL;

        pCube->SetPos(_float3(leaf[j].x, 0.5f + height - 1, leaf[j].y));
        index++;
    }

    for (int j = 0; j < 4; j++)
    {
        CGameObject* temp = m_pGameInstance->Get_Object(LEVEL_MOO, TEXT("Layer_Tree"), index);
        CBreakableCube* pCube = dynamic_cast<CBreakableCube*>(temp);
        if (pCube == nullptr) return E_FAIL;

        pCube->SetPos(_float3(leaf[j].x, 0.5f + height, leaf[j].y));
        index++;
    }

    for (int j = 0; j < 4; j++)
    {
        CGameObject* temp = m_pGameInstance->Get_Object(LEVEL_MOO, TEXT("Layer_Tree"), index);
        CBreakableCube* pCube = dynamic_cast<CBreakableCube*>(temp);
        if (pCube == nullptr)
            return E_FAIL;

        pCube->SetPos(_float3(leaf[j].x, 0.5f + height + 1, leaf[j].y));
        index++;
    }

    return S_OK;
}

CTree* CTree::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CTree* pInstance = new CTree(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CTree");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTree::Clone(void* pArg)
{
    CTree* pInstance = new CTree(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CTree");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTree::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
}
