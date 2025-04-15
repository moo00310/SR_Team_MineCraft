#include "MCTerrain.h"
#include "GameInstance.h"
#include "Stone.h"
#include "CoalOre.h"

#include "Steve.h"
#include "Tree.h"
#include "BreakableRect.h"
#include "Sun.h"
#include "UI_Mgr.h"
#include "MissionControl.h"

CMCTerrain::CMCTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject { pGraphic_Device }
{

}

CMCTerrain::CMCTerrain(const CMCTerrain& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CMCTerrain::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMCTerrain::Initialize(void* pArg)
{
	if (FAILED(Ready_Layer_BackGround()))
		return E_FAIL;

    return S_OK;
}

set<_int>& CMCTerrain::Compute_Near_Chunk_Indexies(_float3 vPos)
{
	_float chunkSize = 16.f;
    _float width = static_cast<int>(sqrt(m_iChunkCount));

    static std::set<_int> nearChunks;
    nearChunks.clear();

    for (int z = 0; z < width; ++z) 
    {
        for (int x = 0; x < width; ++x) 
        {
            _float3 chunkCenter = 
            {
                x * chunkSize + chunkSize * 0.5f,
                0.f,
                z * chunkSize + chunkSize * 0.5f
            };

            _float dx = vPos.x - chunkCenter.x;
            _float dz = vPos.z - chunkCenter.z;
            _float distance = sqrtf(dx * dx + dz * dz);

            if (distance <= chunkSize * 0.5f * sqrtf(2.f) + 0.5f)//대각선 거리 + 블럭 반칸 거리 0.5f (여유)
            {
                int chunkIndex = x + z * width;
                nearChunks.emplace(chunkIndex);
            }
        }
    }

    return nearChunks;
}

// 함수 구현
list<CCollider*> CMCTerrain::Active_Near_Chunk_Colliders(_float3 vPos, _float fDistSq)
{
    list<CCollider*> Colliders;
    set<_int> AcitveChunkIndexies = Compute_Near_Chunk_Indexies(vPos);
    wchar_t layerName[100];

    // 거리 안의 위치들을 리스트로 가져오기
    list<_int3> PositionList = Get_Positions_Within_DistSq(vPos, fDistSq);

    // 각 청크의 오브젝트들 검사
    for (auto chunkIndex : AcitveChunkIndexies)
    {
        swprintf(layerName, 100, L"Layer_Chunk%d", chunkIndex);
        list<CGameObject*> Objects = m_pGameInstance->Get_GameObjectList(LEVEL_YU, layerName);

        for (CGameObject* pObj : Objects)
        {
            if (CBreakableCube* pBreakableCube = dynamic_cast<CBreakableCube*>(pObj))
            {
                const auto& colliderMap = pBreakableCube->Get_ColliderCube();
                for (const auto& key : PositionList)
                {
                    auto it = colliderMap.find(key);
                    if (it == colliderMap.end()) continue;

                    CCollider_Cube* pCollider = it->second;
                    m_pGameInstance->Add_Collider_CollisionGroup(COLLISION_BLOCK, pCollider);
                    pCollider->Set_bColliderActive(true);
                    Colliders.push_back(pCollider);
                }
            }
            else if (CTree* pTree = dynamic_cast<CTree*>(pObj))
            {
                const auto& woodMap = pTree->Get_Wood()->Get_ColliderCube();
                const auto& leafMap = pTree->Get_Leaf()->Get_ColliderCube();

                for (const auto& key : PositionList)
                {
                    auto it = woodMap.find(key);
                    if (it != woodMap.end())
                    {
                        CCollider_Cube* pCollider = it->second;
                        m_pGameInstance->Add_Collider_CollisionGroup(COLLISION_BLOCK, pCollider);
                        pCollider->Set_bColliderActive(true);
                        Colliders.push_back(pCollider);
                        continue;
                    }

                    it = leafMap.find(key);
                    if (it != leafMap.end())
                    {
                        CCollider_Cube* pCollider = it->second;
                        m_pGameInstance->Add_Collider_CollisionGroup(COLLISION_BLOCK, pCollider);
                        pCollider->Set_bColliderActive(true);
                        Colliders.push_back(pCollider);
                    }
                }
            }
            else if (CBreakableRect* pBreakableRect = dynamic_cast<CBreakableRect*>(pObj))
            {
                const auto& colliderMap = pBreakableRect->Get_ColliderRect();
                for (const auto& key : PositionList)
                {
                    auto it = colliderMap.find(key);
                    if (it == colliderMap.end()) continue;

                    CCollider_Cube* pCollider = it->second;
                    m_pGameInstance->Add_Collider_CollisionGroup(COLLISION_NON_PHYSIC_BLOCK, pCollider);
                    pCollider->Set_bColliderActive(true);
                    Colliders.push_back(pCollider);
                }
            }
        }
    }

    return Colliders;
}

list<CCollider*> CMCTerrain::Active_Current_Chunk_Colliders(_float3 vPos, _float fDistSq)
{
    list<CCollider*> Colliders;

    wchar_t layerName[100];
    swprintf(layerName, 100, L"Layer_Chunk%d", Compute_ChunkIndex(vPos));
    list<CGameObject*> Objects = m_pGameInstance->Get_GameObjectList(LEVEL_YU, layerName);

    // 거리 내의 타일 좌표 리스트
    list<_int3> Positions = Get_Positions_Within_DistSq(vPos, fDistSq);

    for (CGameObject* pObj : Objects)
    {
        for (auto vPosition : Positions)
        {
            _int3 key{};
            key.x = static_cast<_int>(vPosition.x);
            key.y = static_cast<_int>(vPosition.y);
            key.z = static_cast<_int>(vPosition.z);

            if (CBreakableCube* pBreakableCube = dynamic_cast<CBreakableCube*>(pObj))
            {
                auto& colliderMap = pBreakableCube->Get_ColliderCube();
                auto it = colliderMap.find(key);
                if (it == colliderMap.end())
                    continue;

                CCollider_Cube* pCollider = it->second;

                m_pGameInstance->Add_Collider_CollisionGroup(COLLISION_BLOCK, pCollider);
                pCollider->Set_bColliderActive(true);
                Colliders.push_back(pCollider);
            }
            else if (CTree* pTree = dynamic_cast<CTree*>(pObj))
            {
                auto& woodMap = pTree->Get_Wood()->Get_ColliderCube();
                auto& leafMap = pTree->Get_Leaf()->Get_ColliderCube();

                CCollider_Cube* pCollider = nullptr;

                auto itWood = woodMap.find(key);
                if (itWood != woodMap.end())
                {
                    pCollider = itWood->second;
                }
                else
                {
                    auto itLeaf = leafMap.find(key);
                    if (itLeaf != leafMap.end())
                    {
                        pCollider = itLeaf->second;
                    }
                }

                if (pCollider == nullptr)
                    continue;

                m_pGameInstance->Add_Collider_CollisionGroup(COLLISION_BLOCK, pCollider);
                pCollider->Set_bColliderActive(true);
                Colliders.push_back(pCollider);
            }
            else if (CBreakableRect* pBreakableRect = dynamic_cast<CBreakableRect*>(pObj))
            {
                auto& colliderMap = pBreakableRect->Get_ColliderRect();
                auto it = colliderMap.find(key);
                if (it == colliderMap.end())
                    continue;

                CCollider_Cube* pCollider = it->second;

                m_pGameInstance->Add_Collider_CollisionGroup(COLLISION_NON_PHYSIC_BLOCK, pCollider);
                pCollider->Set_bColliderActive(true);
                Colliders.push_back(pCollider);
            }
        }
    }

    return Colliders;
}


//스레드풀 버전 해봤는데 별 차이가없음
#pragma region 스레드풀 버전
//스레드풀 버전
//list<CCollider*> CMCTerrain::Active_Near_Chunk_Colliders(_float3 vPos, _float fDistSq)
//{
//    list<CCollider*> Colliders;
//    set<_int> ActiveChunkIndexies = Compute_Near_Chunk_Indexies(vPos);
//    wchar_t layerName[100];
//    std::mutex mtx;
//
//    m_ThreadPool; // 스레드 풀 생성
//
//    for (auto chunkIndex : ActiveChunkIndexies)
//    {
//        swprintf(layerName, 100, L"Layer_Chunk%d", chunkIndex);
//        list<CGameObject*> Objects = m_pGameInstance->Get_GameObjectList(LEVEL_YU, layerName);
//
//        for (CGameObject* pObj : Objects)
//        {
//            m_ThreadPool.Enqueue([&, pObj]() {
//                auto HandleColliderList = [&](const vector<CCollider_Cube*>& colliderList, COLLISION_GROUP eGroup) {
//                    for (auto pCollider : colliderList)
//                    {
//                        _float3 vColliderPos = pCollider->Get_Transform()->Get_State(CTransform::STATE_POSITION) + pCollider->Get_Offset();
//                        _float3 vDiff = vPos - vColliderPos;
//                        _float fLengthSq = D3DXVec3LengthSq(&vDiff);
//
//                        if (fLengthSq < fDistSq)
//                        {
//                            std::lock_guard<std::mutex> lock(mtx);
//                            m_pGameInstance->Add_Collider_CollisionGroup(eGroup, pCollider);
//                            pCollider->Set_bColliderActive(true);
//                            Colliders.push_back(pCollider);
//                        }
//                    }
//                    };
//
//                if (CBreakableCube* pBreakableCube = dynamic_cast<CBreakableCube*>(pObj))
//                {
//                    HandleColliderList(pBreakableCube->Get_ColliderCube(), COLLISION_BLOCK);
//                }
//                else if (CTree* pTree = dynamic_cast<CTree*>(pObj))
//                {
//                    HandleColliderList(pTree->Get_Wood()->Get_ColliderCube(), COLLISION_BLOCK);
//                    HandleColliderList(pTree->Get_Leaf()->Get_ColliderCube(), COLLISION_BLOCK);
//                }
//                else if (CBreakableRect* pBreakableRect = dynamic_cast<CBreakableRect*>(pObj))
//                {
//                    HandleColliderList(pBreakableRect->Get_ColliderRect(), COLLISION_NON_PHYSIC_BLOCK);
//                }
//                });
//        }
//    }
//
//    // 모든 작업 대기
//    m_ThreadPool.WaitAll();
//
//    return Colliders;
//}
//
//list<CCollider*> CMCTerrain::Active_Current_Chunk_Colliders(_float3 vPos, _float fDistSq)
//{
//    list<CCollider*> Colliders;
//    std::mutex mtx;
//
//    wchar_t layerName[100];
//    swprintf(layerName, 100, L"Layer_Chunk%d", Compute_ChunkIndex(vPos));
//
//    list<CGameObject*> Objects = m_pGameInstance->Get_GameObjectList(LEVEL_YU, layerName);
//
//    for (CGameObject* pObj : Objects)
//    {
//        m_ThreadPool.Enqueue([&, pObj]() {
//            auto HandleColliderList = [&](const vector<CCollider_Cube*>& colliderList, COLLISION_GROUP eGroup) {
//                for (auto pCollider : colliderList)
//                {
//                    _float3 vColliderPos = pCollider->Get_Transform()->Get_State(CTransform::STATE_POSITION) + pCollider->Get_Offset();
//                    _float3 vDiff = vPos - vColliderPos;
//                    _float fLengthSq = D3DXVec3LengthSq(&vDiff);
//
//                    if (fLengthSq < fDistSq)
//                    {
//                        std::lock_guard<std::mutex> lock(mtx);
//                        m_pGameInstance->Add_Collider_CollisionGroup(eGroup, pCollider);
//                        pCollider->Set_bColliderActive(true);
//                        Colliders.push_back(pCollider);
//                    }
//                }
//                };
//
//            if (CBreakableCube* pBreakableCube = dynamic_cast<CBreakableCube*>(pObj))
//            {
//                HandleColliderList(pBreakableCube->Get_ColliderCube(), COLLISION_BLOCK);
//            }
//            else if (CTree* pTree = dynamic_cast<CTree*>(pObj))
//            {
//                HandleColliderList(pTree->Get_Wood()->Get_ColliderCube(), COLLISION_BLOCK);
//                HandleColliderList(pTree->Get_Leaf()->Get_ColliderCube(), COLLISION_BLOCK);
//            }
//            else if (CBreakableRect* pBreakableRect = dynamic_cast<CBreakableRect*>(pObj))
//            {
//                HandleColliderList(pBreakableRect->Get_ColliderRect(), COLLISION_NON_PHYSIC_BLOCK);
//            }
//            });
//    }
//
//    m_ThreadPool.WaitAll();
//
//    return Colliders;
//}
#pragma endregion



void CMCTerrain::Priority_Update(_float fTimeDelta)
{
}

void CMCTerrain::Update(_float fTimeDelta)
{

    if (CSteve* _steve = dynamic_cast<CSteve*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Steve")))) {
        m_prePlayerChunk = m_currentPlayerChunk;
        m_currentPlayerChunk = Compute_ChunkIndex(_steve->GetPos());
        // 청크가 변했으면
        if (m_prePlayerChunk != m_currentPlayerChunk) {
            // 렌더 비활성화
            OffAllChunkLayer();

            int width = static_cast<int>(sqrt(m_iChunkCount));

            auto ActivateChunkLayer = [&](int chunkIndex) {
                wchar_t layerName[100];
                swprintf(layerName, 100, L"Layer_Chunk%d", chunkIndex);
                m_pGameInstance->SetLayerRenderActive(LEVEL_YU, layerName, true); };

            // 3x3 영역 활성화
            ActivateChunkLayer(m_currentPlayerChunk);
            ActivateChunkLayer(m_currentPlayerChunk - width);
            ActivateChunkLayer(m_currentPlayerChunk - width*2);
            ActivateChunkLayer(m_currentPlayerChunk + width);
            ActivateChunkLayer(m_currentPlayerChunk + width*2);

            if (m_currentPlayerChunk % width != 0) {
                ActivateChunkLayer(m_currentPlayerChunk - 1);
                ActivateChunkLayer(m_currentPlayerChunk - width - 1);
                ActivateChunkLayer(m_currentPlayerChunk - (width * 2) - 1);
                ActivateChunkLayer(m_currentPlayerChunk + width - 1);
                ActivateChunkLayer(m_currentPlayerChunk + (width * 2) - 1);

                if ((m_currentPlayerChunk - 1) % width != 0) {
                    ActivateChunkLayer(m_currentPlayerChunk - 2);
                    ActivateChunkLayer(m_currentPlayerChunk - width - 2);
                    ActivateChunkLayer(m_currentPlayerChunk - (width * 2) - 2);
                    ActivateChunkLayer(m_currentPlayerChunk + width - 2);
                    ActivateChunkLayer(m_currentPlayerChunk + (width * 2) - 2);
                }
            }

            if ((m_currentPlayerChunk + 1) % width != 0) {
                ActivateChunkLayer(m_currentPlayerChunk + 1);
                ActivateChunkLayer(m_currentPlayerChunk - width + 1);
                ActivateChunkLayer(m_currentPlayerChunk - (width * 2) + 1);
                ActivateChunkLayer(m_currentPlayerChunk + width + 1);
                ActivateChunkLayer(m_currentPlayerChunk + (width * 2) + 1);

                if ((m_currentPlayerChunk + 2) % width != 0) {
                    ActivateChunkLayer(m_currentPlayerChunk + 2);
                    ActivateChunkLayer(m_currentPlayerChunk - width + 2);
                    ActivateChunkLayer(m_currentPlayerChunk - (width * 2) + 2);
                    ActivateChunkLayer(m_currentPlayerChunk + width + 2);
                    ActivateChunkLayer(m_currentPlayerChunk + (width * 2) + 2);
                }
            }
        }
    }
}


void CMCTerrain::Late_Update(_float fTimeDelta)
{
    
}

HRESULT CMCTerrain::Render()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
    return S_OK;
}

