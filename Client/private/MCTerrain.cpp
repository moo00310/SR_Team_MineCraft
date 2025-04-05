#include "MCTerrain.h"
#include "GameInstance.h"
#include "Stone.h"
#include "CoalOre.h"

#include "Steve.h"
#include "Tree.h"
#include "BreakableRect.h"
#include "Sun.h"

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

    //auto* pSteve = dynamic_cast<CSteve*>(m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_Steve"), 0));
    //if (!pSteve) return E_FAIL;

    //_float3 playerPos = pSteve->GetPos();

    //// 플레이어가 위치한 청크 계산
    //int x = static_cast<int>(playerPos.x) / 16;
    //int z = static_cast<int>(playerPos.z) / 16;
    //int width = static_cast<int>(sqrt(m_iChunkCount));
    //m_currentPlayerChunk = x + (width * z);

    //for (int dy = -1; dy <= 1; ++dy) {
    //    for (int dx = -1; dx <= 1; ++dx) {
    //        int newChunk = m_currentPlayerChunk + dx + dy * width;
    //        // 왼쪽 경계 검사
    //        if (m_currentPlayerChunk % width == 0 && dx == -1) continue;
    //        // 오른쪽 경계 검사
    //        if ((m_currentPlayerChunk + 1) % width == 0 && dx == 1) continue;

    //        ActivateChunkLayer(newChunk, true);
    //        m_ActiveChunkIndexies.emplace(newChunk);

    //    }
    //}

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

list<CCollider*> CMCTerrain::Active_Near_Chunk_Colliders(_float3 vPos, _float fDistSq)
{
    //주위 충돌 할 녀석들 가져오기
    list<CCollider*> Colliders;
    set<_int> AcitveChunkIndexies = Compute_Near_Chunk_Indexies(vPos);
    wchar_t layerName[100];

    //활성화된 청크 인덱스 순회
    for (auto chunkIndex : AcitveChunkIndexies)
    {
        swprintf(layerName, 100, L"Layer_Chunk%d", chunkIndex);
        list<CGameObject*> Objects = m_pGameInstance->Get_GameObjectList(LEVEL_YU, layerName);
        //활성화된 청크의 큐브들 순회
        for (CGameObject* pObj : Objects)
        {
            if (CBreakableCube* pBreakableCube = dynamic_cast<CBreakableCube*>(pObj))
            {
                //큐브의 콜라이더 순회
                for (auto pCollider : pBreakableCube->Get_ColliderCube())
                {
                    _float3 vColliderPos = pCollider->Get_Transform()->Get_State(CTransform::STATE_POSITION) + pCollider->Get_Offset();
                    _float3 vDiff = vPos - vColliderPos;
                    _float fLengthSq = D3DXVec3LengthSq(&vDiff);

                    if (fLengthSq < fDistSq)
                    {
                        //플레이어와 가까이 있는 콜라이더만 활성화 시키고 등록함
                        m_pGameInstance->Add_Collider_CollisionGroup(COLLISION_BLOCK, pCollider);
                        pCollider->Set_bColliderActive(true);
                        Colliders.push_back(pCollider);
                    }
                }
            }
            //나무일 때
            else if (CTree * pTree{ dynamic_cast<CTree*>(pObj) })
            {
                //원목의 콜라이더 순회
                for (auto pCollider : pTree->Get_Wood()->Get_ColliderCube())
                {
                    _float3 vColliderPos = pCollider->Get_Transform()->Get_State(CTransform::STATE_POSITION) + pCollider->Get_Offset();
                    _float3 vDiff = vPos - vColliderPos;
                    _float fLengthSq = D3DXVec3LengthSq(&vDiff);

                    if (fLengthSq < fDistSq)
                    {
                        //플레이어와 가까이 있는 콜라이더만 활성화 시키고 등록함
                        m_pGameInstance->Add_Collider_CollisionGroup(COLLISION_BLOCK, pCollider);
                        pCollider->Set_bColliderActive(true);
                        Colliders.push_back(pCollider);
                    }
                }

                //잎파리의 콜라이더 순회
                for (auto pCollider : pTree->Get_Leaf()->Get_ColliderCube())
                {
                    _float3 vColliderPos = pCollider->Get_Transform()->Get_State(CTransform::STATE_POSITION) + pCollider->Get_Offset();
                    _float3 vDiff = vPos - vColliderPos;
                    _float fLengthSq = D3DXVec3LengthSq(&vDiff);

                    if (fLengthSq < fDistSq)
                    {
                        //플레이어와 가까이 있는 콜라이더만 활성화 시키고 등록함
                        m_pGameInstance->Add_Collider_CollisionGroup(COLLISION_BLOCK, pCollider);
                        pCollider->Set_bColliderActive(true);
                        Colliders.push_back(pCollider);
                    }
                }
            }
			else if (CBreakableRect* pBreakableRect = dynamic_cast<CBreakableRect*>(pObj))
			{
				//큐브의 콜라이더 순회
				for (auto pCollider : pBreakableRect->Get_ColliderRect())
				{
					_float3 vColliderPos = pCollider->Get_Transform()->Get_State(CTransform::STATE_POSITION) + pCollider->Get_Offset();
					_float3 vDiff = vPos - vColliderPos;
					_float fLengthSq = D3DXVec3LengthSq(&vDiff);
					if (fLengthSq < fDistSq)
					{
						//플레이어와 가까이 있는 콜라이더만 활성화 시키고 등록함
						m_pGameInstance->Add_Collider_CollisionGroup(COLLISION_NON_PHYSIC_BLOCK, pCollider);
						pCollider->Set_bColliderActive(true);
						Colliders.push_back(pCollider);
					}
				}
			}
        }
    }

    return Colliders;
}

