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
    DESC* pDesc = static_cast<DESC*>(pArg);
    m_iHeight = pDesc->iHeight;
    m_iAddLeaf = pDesc->AddLeaf;
    m_Pos = pDesc->pos;
    m_iTreeIndex = pDesc->TreeIndex;

    if (FAILED(Ready_Objects(m_iHeight, m_iAddLeaf, m_iTreeIndex)))
        return E_FAIL; 

    if (FAILED(Ready_Pos(m_iHeight, m_iAddLeaf, m_iTreeIndex)))
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


HRESULT CTree::Ready_Objects(int height, int iAddLeaf, int treeIndex)
{
    wstring LayerName = L"Layer_Tree" + to_wstring(treeIndex);

    // Wood
    for (int i = 0; i <= height; i++)
    {
        if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_MOO, TEXT("Prototype_GameObject_Wood"),
            LEVEL_MOO, LayerName)))
            return E_FAIL;
    }

    for (int i = 0; i <= 48 +iAddLeaf; i++)
    {
        if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_MOO, TEXT("Prototype_GameObject_Leaf"),
            LEVEL_MOO, LayerName)))
            return E_FAIL;
    }


    return S_OK;
}

HRESULT CTree::Ready_Pos(int height, int iAddLeaf, int treeIndex)
{
    Matrix mat = {};
    wstring LayerName = L"Layer_Tree" + to_wstring(treeIndex);

    // 줄기 배치
    for (int i = 0; i <= height + 1; i++)  // height + 1까지 줄기 배치
    {
        CBreakableCube* pCube = dynamic_cast<CBreakableCube*>(m_pGameInstance->Get_Object(LEVEL_MOO, LayerName.c_str(), index));
        if (pCube == nullptr) return E_FAIL;

        pCube->SetPos(_float3(0.f, 0.5f + i, 0.f) + m_Pos);  // 줄기 위치 설정
        index++;
    }

    // 잎 배치
      // 잎(leaf) 생성
    int leafIndex = 0; // 각 잎 블록을 가져오기 위한 별도 인덱스
    for (int j = 0; j < 20; j++)
    {
        CBreakableCube* pCube = dynamic_cast<CBreakableCube*>(m_pGameInstance->Get_Object(LEVEL_MOO, LayerName.c_str(), index));
        if (pCube == nullptr) return E_FAIL;

        pCube->SetPos(_float3(leaf[j].x, 0.5f + height - 2, leaf[j].y) + m_Pos);
        index++;
    }

    for (int j = 0; j < 20; j++)
    {
        CBreakableCube* pCube = dynamic_cast<CBreakableCube*>(m_pGameInstance->Get_Object(LEVEL_MOO, LayerName.c_str(), index ));
        if (pCube == nullptr) return E_FAIL;

        pCube->SetPos(_float3(leaf[j].x, 0.5f + height - 1, leaf[j].y) + m_Pos);
        index++;
    }

    for (int j = 0; j < 4; j++)
    {
        CBreakableCube* pCube = dynamic_cast<CBreakableCube*>(m_pGameInstance->Get_Object(LEVEL_MOO, LayerName.c_str(), index ));
        if (pCube == nullptr) return E_FAIL;

        pCube->SetPos(_float3(leaf[j].x, 0.5f + height, leaf[j].y) + m_Pos);
        index++;
    }

    for (int j = 0; j < 4; j++)
    {
        CBreakableCube* pCube = dynamic_cast<CBreakableCube*>(m_pGameInstance->Get_Object(LEVEL_MOO, LayerName.c_str(), index ));
        if (pCube == nullptr)
            return E_FAIL;

        pCube->SetPos(_float3(leaf[j].x, 0.5f + height + 1, leaf[j].y)+ m_Pos);
        index++;
    }

    if (iAddLeaf == 0) return S_OK;
 
    random_device rd;   // 시드 값 생성
    mt19937 g(rd());    // Mersenne Twister 엔진 사용
    shuffle(vecAddLeadPos.begin(), vecAddLeadPos.end(), g);

    for (int j = 0; j < iAddLeaf; j++)
    {
        CBreakableCube* pCube = dynamic_cast<CBreakableCube*>(m_pGameInstance->Get_Object(LEVEL_MOO, LayerName.c_str(), index ));
        if (pCube == nullptr)
            return E_FAIL;

        pCube->SetPos(_float3(vecAddLeadPos[j].x, 0.5f + vecAddLeadPos[j].y + height, vecAddLeadPos[j].z) + m_Pos);
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
}