CGameInstance* CMCTerrain::GetGameInstance()
{
   return m_pGameInstance; 
}

int CMCTerrain::GetFileCount()
{
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(L"../bin/Resources/DataFiles/BlockDataChunk*.txt", &findFileData);

    if (hFind != INVALID_HANDLE_VALUE)
    {
        do {
            m_iChunkCount++;
        } while (FindNextFile(hFind, &findFileData));

        FindClose(hFind);
    }

    if (CSun* _sun = dynamic_cast<CSun*>(m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_Sun"), 0))) {
        _sun->Set_ChunkCount(m_iChunkCount);
    }

    return m_iChunkCount;
}

void CMCTerrain::Create_Cube(_float3 vMyPos, ITEMNAME eItemName, _float3 vCreatePos, _float3 vDir)
{
    //eItemName = ITEMNAME_GRASSDIRT;
    //eItemName = ITEMNAME_DIRT;
    //eItemName = ITEMNAME_COALORE;
    //eItemName = ITEMNAME_WOOD;
    //eItemName = ITEMNAME_IRONORE;

    //현재 청크레이어 가져오기
    wchar_t layerName[100];
    swprintf(layerName, 100, L"Layer_Chunk%d", Compute_ChunkIndex(vCreatePos));
    list<CGameObject*> Objects = m_pGameInstance->Get_GameObjectList(LEVEL_YU, layerName);

    bool _bExist = false;;
    for (CGameObject* pObj : Objects)
    {
        if (CBreakableCube* pBreakableCube = dynamic_cast<CBreakableCube*>(pObj))
        {
            if (eItemName == pBreakableCube->Get_ItemName())
            {
                pBreakableCube->Create_Cube(vCreatePos, vDir);
                _bExist = true;
                CUI_Mgr::Get_Instance()->ItemCount_Update(eItemName, -1);
                break;
            }
        }

        else if (CTree* pTree = dynamic_cast<CTree*>(pObj))
        {
            CBreakableCube* pWood = pTree->Get_Wood();
            if (eItemName == pWood->Get_ItemName())
            {
                pWood->Create_Cube(vCreatePos, vDir);
                _bExist = true;
                CUI_Mgr::Get_Instance()->ItemCount_Update(eItemName, -1);
                break;
            }
        }
    }

    if (!_bExist) {
        CBreakableCube* pCube = nullptr;
        switch (eItemName)
        {
        case Client::ITEMNAME_FURANCE:
            m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Furnace"), LEVEL_YU, layerName);
            pCube = dynamic_cast<CBreakableCube*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName));
            if (pCube) {
                vector<D3DXVECTOR3> pos;
                pos.push_back(vCreatePos+ vDir);
                pCube->Set_InstanceBuffer(pos, 0.6f);
                pCube->Set_MyChunk(Compute_ChunkIndex(vCreatePos));
                pCube->Set_BlockPositions(pos, eItemName);
                CUI_Mgr::Get_Instance()->ItemCount_Update(eItemName, -1);
                _bExist = true;
            }
            break;
        case Client::ITEMNAME_CRAFTINGTABLE:
            m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_CraftingTableCube"), LEVEL_YU, layerName);
            pCube = dynamic_cast<CBreakableCube*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName));
            if (pCube) {
                vector<D3DXVECTOR3> pos;
                pos.push_back(vCreatePos+ vDir);
                pCube->Set_InstanceBuffer(pos, 0.6f);
                pCube->Set_MyChunk(Compute_ChunkIndex(vCreatePos));
                pCube->Set_BlockPositions(pos, eItemName);
                CUI_Mgr::Get_Instance()->ItemCount_Update(eItemName, -1);
                _bExist = true;
            }
            break;
        case Client::ITEMNAME_OAKPLANKS:
            m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_OakWood"), LEVEL_YU, layerName);
            pCube = dynamic_cast<CBreakableCube*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName));
            if (pCube) {
                vector<D3DXVECTOR3> pos; 
                pos.push_back(vCreatePos + vDir);
                pCube->Set_InstanceBuffer(pos, 0.6f);
                pCube->Set_MyChunk(Compute_ChunkIndex(vCreatePos));
                pCube->Set_BlockPositions(pos, eItemName);
                CUI_Mgr::Get_Instance()->ItemCount_Update(eItemName, -1);
                _bExist = true;
            }
            break;
        case Client::ITEMNAME_COBBLESTONE:
            m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_CobbleStone"), LEVEL_YU, layerName);
            pCube = dynamic_cast<CBreakableCube*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName));
            if (pCube) {
                vector<D3DXVECTOR3> pos;
                pos.push_back(vCreatePos + vDir);
                pCube->Set_InstanceBuffer(pos, 0.6f);
                pCube->Set_MyChunk(Compute_ChunkIndex(vCreatePos));
                pCube->Set_BlockPositions(pos, eItemName);
                CUI_Mgr::Get_Instance()->ItemCount_Update(eItemName, -1);
                _bExist = true;
            }
            break;
        default:
            break;
        }
    }

    if (_bExist) {
        if (CMissionControl* _control = dynamic_cast<CMissionControl*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Mission")))) {
            _control->Update_Mission(L"block");
        }
    }
}

