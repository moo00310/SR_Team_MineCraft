#include "BreakableCube.h"
#include "Tree.h"
#include "Texture.h"
#include "Leaf.h"

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
    m_iMyChunk = pDesc->myChunk;

    if (FAILED(Ready_Objects(m_iHeight, m_iAddLeaf, m_iTreeIndex)))
        return E_FAIL; 

    if (FAILED(Ready_Pos(m_iHeight, m_iAddLeaf, m_iTreeIndex)))
        return E_FAIL;

    return S_OK;
}

void CTree::Priority_Update(_float fTimeDelta)
{
    if (m_pWood->Get_PositionSize() == 0) {
        m_bWoodZero = true;
    }

    if (m_pWood->Get_PositionSize() > 0) {
        m_pWood->Priority_Update(fTimeDelta);
    }
    if (m_pLeaf->Get_PositionSize() > 0) {
        m_pLeaf->Priority_Update(fTimeDelta);
    }
}

void CTree::Update(_float fTimeDelta)
{
    if (m_bWoodZero)
        m_iRemoveFrame++;

    if (m_iRemoveFrame > 10) {
        m_iRemoveFrame = 0;
        dynamic_cast<CLeaf*>(m_pLeaf)->RemoveLeaf();
    }

    if (m_pWood->Get_PositionSize() > 0) {
        m_pWood->Update(fTimeDelta);
    }
    if (m_pLeaf->Get_PositionSize() > 0) {
        m_pLeaf->Update(fTimeDelta);
    }
}

void CTree::Late_Update(_float fTimeDelta)
{
    if ((m_pWood->Get_PositionSize() == 0) && (m_pLeaf->Get_PositionSize() == 0)) {
        Destroy();
        return;
    } 

    if (m_pWood->Get_PositionSize() > 0) {
        m_pWood->Late_Update(fTimeDelta);
    }
    if (m_pLeaf->Get_PositionSize() > 0) {
        m_pLeaf->Late_Update(fTimeDelta);
    }
}

HRESULT CTree::Render()
{


    return S_OK;
}


HRESULT CTree::Ready_Objects(int height, int iAddLeaf, int treeIndex)
{

    // Wood
    CGameObject* pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, LEVEL_YU, TEXT("Prototype_GameObject_Wood")));
    if (CBreakableCube* pBreakableObj = dynamic_cast<CBreakableCube*>(pGameObject)) {
        pBreakableObj->Set_MyChunk(m_iMyChunk);
        m_pWood = pBreakableObj;
    }

    //Leaf
    pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, LEVEL_YU, TEXT("Prototype_GameObject_Leaf")));
    if (CBreakableCube* pBreakableObj = dynamic_cast<CBreakableCube*>(pGameObject)) {
        pBreakableObj->Set_MyChunk(m_iMyChunk);
        m_pLeaf = pBreakableObj;
    }

    return S_OK;
}

HRESULT CTree::Ready_Pos(int height, int iAddLeaf, int treeIndex)
{
    Matrix mat = {};
    wstring LayerName = L"Layer_Tree" + to_wstring(treeIndex);

    vector<_float3> _woodPos;
    vector<_float3> _leafPos;

    // 줄기 배치
    for (int i = 0; i <= height+1; i++)  // height + 1까지 줄기 배치
    {
        if (i == height + 1) {
            _leafPos.push_back(_float3(0.f, 0.5f + i, 0.f) + m_Pos);
            break;
        }
        _woodPos.push_back(_float3(0.f, 0.5f + i, 0.f) + m_Pos);
    }


     //잎 배치
     //  잎(leaf) 생성
    for (int j = 0; j < 20; j++)
    {
        _leafPos.push_back(_float3(leaf[j].x, 0.5f + height - 2, leaf[j].y) + m_Pos);
    }

    for (int j = 0; j < 20; j++)
    {
        _leafPos.push_back(_float3(leaf[j].x, 0.5f + height - 1, leaf[j].y) + m_Pos);
    }

    for (int j = 0; j < 4; j++)
    {
        _leafPos.push_back(_float3(leaf[j].x, 0.5f + height , leaf[j].y) + m_Pos);
    }

    for (int j = 0; j < 4; j++)
    {
        _leafPos.push_back(_float3(leaf[j].x, 0.5f + height + 1, leaf[j].y) + m_Pos);
    }

    if (iAddLeaf == 0) return S_OK;
 
    random_device rd;   // 시드 값 생성
    mt19937 g(rd());    // Mersenne Twister 엔진 사용
    shuffle(vecAddLeadPos.begin(), vecAddLeadPos.end(), g);

    for (int j = 0; j < iAddLeaf; j++)
    {
        _leafPos.push_back(_float3(vecAddLeadPos[j].x, 0.5f + vecAddLeadPos[j].y + height, vecAddLeadPos[j].z) + m_Pos);
    }

    m_pWood->Set_InstanceBuffer(_woodPos, 1.f);
    m_pLeaf->Set_InstanceBuffer(_leafPos,1.f);

    m_pWood->Set_BlockPositions(_woodPos, ITEMNAME_WOOD);
    m_pLeaf->Set_BlockPositions(_leafPos, ITEMNAME_LEAF);

    vector<CCollider_Cube*> _cube = m_pWood->Get_ColliderCube();
    for (auto _collider : _cube) {
        _collider->Set_bColliderActive(true);
    }

    _cube = m_pLeaf->Get_ColliderCube();
    for (auto _collider : _cube) {
        _collider->Set_bColliderActive(true);
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

    if (m_isCloned)
    {
        Safe_Release(m_pWood);
        Safe_Release(m_pLeaf);
    }
}
