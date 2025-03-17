#include "MCTerrain.h"
#include "GameInstance.h"

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

    CheckRenderLayerObjects();
    return S_OK;
}

void CMCTerrain::Priority_Update(_float fTimeDelta)
{
}

void CMCTerrain::Update(_float fTimeDelta)
{
}

void CMCTerrain::Late_Update(_float fTimeDelta)
{	
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
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

#pragma region 파일 읽기 with 쓰레드
CRITICAL_SECTION cs;

int CMCTerrain::GetFileCount()
{
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(L"../bin/Resources/DataFiles/BlockDataChunk*.txt", &findFileData);

    if (hFind != INVALID_HANDLE_VALUE)
    {
        do {
            m_iFileCount++;
        } while (FindNextFile(hFind, &findFileData));

        FindClose(hFind);
    }

    return m_iFileCount;
}

struct ThreadParams
{
    int chunkIndex;
    CMCTerrain* pTerrain;
};

DWORD WINAPI ProcessFileReadThread(LPVOID lpParam)
{
    ThreadParams* params = (ThreadParams*)lpParam;
    int chunkIndex = params->chunkIndex;
    CMCTerrain* pTerrain = params->pTerrain;

    if (!pTerrain) return 1;
    CGameInstance* pGameInstance = pTerrain->GetGameInstance();

    wchar_t filename[100];
    swprintf(filename, 100, L"../bin/Resources/DataFiles/BlockDataChunk%d.txt", chunkIndex);

    wchar_t layerName[100];
    swprintf(layerName, 100, L"Layer_Chunk%d", chunkIndex);

    HANDLE hFile = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return 1;
    }

    DWORD dwBytesRead;
    BLOCKDESC eblockData;
    int index = 0;

    while (ReadFile(hFile, &eblockData, sizeof(BLOCKDESC), &dwBytesRead, NULL) && dwBytesRead > 0)
    {
        CBreakableCube* pCube = nullptr;

        EnterCriticalSection(&cs);  // 동기화 시작
        switch (eblockData.eBlockType)
        {
        case GRASSDIRT:
            if (FAILED(pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_GrassDirt"), LEVEL_YU, layerName)))
                return 1;
            pCube = dynamic_cast<CBreakableCube*>(pGameInstance->Get_Object(LEVEL_YU, layerName, index));
            if (pCube) {
                pCube->SetPos(_float3(eblockData.fPosition));
            }
            index++;
            break;
        case DIRT:
            if (FAILED(pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Dirt"), LEVEL_YU, layerName)))
                return 1;
            pCube = dynamic_cast<CBreakableCube*>(pGameInstance->Get_Object(LEVEL_YU, layerName, index));
            if (pCube) {
                pCube->SetPos(_float3(eblockData.fPosition));
            }
            index++;
            break;
        case STONE:
            if (FAILED(pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Stone"), LEVEL_YU, layerName)))
                return 1;
            pCube = dynamic_cast<CBreakableCube*>(pGameInstance->Get_Object(LEVEL_YU, layerName, index));
            if (pCube) {
                pCube->SetPos(_float3(eblockData.fPosition));
            }
            index++;
            break;
        default:
            break;
        }
        LeaveCriticalSection(&cs);  // 동기화 종료
    }
    CloseHandle(hFile);
    return 0;
}

HRESULT CMCTerrain::Ready_Layer_BackGround()
{
    InitializeCriticalSection(&cs); // CRITICAL_SECTION 초기화

    int fileCount = GetFileCount();
    if (fileCount == 0) {
        MSG_BOX("파일이 없습니다");
        return S_OK;
    }

    HANDLE* hThreads = new HANDLE[fileCount];
    ThreadParams* params = new ThreadParams[fileCount];

    for (int i = 0; i < fileCount; ++i)
    {
        params[i].chunkIndex = i;
        params[i].pTerrain = this;

        hThreads[i] = CreateThread(NULL, 0, ProcessFileReadThread, &params[i], 0, NULL);
        if (!hThreads[i]) {
            delete[] hThreads;
            delete[] params;
            return E_FAIL;
        }
    }

    WaitForMultipleObjects(fileCount, hThreads, TRUE, INFINITE);

    for (int i = 0; i < fileCount; ++i)
    {
        CloseHandle(hThreads[i]);
    }

    delete[] hThreads;
    delete[] params;

    DeleteCriticalSection(&cs); // CRITICAL_SECTION 삭제

    MSG_BOX("모든 블록 데이터 처리가 완료되었습니다!");
    return S_OK;
}
#pragma endregion

void CMCTerrain::CheckRenderLayerObjects()
{
    for (int i = 0; i < m_iFileCount; ++i) {
        wchar_t layerName[100];
        swprintf(layerName, 100, L"Layer_Chunk%d", i);

        list<class CGameObject*> _copyObjectList = m_pGameInstance->Get_GameObjectList(LEVEL_YU, layerName);

        for (auto _object : _copyObjectList) {
            if (CBreakableCube* _break = dynamic_cast<CBreakableCube*>(_object)) {
                _float3 pos = _break->GetPos(); // 현재 블록 위치

                // 주변 블록 존재 여부 확인
                bool hasLeft = false, hasRight = false, hasTop = false, hasBottom = false, hasDown = false, hasUp = false;

                for (auto _object2 : _copyObjectList) {
                    if (_object == _object2) continue;

                    if (CBreakableCube* _other = dynamic_cast<CBreakableCube*>(_object2)) {
                        _float3 otherPos = _other->GetPos();

                        if (otherPos.x == pos.x - 1 && otherPos.y == pos.y && otherPos.z == pos.z) hasLeft = true;
                        if (otherPos.x == pos.x + 1 && otherPos.y == pos.y && otherPos.z == pos.z) hasRight = true;
                        if (otherPos.x == pos.x && otherPos.y == pos.y + 1 && otherPos.z == pos.z) hasTop = true;
                        if (otherPos.x == pos.x && otherPos.y == pos.y - 1 && otherPos.z == pos.z) hasBottom = true;
                        if (otherPos.x == pos.x && otherPos.y == pos.y && otherPos.z == pos.z +1) hasUp = true;
                        if (otherPos.x == pos.x && otherPos.y == pos.y && otherPos.z == pos.z -1) hasDown = true;
                    }
                }

                // 위, 아래, 좌, 우에 블록이 없으면 렌더링 활성화
                if (hasLeft && hasRight && hasTop && hasBottom && hasDown && hasUp) {
                    _break->Set_RenderActive(false);
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