_int CMCTerrain::Compute_ChunkIndex(_float3 vPos)
{
	int x = static_cast<int>(vPos.x) / 16;
	int z = static_cast<int>(vPos.z) / 16;
	int width = static_cast<int>(sqrt(m_iChunkCount));
	return x + (width * z); 
}

list<_int3> CMCTerrain::Get_Positions_Within_DistSq(_float3 vPos, float fDistSq)
{
    list<_int3> result;

    int radius = static_cast<int>(std::ceil(std::sqrt(fDistSq)));

    int cx = static_cast<int>(vPos.x);
    int cy = static_cast<int>(vPos.y);
    int cz = static_cast<int>(vPos.z);

    float fx = vPos.x;
    float fy = vPos.y;
    float fz = vPos.z;

    for (int x = cx - radius; x <= cx + radius; ++x)
    {
        float dx = fx - x;
        float dx2 = dx * dx;
        if (dx2 > fDistSq) continue;

        for (int y = cy - radius; y <= cy + radius; ++y)
        {
            float dy = fy - y;
            float dy2 = dy * dy;
            if (dx2 + dy2 > fDistSq) continue;

            for (int z = cz - radius; z <= cz + radius; ++z)
            {
                float dz = fz - z;
                float distSq = dx2 + dy2 + dz * dz;

                if (distSq <= fDistSq)
                {
                    result.push_back({ x, y, z });
                }
            }
        }
    }

    return result;
}

