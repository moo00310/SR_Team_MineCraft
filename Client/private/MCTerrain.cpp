#include "MCTerrain.h"
#include "GameInstance.h"
#include "Stone.h"
#include "CoalOre.h"

#include "Steve.h"
#include "Tree.h"
#include "BreakableRect.h"

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

    SetColliderChunk();
    

    return S_OK;
}

void CMCTerrain::Priority_Update(_float fTimeDelta)
{

}

void CMCTerrain::Update(_float fTimeDelta)
{
    bool currF1State = (GetAsyncKeyState(VK_F1) & 0x8000) != 0;
    bool currF2State = (GetAsyncKeyState(VK_F2) & 0x8000) != 0;

    if (currF1State && !prevF1State)
    {
        //CheckColliderActive();
    }

    prevF1State = currF1State;
    prevF2State = currF2State;
}

void CMCTerrain::Late_Update(_float fTimeDelta)
{
    auto* pSteve = dynamic_cast<CSteve*>(m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_Steve"), 0));
    if (!pSteve) return;

    _float3 playerPos = pSteve->GetPos();

    // 플레이어가 위치한 청크 계산
    int x = static_cast<int>(playerPos.x) / 16;
    int z = static_cast<int>(playerPos.z) / 16;
    int width = static_cast<int>(sqrt(m_iChunkCount));
    m_currentPlayerChunk = x + (width * z);

    //청크를 이동 했다면
    if (m_prePlayerChunk != m_currentPlayerChunk) {
        SetColliderChunk();
        m_prePlayerChunk = m_currentPlayerChunk;
    }

    //CheckColliderActive();
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

    return m_iChunkCount;
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
        vector<D3DXVECTOR3> grassVec;
        vector<D3DXVECTOR3> ironVec;
        vector<D3DXVECTOR3> coalVec;

        vector<CTree::DESC> vecTreeDesc = {};
        vector<D3DXVECTOR3> vecGrass;
        vector<D3DXVECTOR3> vecTulip;

        int percent = 999;
        CBreakableRect* pRect = nullptr;

        while (ReadFile(hFile, &eblockData, sizeof(BLOCKDESC), &dwBytesRead, NULL) && dwBytesRead > 0)
        {

            switch (eblockData.eBlockType)
            {
            case GRASSDIRT:
                grassVec.push_back(eblockData.fPosition);

                percent = rand() % 100;
                if (percent < 1) {
                    int randWood = rand() % 3 + 4;
                    int ranLeaf = rand() % 8 + 4;

                    CTree::DESC desc = { randWood, ranLeaf, _float3(eblockData.fPosition.x, eblockData.fPosition.y + 0.5, eblockData.fPosition.z),0 };
                    vecTreeDesc.push_back(desc);
                }
                else if (1 <= percent && percent < 5) {
                    vecGrass.push_back(_float3(eblockData.fPosition.x, eblockData.fPosition.y + 1, eblockData.fPosition.z));
                }
                else if (5 <= percent && percent < 6) {
                    vecTulip.push_back(_float3(eblockData.fPosition.x, eblockData.fPosition.y + 0.7, eblockData.fPosition.z));
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


         vector<pair<const wchar_t*, vector<D3DXVECTOR3>*>> blockTypes = {
            {TEXT("Prototype_GameObject_GrassDirt"), &grassVec},
            {TEXT("Prototype_GameObject_Dirt"), &dirtVec},
            {TEXT("Prototype_GameObject_Stone"), &stoneVec},
            {TEXT("Prototype_GameObject_IronOre"), &ironVec},
            {TEXT("Prototype_GameObject_CoalOre"), &coalVec}
        };


        for (size_t i = 0; i < blockTypes.size(); ++i)
        {
            m_pGameInstance->Add_GameObject(LEVEL_YU, blockTypes[i].first, LEVEL_YU, layerName);
            CBreakableCube* pCube = dynamic_cast<CBreakableCube*>(m_pGameInstance->Get_Object(LEVEL_YU, layerName, static_cast<int>(i)));
            if (pCube) {
                pCube->Set_InstanceBuffer(*(blockTypes[i].second));
                pCube->Set_MyChunk(static_cast<int>(i));
                pCube->Set_BlockPositions(*(blockTypes[i].second));
            }
        }

        for (auto desc : vecTreeDesc) {
            if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Tree"), LEVEL_YU, layerName, &desc)))
                return E_FAIL;
        }

        //그래스랑 튤립 소환 안시켰는데도 레퍼런스카운트 오류남

        for (auto pos : vecGrass) {
            if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Grass"), LEVEL_YU, layerName)))
                return 1;
            pRect = dynamic_cast<CBreakableRect*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName));
            if (pRect) {
                pRect->SetPos(pos);
            }
        }

        for (auto pos : vecTulip) {
            if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_RedTulip"), LEVEL_YU, layerName)))
                return 1;
            pRect = dynamic_cast<CBreakableRect*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName));
            if (pRect) {
                pRect->SetPos(pos);
            }
        }
    }

    MSG_BOX("모든 블록 데이터 처리가 완료되었습니다!");
    return S_OK;
}