list<CCollider*> CMCTerrain::Active_Current_Chunk_Colliders(_float3 vPos, _float fDistSq)
{
    //주위 충돌 할 녀석들 가져오기
    list<CCollider*> Colliders;
    
    wchar_t layerName[100];

    //활성화된 청크 인덱스 순회
    swprintf(layerName, 100, L"Layer_Chunk%d", Compute_ChunkIndex(vPos));
    list<CGameObject*> Objects = m_pGameInstance->Get_GameObjectList(LEVEL_YU, layerName);
    //활성화된 청크의 큐브들 순회
    for (CGameObject* pObj : Objects)
    {
        if (CBreakableCube* pBreakableCube = dynamic_cast<CBreakableCube*>(pObj))
        {
            //큐브의 콜라이더 순회
            for (auto pCollider : pBreakableCube->Get_ColliderCube())
            {
                _float3 vColliderPos = pCollider->Get_Transform()->Get_State(CTransform::STATE_POSITION) + pCollider->Get_Offset();
                _float3 vDiff = vPos - vColliderPos;
                _float fLengthSq = D3DXVec3LengthSq(&vDiff);

                if (fLengthSq < fDistSq)
                {
                    //플레이어와 가까이 있는 콜라이더만 활성화 시키고 등록함
                    m_pGameInstance->Add_Collider_CollisionGroup(COLLISION_BLOCK, pCollider);
                    pCollider->Set_bColliderActive(true);
                    Colliders.push_back(pCollider);
                }
            }
        }
        //나무일 때
        else if (CTree * pTree{ dynamic_cast<CTree*>(pObj) })
        {
            //원목의 콜라이더 순회
            for (auto pCollider : pTree->Get_Wood()->Get_ColliderCube())
            {
                _float3 vColliderPos = pCollider->Get_Transform()->Get_State(CTransform::STATE_POSITION) + pCollider->Get_Offset();
                _float3 vDiff = vPos - vColliderPos;
                _float fLengthSq = D3DXVec3LengthSq(&vDiff);

                if (fLengthSq < fDistSq)
                {
                    //플레이어와 가까이 있는 콜라이더만 활성화 시키고 등록함
                    m_pGameInstance->Add_Collider_CollisionGroup(COLLISION_BLOCK, pCollider);
                    pCollider->Set_bColliderActive(true);
                    Colliders.push_back(pCollider);
                }
            }

            //잎파리의 콜라이더 순회
            for (auto pCollider : pTree->Get_Leaf()->Get_ColliderCube())
            {
                _float3 vColliderPos = pCollider->Get_Transform()->Get_State(CTransform::STATE_POSITION) + pCollider->Get_Offset();
                _float3 vDiff = vPos - vColliderPos;
                _float fLengthSq = D3DXVec3LengthSq(&vDiff);

                if (fLengthSq < fDistSq)
                {
                    //플레이어와 가까이 있는 콜라이더만 활성화 시키고 등록함
                    m_pGameInstance->Add_Collider_CollisionGroup(COLLISION_BLOCK, pCollider);
                    pCollider->Set_bColliderActive(true);
                    Colliders.push_back(pCollider);
                }
            }
        }
        else if (CBreakableRect* pBreakableRect = dynamic_cast<CBreakableRect*>(pObj))
        {
            //큐브의 콜라이더 순회
            for (auto pCollider : pBreakableRect->Get_ColliderRect())
            {
                _float3 vColliderPos = pCollider->Get_Transform()->Get_State(CTransform::STATE_POSITION) + pCollider->Get_Offset();
                _float3 vDiff = vPos - vColliderPos;
                _float fLengthSq = D3DXVec3LengthSq(&vDiff);
                if (fLengthSq < fDistSq)
                {
                    //플레이어와 가까이 있는 콜라이더만 활성화 시키고 등록함
                    m_pGameInstance->Add_Collider_CollisionGroup(COLLISION_NON_PHYSIC_BLOCK, pCollider);
                    pCollider->Set_bColliderActive(true);
                    Colliders.push_back(pCollider);
                }
            }
        }
    }

    return Colliders;
}