HRESULT CMCTerrain::Ready_Layer_BackGround()
{

    int fileCount = GetFileCount();
    if (fileCount == 0) {
        MSG_BOX("파일이 없습니다");
        return S_OK;
    }

    for (int i = 0; i < fileCount; ++i)
    {
        wchar_t filename[100];
        swprintf(filename, 100, L"../bin/Resources/DataFiles/BlockDataChunk%d.txt", i);

        wchar_t layerName[100];
        swprintf(layerName, 100, L"Layer_Chunk%d", i);

        HANDLE hFile = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
            return 1;
        }


        DWORD dwBytesRead;
        BLOCKDESC eblockData;

        vector<D3DXVECTOR3> stoneVec;
        vector<D3DXVECTOR3> dirtVec;
        vector<D3DXVECTOR3> grassDirtVec;
        vector<D3DXVECTOR3> ironVec;
        vector<D3DXVECTOR3> coalVec;

        vector<CTree::DESC> vecTreeDesc = {};
        vector<D3DXVECTOR3> vecGrass;
        vector<D3DXVECTOR3> vecTulip;

        int percent = 999;

        while (ReadFile(hFile, &eblockData, sizeof(BLOCKDESC), &dwBytesRead, NULL) && dwBytesRead > 0)
        {

            switch (eblockData.eBlockType)
            {
            case GRASSDIRT:
                grassDirtVec.push_back(eblockData.fPosition);

                percent = rand() % 100;
                if (percent < 1) {
                    int randWood = rand() % 3 + 4;
                    int ranLeaf = rand() % 8 + 4;

                    CTree::DESC desc = { randWood, ranLeaf, _float3(eblockData.fPosition.x, eblockData.fPosition.y + 0.5f, eblockData.fPosition.z), i};
                    vecTreeDesc.push_back(desc);
                }
                else if (1 <= percent && percent < 5) {
                    vecGrass.push_back(_float3(eblockData.fPosition.x, eblockData.fPosition.y + 1, eblockData.fPosition.z));
                }
                else if (5 <= percent && percent < 6) {
                    vecTulip.push_back(_float3(eblockData.fPosition.x, eblockData.fPosition.y + 1, eblockData.fPosition.z));
                }
                else
                {
                    m_SpawnPos[i].push_back(_float3(eblockData.fPosition.x, eblockData.fPosition.y + 1, eblockData.fPosition.z));
                }

                break;
            case DIRT:
                dirtVec.push_back(eblockData.fPosition);
                break;
            case STONE:
                stoneVec.push_back(eblockData.fPosition);
                break;
            case IRONORE:
                ironVec.push_back(eblockData.fPosition);
                break;
            case COALORE:
                coalVec.push_back(eblockData.fPosition);
                break;
            default:
                break;
            }
        }
        CloseHandle(hFile);

        struct desc {
            const wchar_t* _name;
            vector<D3DXVECTOR3>* _vec;
            ITEMNAME _itemName;
        };

         vector<desc> blockTypes = {
            {TEXT("Prototype_GameObject_GrassDirt"), &grassDirtVec, ITEMNAME_GRASSDIRT},
            {TEXT("Prototype_GameObject_Dirt"), &dirtVec, ITEMNAME_DIRT},
            {TEXT("Prototype_GameObject_Stone"), &stoneVec, ITEMNAME_STONE},
            {TEXT("Prototype_GameObject_IronOre"), &ironVec, ITEMNAME_IRONORE},
            {TEXT("Prototype_GameObject_CoalOre"), &coalVec,ITEMNAME_COALORE},
            {TEXT("Prototype_GameObject_Grass"), &vecGrass, ITEMNAME_GRASS},
            {TEXT("Prototype_GameObject_RedTulip"), &vecTulip, ITEMNAME_REDTULIP},
        };

        for (size_t j = 0; j < blockTypes.size(); ++j)
        {
             m_pGameInstance->Add_GameObject(LEVEL_YU, blockTypes[j]._name, LEVEL_YU, layerName);
            CBreakableCube* pCube = dynamic_cast<CBreakableCube*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName));
            if (pCube) {
                pCube->Set_InstanceBuffer(*(blockTypes[j]._vec), 0.6f);
                pCube->Set_MyChunk(static_cast<int>(i));
                pCube->Set_BlockPositions(*(blockTypes[j]._vec), blockTypes[j]._itemName);
            }

            CBreakableRect* pRect = dynamic_cast<CBreakableRect*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName));
            if (pRect) {
                pRect->Set_InstanceBuffer(*(blockTypes[j]._vec), 0.6f);
                pRect->Set_MyChunk(static_cast<int>(i));
                pRect->Set_BlockPositions(*(blockTypes[j]._vec), blockTypes[j]._itemName);
            }
        }

        //맵에서 자동 생성되지 않지만 컨테이너 용도로 만들어야함
        //if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Wood"), LEVEL_YU, layerName)))
        //    return E_FAIL;
        //CBreakableCube* pCube = dynamic_cast<CBreakableCube*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName));
        //if (pCube) {
        //    //pCube->Set_InstanceBuffer(*(blockTypes[i]._vec), 0.6f);
        //    pCube->Set_MyChunk(static_cast<int>(i));
        //    //pCube->Set_BlockPositions(*(blockTypes[i]._vec), blockTypes[i]._itemName);
        //}

        for (auto desc : vecTreeDesc) {
            if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Tree"), LEVEL_YU, layerName, &desc)))
                return E_FAIL;
        }

    }

    //MSG_BOX("모든 블록 데이터 처리가 완료되었습니다!");
    return S_OK;
}