/*
void CMCTerrain::CheckColliderActive()
{
    //이전 청크의 활성화된 충돌 블럭들 모두 비활성화
    wchar_t layerName[100];
    swprintf(layerName, 100, L"Layer_Chunk%d", m_prePlayerChunk);

    list<class CGameObject*> _copyObjectList = m_pGameInstance->Get_GameObjectList(LEVEL_YU, layerName);

    for (auto _object : _copyObjectList) {
        if (_object->Get_isDestroy() == false) {
            if (CBreakableCube* _break = dynamic_cast<CBreakableCube*>(_object)) {
                vector<CCollider_Cube*> _vecCopyCollider = _break->Get_ColliderCube();
                for (auto collider : _vecCopyCollider) {
                    collider->Set_bColliderActive(false);
                }
            }
        }
    }

    // 현재 청크에 있는 안보이는 블럭들은 모두 비활성화

    swprintf(layerName, 100, L"Layer_Chunk%d", m_currentPlayerChunk);
    _copyObjectList = m_pGameInstance->Get_GameObjectList(LEVEL_YU, layerName);
    std::unordered_set<_float3, Float3Hash> blockPositions;

    // 1. 일단 모두 활성화
    for (auto _object : _copyObjectList) {
        if (_object->Get_isDestroy() == false) {
            if (CBreakableCube* _break = dynamic_cast<CBreakableCube*>(_object)) {
                vector<CCollider_Cube*> _vecCopyCollider = _break->Get_ColliderCube();
                vector<_float3> _vecCopyPosition = _break->Get_Positions();

                for (int i = 0; i < _vecCopyCollider.size(); ++i) {
                    _vecCopyCollider[i]->Set_bColliderActive(true);
                    blockPositions.insert(_vecCopyPosition[i]);
                }
            }
        }
    }

    // 2. 블록의 충돌 여부 확인
    for (auto _object : _copyObjectList) {
        if (_object->Get_isDestroy() == false) {
            if (CBreakableCube* _break = dynamic_cast<CBreakableCube*>(_object)) {
                vector<CCollider_Cube*> _vecCopyCollider = _break->Get_ColliderCube();
                vector<_float3> _vecCopyPosition = _break->Get_Positions();

                for (int i = 0; i < _vecCopyCollider.size(); ++i) {
                    _float3 pos = _vecCopyPosition[i];

                    // 6방향 확인
                    static const _float3 offsets[] = {
                        {-1, 0, 0}, {1, 0, 0}, {0, 1, 0},
                        {0, -1, 0}, {0, 0, 1}, {0, 0, -1}
                    };

                    bool isSurrounded = true;
                    for (const auto& offset : offsets) {
                        if (blockPositions.find({ pos.x + offset.x, pos.y + offset.y, pos.z + offset.z }) == blockPositions.end()) {
                            isSurrounded = false;
                            break;
                        }
                    }

                    // 3. 둘러싸여져 있으면 충돌 비활성화
                    if (isSurrounded) {
                        _vecCopyCollider[i]->Set_bColliderActive(false);
                    }
                }
            }
        }
    }
}
*/