void CMCTerrain::ActivateChunkLayer(int chunkIndex, bool _b) {
    wchar_t layerName[100];
    swprintf(layerName, 100, L"Layer_Chunk%d", chunkIndex);
    list<CGameObject*> objlist = m_pGameInstance->Get_GameObjectList(LEVEL_YU, layerName);
    for (auto obj : objlist) {
        if (CBreakableCube* _cube = dynamic_cast<CBreakableCube*>(obj)) {
            _cube->Set_ChunkColliderActive(_b);
        }
        if (CBreakableRect* _rect = dynamic_cast<CBreakableRect*>(obj)) {
            _rect->Set_ChunkColliderActive(_b);
        }

        if (CTree* _Tree = dynamic_cast<CTree*>(obj)) {
            _Tree->Get_Wood()->Set_ChunkColliderActive(_b);
            _Tree->Get_Leaf()->Set_ChunkColliderActive(_b);
        }
    }

};

void CMCTerrain::Priority_Update(_float fTimeDelta)
{

}

void CMCTerrain::Update(_float fTimeDelta)
{
    bool currF1State = (GetAsyncKeyState(VK_F1) & 0x8000) != 0;
    bool currF2State = (GetAsyncKeyState(VK_F2) & 0x8000) != 0;

    if (currF1State && !prevF1State)
    {
        // CheckColliderActive();
    }

    prevF1State = currF1State;
    prevF2State = currF2State;
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

    for (CGameObject* pObj : Objects)
    {
        if (CBreakableCube* pBreakableCube = dynamic_cast<CBreakableCube*>(pObj))
        {
            if (eItemName == pBreakableCube->Get_ItemName())
            {
                pBreakableCube->Create_Cube(vCreatePos, vDir);
                break;
            }
        }

        else if (CTree* pTree = dynamic_cast<CTree*>(pObj))
        {
            CBreakableCube* pWood = pTree->Get_Wood();
            if (eItemName == pWood->Get_ItemName())
            {
                pWood->Create_Cube(vCreatePos, vDir);
                break;
            }
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

                    CTree::DESC desc = { randWood, ranLeaf, _float3(eblockData.fPosition.x, eblockData.fPosition.y + 0.5f, eblockData.fPosition.z), 0};
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

        for (size_t i = 0; i < blockTypes.size(); ++i)
        {
             m_pGameInstance->Add_GameObject(LEVEL_YU, blockTypes[i]._name, LEVEL_YU, layerName);
            CBreakableCube* pCube = dynamic_cast<CBreakableCube*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName));
            if (pCube) {
                pCube->Set_InstanceBuffer(*(blockTypes[i]._vec), 0.6f);
                pCube->Set_MyChunk(static_cast<int>(i));
                pCube->Set_BlockPositions(*(blockTypes[i]._vec), blockTypes[i]._itemName);
            }

            CBreakableRect* pRect = dynamic_cast<CBreakableRect*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName));
            if (pRect) {
                pRect->Set_InstanceBuffer(*(blockTypes[i]._vec), 0.6f);
                pRect->Set_MyChunk(static_cast<int>(i));
                pRect->Set_BlockPositions(*(blockTypes[i]._vec), blockTypes[i]._itemName);
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

    MSG_BOX("모든 블록 데이터 처리가 완료되었습니다!");
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

void CMCTerrain::GetPlayerChunk3x3()
{
    // 플레이어 위치 가져오기
    auto* pSteve = dynamic_cast<CSteve*>(m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_Steve"), 0));
    if (!pSteve) return;
    _float3 playerPos = pSteve->GetPos();

    // 플레이어가 위치한 청크 계산
    int x = static_cast<int>(playerPos.x) / 16;
    int z = static_cast<int>(playerPos.z) / 16;
    int width = static_cast<int>(sqrt(m_iChunkCount));
    int chunk = x + (width * z);

    auto ActivateChunkLayer = [&](int chunkIndex) {
        wchar_t layerName[100];
        swprintf(layerName, 100, L"Layer_Chunk%d", chunkIndex);
        m_pGameInstance->SetLayerRenderActive(LEVEL_YU, layerName, true);

        for (CGameObject* pGameObject : m_pGameInstance->Get_GameObjectList(LEVEL_YU, layerName)) {
            if (CBreakableCube* pBreakableCube = dynamic_cast<CBreakableCube*>(pGameObject)) {
                if (pBreakableCube->Get_ChunkColliderActive()) {
                    m_pGameInstance->Add_CollisionGroup(COLLISION_BLOCK, pGameObject);
                }
            }
        }
     };

    // 3x3 영역 활성화
    ActivateChunkLayer(chunk);
    ActivateChunkLayer(chunk - width);
    ActivateChunkLayer(chunk + width);

    if (chunk % width != 0) {
        ActivateChunkLayer(chunk - 1);
        ActivateChunkLayer(chunk - width - 1);
        ActivateChunkLayer(chunk + width - 1);
    }

    if ((chunk + 1) % width != 0) {
        ActivateChunkLayer(chunk + 1);
        ActivateChunkLayer(chunk - width + 1);
        ActivateChunkLayer(chunk + width + 1);
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
            if (newRow < 0 || newCol < 0) continue;
            if (newCol >= width || newRow >= height) continue;

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