void CMCTerrain::SetColliderChunk()
{
    //// 이전 프레임에 충돌 매니저에 올라가라고 활성화되어 있던 청크 비활성화
    //wchar_t layerName[100];
    //swprintf(layerName, 100, L"Layer_Chunk%d", m_prePlayerChunk);
    //list<CGameObject*> objlist = m_pGameInstance->Get_GameObjectList(LEVEL_YU, layerName);
    //for (auto obj : objlist) {
    //    if (CBreakableCube* _copy = dynamic_cast<CBreakableCube*>(obj)) {
    //        _copy->Set_ChunkColliderActive(false);
    //    }

    //    if (CTree* _copy = dynamic_cast<CTree*>(obj)) {
    //        _copy->Get_Leaf()->Set_ChunkColliderActive(false);
    //        _copy->Get_Wood()->Set_ChunkColliderActive(false);
    //    }


    //    if (CBreakableRect* _copy = dynamic_cast<CBreakableRect*>(obj)) {
    //        _copy->Set_ChunkColliderActive(false);
    //    }
    //}

    //// 현재 플레이어 밑에 있는 청크만 충돌 매니저에 올림
    //swprintf(layerName, 100, L"Layer_Chunk%d", m_currentPlayerChunk);
    //objlist = m_pGameInstance->Get_GameObjectList(LEVEL_YU, layerName);
    //for (auto obj : objlist) {
    //    if (CBreakableCube* _copy = dynamic_cast<CBreakableCube*>(obj)) {
    //        _copy->Set_ChunkColliderActive(true);
    //    }

    //    if (CTree* _copy = dynamic_cast<CTree*>(obj)) {
    //        _copy->Get_Leaf()->Set_ChunkColliderActive(true);
    //        _copy->Get_Wood()->Set_ChunkColliderActive(true);
    //    }

    //    if (CBreakableRect* _copy = dynamic_cast<CBreakableRect*>(obj)) {
    //        _copy->Set_ChunkColliderActive(true);
    //    }
    //}
}

