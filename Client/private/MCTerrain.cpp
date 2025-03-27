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
    //CheckRenderLayerObjects();
    return S_OK;
}

void CMCTerrain::Priority_Update(_float fTimeDelta)
{
    //OffAllChunkLayer();
    GetPlayerChunk();
}

void CMCTerrain::Update(_float fTimeDelta)
{
    bool currF1State = (GetAsyncKeyState(VK_F1) & 0x8000) != 0;
    bool currF2State = (GetAsyncKeyState(VK_F2) & 0x8000) != 0;

    if (currF1State && !prevF1State)
    {
        CheckRenderLayerObjects();
    }

    if (currF2State && !prevF2State)
    {
        RenderWithoutStone();
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
        int index = 0;

        vector<D3DXVECTOR3> stoneVec;
        vector<D3DXVECTOR3> dirtVec;
        vector<D3DXVECTOR3> grassVec;
        vector<D3DXVECTOR3> ironVec;
        vector<D3DXVECTOR3> coalVec;

        while (ReadFile(hFile, &eblockData, sizeof(BLOCKDESC), &dwBytesRead, NULL) && dwBytesRead > 0)
        {

            switch (eblockData.eBlockType)
            {
            case GRASSDIRT:
                grassVec.push_back(eblockData.fPosition);
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
            //{TEXT("Prototype_GameObject_Dirt"), &dirtVec},
            //{TEXT("Prototype_GameObject_Stone"), &stoneVec},
            //{TEXT("Prototype_GameObject_IronOre"), &ironVec},
            //{TEXT("Prototype_GameObject_CoalOre"), &coalVec}
            //{TEXT("Prototype_GameObject_CoalOre"), &coalVec}
        };


        for (size_t i = 0; i < blockTypes.size(); ++i)
        {
            m_pGameInstance->Add_GameObject(LEVEL_YU, blockTypes[i].first, LEVEL_YU, layerName);
            CBreakableCube* pCube = dynamic_cast<CBreakableCube*>(m_pGameInstance->Get_Object(LEVEL_YU, layerName, static_cast<int>(i)));
            if (pCube) {
                pCube->Set_InstanceBuffer(*(blockTypes[i].second));
                pCube->Set_MyChunk(i);
                pCube->Set_BlockPositions(*(blockTypes[i].second));
            }
                
        }



    }

    MSG_BOX("모든 블록 데이터 처리가 완료되었습니다!");
    return S_OK;
}

#include <unordered_set>

struct Float3Hash {
    size_t operator()(const _float3& v) const {
        size_t hx = std::hash<float>()(v.x);
        size_t hy = std::hash<float>()(v.y);
        size_t hz = std::hash<float>()(v.z);
        return hx ^ (hy << 1) ^ (hz << 2);  // XOR을 이용한 해싱
    }
};

void CMCTerrain::CheckRenderLayerObjects()
{

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

    list<class CGameObject*> _copyObjectList = m_pGameInstance->Get_GameObjectList(LEVEL_YU, layerName);

    // 블록 위치 정보를 저장할 unordered_set
    std::unordered_set<_float3, Float3Hash> blockPositions;

    // 먼저 모든 블록의 위치를 저장
    for (auto _object : _copyObjectList) {
        if (_object->Get_isDestroy() == false) {
            if (CBreakableCube* _break = dynamic_cast<CBreakableCube*>(_object)) {
                _break->Set_RenderActive(true);
                blockPositions.insert(_break->GetPos());
            }
        }
    }

    // 블록의 렌더링 여부 확인
    for (auto _object : _copyObjectList) {
        if (CBreakableCube* _break = dynamic_cast<CBreakableCube*>(_object)) {
            _float3 pos = _break->GetPos();

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

            if (isSurrounded) {
                _break->Set_RenderActive(false);
            }
        }
    }
}

void CMCTerrain::RenderWithoutStone()
{
    for (int i = 0; i < m_iChunkCount; ++i) {
        wchar_t layerName[100];
        swprintf(layerName, 100, L"Layer_Chunk%d", i);

        list<class CGameObject*> _copyObjectList = m_pGameInstance->Get_GameObjectList(LEVEL_YU, layerName);

        for (auto _object : _copyObjectList) {
            if (CBreakableCube* _break = dynamic_cast<CBreakableCube*>(_object)) {
                _break->Set_RenderActive(true);
                if (dynamic_cast<CStone*>(_object)) {
                    _break->Set_RenderActive(false);
                }
            }
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
                if (pBreakableCube->Get_RenderActive()) {
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
            if (pBreakableCube->Get_RenderActive()) {
                m_pGameInstance->Add_CollisionGroup(COLLISION_BLOCK, pGameObject);
            }
        }
        else {
            //나무
            if (CTree* pTree = dynamic_cast<CTree*>(pGameObject)) {
                vector<CGameObject*> _copyVec = pTree->Get_WoodInfo();
                for (auto copy : _copyVec) {
                    m_pGameInstance->Add_CollisionGroup(COLLISION_BLOCK, copy);
                }
                _copyVec = pTree->Get_LeafInfo();
                for (auto copy : _copyVec) {
                    m_pGameInstance->Add_CollisionGroup(COLLISION_BLOCK, copy);
                }
            }

            //잔디 & 꽃
            if (CBreakableRect* pBreakableRect = dynamic_cast<CBreakableRect*>(pGameObject)) {
                if (pBreakableRect->Get_RenderActive()) {
                    m_pGameInstance->Add_CollisionGroup(COLLISION_BLOCK, pGameObject);
                }
            }

        }
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