void CMCTerrain::CheckColliderActive()
{
    //이전 청크의 활성화된 충돌 블럭들 모두 비활성화
    wchar_t layerName[100];
    swprintf(layerName, 100, L"Layer_Chunk%d", m_prePlayerChunk);

    vector<_float3> blockPositions;

    list<class CGameObject*> _copyObjectList = m_pGameInstance->Get_GameObjectList(LEVEL_YU, layerName);

    for (auto _object : _copyObjectList) {
        if (_object->Get_isDestroy() == false) {
            if (CBreakableCube* _break = dynamic_cast<CBreakableCube*>(_object)) {
                vector<CCollider_Cube*> _vecCopyCollider = _break->Get_ColliderCube();
                vector<_float3> _vecCopyPosition = _break->Get_Positions();
                for (auto collider : _vecCopyCollider) {
                    collider->Set_bColliderActive(false);
                }

                for (auto pos : _vecCopyPosition) {
                    blockPositions.push_back(pos);
                }
            }
        }
    }

    // 플레이어 주변에 있는 블럭들만 활성화
    auto* pSteve = dynamic_cast<CSteve*>(m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_Steve"), 0));
    if (!pSteve) return;

    _float3 playerPos = pSteve->GetPos();
    // 플레이어가 위치한 청크 계산
    int x = static_cast<int>(playerPos.x) / 16;
    int z = static_cast<int>(playerPos.z) / 16;
    int width = static_cast<int>(sqrt(m_iChunkCount));
    int chunk = x + (width * z);

    swprintf(layerName, 100, L"Layer_Chunk%d", m_currentPlayerChunk);
    _copyObjectList = m_pGameInstance->Get_GameObjectList(LEVEL_YU, layerName);
    

    for (auto _object : _copyObjectList) {
        if (_object->Get_isDestroy() == false) {
            if (CBreakableCube* _break = dynamic_cast<CBreakableCube*>(_object)) {
                vector<CCollider_Cube*> _vecCopyCollider = _break->Get_ColliderCube();
                vector<_float3> _vecCopyPosition = _break->Get_Positions();

                for (int i = 0; i < _vecCopyCollider.size(); ++i) {
                    _float3 pos = _vecCopyPosition[i];

                    // 26방향 확인
                    static const _float3 offsets[] = {
                        // 6방향 (기본)
                        {-1,  0,  0}, {1,  0,  0}, {0,  1,  0},
                        { 0, -1,  0}, {0,  0,  1}, {0,  0, -1},

                        // 12 엣지 방향
                        {-1, -1,  0}, {-1,  1,  0}, {1, -1,  0}, {1,  1,  0},
                        {-1,  0, -1}, {-1,  0,  1}, {1,  0, -1}, {1,  0,  1},
                        { 0, -1, -1}, { 0, -1,  1}, {0,  1, -1}, {0,  1,  1},

                        // 8 코너 방향
                        {-1, -1, -1}, {-1, -1,  1}, {-1,  1, -1}, {-1,  1,  1},
                        { 1, -1, -1}, { 1, -1,  1}, { 1,  1, -1}, { 1,  1,  1}
                    };

                    for (const auto& offset : offsets)
                    {
                        // pos + offset 계산
                        _float3 newPos = pos + offset;

                        // playerPos와 newPos 사이의 거리를 계산
                        float distance = sqrtf(powf(newPos.x - playerPos.x, 2) + powf(newPos.y - playerPos.y, 2) + powf(newPos.z - playerPos.z, 2));

                        // 거리가 3.0f 이내일 때 collider 활성화
                        if (distance <= 3.0f)
                        {
                            _vecCopyCollider[i]->Set_bColliderActive(true);
                            break; // 첫 번째로 조건을 만족한 collider만 활성화하고 종료
                        }
                    }

                }
            }
        }
    }
}

void CMCTerrain::SetColliderChunk()
{
    // 이전 프레임에 충돌 매니저에 올라가라고 활성화되어 있던 청크 비활성화
    wchar_t layerName[100];
    swprintf(layerName, 100, L"Layer_Chunk%d", m_prePlayerChunk);
    list<CGameObject*> objlist = m_pGameInstance->Get_GameObjectList(LEVEL_YU, layerName);
    for (auto obj : objlist) {
        if (CBreakableCube* _copy = dynamic_cast<CBreakableCube*>(obj)) {
            _copy->Set_ChunkColliderActive(false);
        }

        if (CTree* _copy = dynamic_cast<CTree*>(obj)) {
            _copy->Get_Leaf()->Set_ChunkColliderActive(false);
            _copy->Get_Wood()->Set_ChunkColliderActive(false);
        }


        if (CBreakableRect* _copy = dynamic_cast<CBreakableRect*>(obj)) {
            _copy->Set_ChunkColliderActive(false);
        }
    }

    // 현재 플레이어 밑에 있는 청크만 충돌 매니저에 올림
    swprintf(layerName, 100, L"Layer_Chunk%d", m_currentPlayerChunk);
    objlist = m_pGameInstance->Get_GameObjectList(LEVEL_YU, layerName);
    for (auto obj : objlist) {
        if (CBreakableCube* _copy = dynamic_cast<CBreakableCube*>(obj)) {
            _copy->Set_ChunkColliderActive(true);
        }

        if (CTree* _copy = dynamic_cast<CTree*>(obj)) {
            _copy->Get_Leaf()->Set_ChunkColliderActive(true);
            _copy->Get_Wood()->Set_ChunkColliderActive(true);
        }

        if (CBreakableRect* _copy = dynamic_cast<CBreakableRect*>(obj)) {
            _copy->Set_ChunkColliderActive(true);
        }
    }
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

void CMCTerrain::GetPlayerChunk()
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

    wchar_t layerName[100];
    swprintf(layerName, 100, L"Layer_Chunk%d", chunk);
    m_pGameInstance->SetLayerRenderActive(LEVEL_YU, layerName, true);
    list<CGameObject*> objlist = m_pGameInstance->Get_GameObjectList(LEVEL_YU, layerName);
    for (CGameObject* pGameObject : objlist)
    {
        if (CBreakableCube* pBreakableCube = dynamic_cast<CBreakableCube*>(pGameObject)) 
        {
            if (pBreakableCube->Get_ChunkColliderActive()) {
                m_pGameInstance->Add_CollisionGroup(COLLISION_BLOCK, pGameObject);
            }
        }
        //else {
        //    //나무
        //    if (CTree* pTree = dynamic_cast<CTree*>(pGameObject)) {
        //        vector<CGameObject*> _copyVec = pTree->Get_WoodInfo();
        //        for (auto copy : _copyVec) {
        //            m_pGameInstance->Add_CollisionGroup(COLLISION_BLOCK, copy);
        //        }
        //        _copyVec = pTree->Get_LeafInfo();
        //        for (auto copy : _copyVec) {
        //            m_pGameInstance->Add_CollisionGroup(COLLISION_BLOCK, copy);
        //        }
        //    }

        //    //잔디 & 꽃
        //    if (CBreakableRect* pBreakableRect = dynamic_cast<CBreakableRect*>(pGameObject)) {
        //        if (pBreakableRect->Get_RenderActive()) {
        //            m_pGameInstance->Add_CollisionGroup(COLLISION_BLOCK, pGameObject);
        //        }
        //    }

        //}
    }
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