void CMCTerrain::OffAllChunkLayer()
{
    for (int i = 0; i < m_iChunkCount; ++i) {
        wchar_t layerName[100];
        swprintf(layerName, 100, L"Layer_Chunk%d", i);

        m_pGameInstance->SetLayerRenderActive(LEVEL_YU, layerName, false);
    }
}

const vector<_float3>& CMCTerrain::Get_SpwanAble()
{
    // 플레이어 위치 가져오기
    auto* pSteve = dynamic_cast<CSteve*>(m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_Steve"), 0));
    if (!pSteve) return m_vecMerged;
    _float3 playerPos = pSteve->GetPos();

    int x = static_cast<int>(playerPos.x) / 16;
    int z = static_cast<int>(playerPos.z) / 16;
    int width = static_cast<int>(sqrt(m_iChunkCount));
    int height = m_iChunkCount / width;
    int chunk = x + (width * z);

    int row = chunk / width;
    int col = chunk % width;

    m_vecMerged.clear();

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue; // 자기 자신은 제외

            int newRow = row + dy;
            int newCol = col + dx;

            // 경계 체크 (예: 맵 밖은 제외)
            if (newRow < 2 || newCol < 2) continue;
            if (newCol >= width - 2 || newRow >= height - 2) continue;

            int neighborChunk = newRow * width + newCol;

            // m_SpawnPos에 해당 청크가 존재하면 병합
            auto it = m_SpawnPos.find(neighborChunk);
            if (it != m_SpawnPos.end()) {
                m_vecMerged.insert(m_vecMerged.end(), it->second.begin(), it->second.end());
            }
        }
    }

    return m_vecMerged;
}

CMCTerrain* CMCTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMCTerrain* pInstance = new CMCTerrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMCTerrain::Clone(void* pArg)
{	
	CMCTerrain* pInstance = new CMCTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMCTerrain::Free()
{
	__super::Free();

}
