#include "Camera_Player.h"
#include <DirectXMath.h>
using namespace DirectX;

#include "BreakableCube.h"
#include "BreakableRect.h"

#include "Monster.h"

#include "UI_Mgr.h"
#include "MCTerrain.h"
#include "Furnace.h"
#include "CraftingTableCube.h"
#include "BurnUi.h"
#include "BurnResultUi.h"
#include "FurnaceUi.h"

CCamera_Player::CCamera_Player(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CCamera{ pGraphic_Device }
{
}

CCamera_Player::CCamera_Player(const CCamera_Player& Prototype)
	:CCamera(Prototype)
{
}

HRESULT CCamera_Player::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Player::Initialize(void* pArg)
{
    m_isActiveMouse = false;
    ShowCursor(false);

	/* TransformCom 생성 */
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTerrain = static_cast<CMCTerrain*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Terrain")));
	Safe_AddRef(m_pTerrain);

	// 아규먼트 받기
	CAMERA_PLAYER_DESC Desc{ *static_cast<CAMERA_PLAYER_DESC*>(pArg) };
	m_fMouseSensor = Desc.fMouseSensor;
	if (!Desc.pTarget)
 		return E_FAIL;

    // 플레이어 받기
    m_pPlayer = static_cast<CSteve*>(Desc.pTarget);
    Safe_AddRef(m_pPlayer);

	// 플레이어 트랜스폼 받기
	m_pTarget_Transform_Com = static_cast<CTransform*>(Desc.pTarget->Find_Component(TEXT("Com_Transform")));
    Safe_AddRef(m_pTarget_Transform_Com);
    
    // 플레이어 리지드바디 받기
    m_pTarget_Rigidbody_Com = static_cast<CRigidbody*>(Desc.pTarget->Find_Component(TEXT("Com_Rigidbody")));
    Safe_AddRef(m_pTarget_Rigidbody_Com);

	// 카메라 기본 값 세팅
	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	// 마우스 올드값 세팅
	GetCursorPos(&m_ptOldMousePos);
	ScreenToClient(g_hWnd, &m_ptOldMousePos);

	// 기본 모드를 TPS로 설정
	m_eCameraMode = E_CAMERA_MODE::TPS;

    m_pCrosshair = static_cast<CCrosshair*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Crosshair")));
    Safe_AddRef(m_pCrosshair);
    //m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget_Transform_Com->Get_State(CTransform::STATE_POSITION));

    //m_vCurrentCameraPos = m_pTarget_Transform_Com->Get_State(CTransform::STATE_POSITION);

	return S_OK;
}

void CCamera_Player::Priority_Update(_float fTimeDelta)
{

}

void CCamera_Player::Update(_float fTimeDelta)
{
    Input_Key(fTimeDelta);

    if (m_pGameInstance->Key_Down(VK_F5))
    {
        m_eCameraMode = (m_eCameraMode == E_CAMERA_MODE::FPS) ? E_CAMERA_MODE::TPS : E_CAMERA_MODE::FPS;
        
        //CrossHair UI On/Off
        if (m_eCameraMode == E_CAMERA_MODE::FPS)
        {
            m_pCrosshair->On();
        }
        else
        {
            m_pCrosshair->Off();
        }
    }
}

void CCamera_Player::Late_Update(_float fTimeDelta)
{
    // 목표 FOV 설정
    _float fTargetFov = m_pPlayer->isRun() ? D3DXToRadian(80.f) : D3DXToRadian(60.f);

    // 부드럽게 보간 (Lerp)
    m_fFov = Lerp(m_fFov, fTargetFov, fTimeDelta * 5.f); // 5.f는 속도 조절용

    Follow_Player(fTimeDelta);
    __super::Update_VP_Matrices();
}


HRESULT CCamera_Player::Render()
{
	return S_OK;
}

CTransform* CCamera_Player::GetTransform() const
{
    return m_pTransformCom;
}

void CCamera_Player::Input_Key(_float fTimeDelta)
{
    // 창이 활성화 상태가 아닐 경우 입력을 무시
    if (!(GetForegroundWindow() == g_hWnd))
        return;

    if (!m_isActiveMouse)
    {
        On_MouseMove(fTimeDelta);
    }

	_float3 vHeadPos = m_pTarget_Transform_Com->Get_State(CTransform::STATE_POSITION) + _float3{ 0.f, 1.5f, 0.f };

    m_pPlayer->Set_AttackContinue(false);

    //아이템 버리기
    if (m_pGameInstance->Key_Down('Q'))
    {
        ITEMNAME eCurItem = CUI_Mgr::Get_Instance()->GetItemTypeName();
        //아이템이 없으면 실행 하지마
        if (eCurItem == ITEMNAME_END)
            return;

        CUI_Mgr::Get_Instance()->ItemCount_Update(eCurItem, -1);

        wchar_t layerName[100];
        swprintf(layerName, 100, L"Layer_Chunk%d", 0); //0청크에 넣어도 상관 없을라나~

        //2d 아이템들이면(ㅋㅋ)
        if (eCurItem == ITEMNAME::ITEMNAME_APPLE ||
            eCurItem == ITEMNAME::ITEMNAME_COAL || 
            eCurItem == ITEMNAME::ITEMNAME_DANDELION||
            eCurItem == ITEMNAME::ITEMNAME_GUNPOWDER || 
            eCurItem == ITEMNAME::ITEMNAME_GRASS ||
            eCurItem == ITEMNAME::ITEMNAME_IRON ||
            eCurItem == ITEMNAME::ITEMNAME_REDTULIP ||
            eCurItem == ITEMNAME::ITEMNAME_SEED)
        {
            if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemRect"), LEVEL_YU, layerName)))
                return;

            if (CItemRect* _copy = dynamic_cast<CItemRect*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))) {
                _copy->SetPos(m_pTarget_Transform_Com->Get_State(CTransform::STATE_POSITION) + _float3{ 0.0f, 1.5f, 0.0f });
                _copy->Set_ItemTypeAndBindTexture(eCurItem);
                _copy->Get_Rigidbody()->Set_isKnockBack(false); //이런 내가 싫다
                _copy->Get_Rigidbody()->Set_Velocity(_float3{ 0.f, 0.f, 0.f });//나를 용서하시오
                _copy->Get_Rigidbody()->Knock_back(m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 10.f);
            }
        }
        else
        {
            if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemCube"), LEVEL_YU, layerName)))
                return;

            if (CItemCube* _copy = dynamic_cast<CItemCube*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))) {
                _copy->SetPos(m_pTarget_Transform_Com->Get_State(CTransform::STATE_POSITION) + _float3{ 0.0f, 1.5f, 0.0f });
                _copy->Set_ItemTypeAndBindTexture(eCurItem);
                _copy->Get_Rigidbody()->Set_isKnockBack(false); //이런 내가 싫다
                _copy->Get_Rigidbody()->Set_Velocity(_float3{ 0.f, 0.f, 0.f });//이런 내가 싫다
                _copy->Get_Rigidbody()->Knock_back(m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 10.f);
            }
        }
    }


    list<CCollider*> Colliders;
    if (m_pGameInstance->Key_Pressing(VK_LBUTTON) || m_pGameInstance->Key_Pressing(VK_RBUTTON))
    {
        //충돌체 추가

		_float3 vStevePos = m_pTarget_Transform_Com->Get_State(CTransform::STATE_POSITION) + _float3{ 0.f, 1.5f, 0.f };
		_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		_float3 vSearchPos = vStevePos + vLook * 2.f;

        Colliders = m_pTerrain->Active_Near_Chunk_Colliders(vSearchPos, 8.f);
    }

    if (m_pGameInstance->Key_Pressing(VK_LBUTTON) && !g_bMainInventoryOpen && !g_bFurnaceUiOpen && !g_bMCraftingTableOpen)
    {
        _float fDist;                  // 광선과 오브젝트 간의 거리
        CGameObject* pHitObject;       // 충돌한 오브젝트
        CComponent* pHitComponent;     // 충돌한 컴포넌트 (콜라이더)
        _float3 vDir{};

        vector<_uint> Groups = { COLLISION_NON_PHYSIC_BLOCK, COLLISION_BLOCK };


        // Ray Casting: Instancing된 오브젝트와 충돌 검사
        pHitObject = m_pGameInstance->Ray_Cast_MultiGroup_InstancedObjects(
            vHeadPos, // 시작 위치 (플레이어의 머리 위치)
            m_pTransformCom->Get_State(CTransform::STATE_LOOK), // 시선 방향
            5.f, // 최대 탐색 거리
            Groups, // 충돌 그룹
            &fDist, // 충돌한 거리 저장
            &vDir,  //충돌 방향 저장
            &pHitComponent // 충돌한 콜라이더 저장
        );

        if (pHitObject)
        {
            if (CBreakableRect* pBreakableRect = dynamic_cast<CBreakableRect*>(pHitObject)) {
                // 충돌한 콜라이더를 CCollider_Cube로 형변환
                CCollider_Cube* pCollider_Cube = static_cast<CCollider_Cube*>(pHitComponent);
                if (!pCollider_Cube)
                    return;

                // 충돌한 콜라이더의 위치를 가져와 해당 블록 삭제
                _float3 hitPosition{ pCollider_Cube->Get_Offset() };
              

                if (FAILED(pBreakableRect->Delete_Cube(hitPosition)))
                {
                    MSG_BOX("Delete_Rect: Fail");
                }


                return;
            }

            // 충돌한 오브젝트가 CBreakableCube인지 확인 후 형변환
            if (CBreakableCube* pBreakableCube = dynamic_cast<CBreakableCube*>(pHitObject)) {
                // 충돌한 콜라이더를 CCollider_Cube로 형변환
                CCollider_Cube* pCollider_Cube = static_cast<CCollider_Cube*>(pHitComponent);
                if (!pCollider_Cube)
                    return;

                m_pPlayer->Set_AttackContinue(true);

                // 충돌한 콜라이더의 위치를 가져와 해당 블록 삭제
                _float3 hitPosition{ pCollider_Cube->Get_Offset()};

                //pCollider_Cube->Render_Collider(true); 안돼네
                
                // 깨지는 블럭 활성화.
                m_DestroyCube->SetActive(true);

                // 깨지는 블럭 위치 이동.
                m_DestroyCube->GetTransform()->Set_State(CTransform::STATE_POSITION, hitPosition);

                // hp 감소.
                pBreakableCube->Attacked_Block(hitPosition,1, fTimeDelta);


                // 꺠지는 블럭 hp 값 넘겨줌.
                m_DestroyCube->SetTextureIndex(static_cast<_int>(pBreakableCube->GetHP()));

                return;
            }
        }
    }

    //몬스터는 keydown
    if (m_pGameInstance->Key_Down(VK_LBUTTON)) {
        _float fDist;                  // 광선과 오브젝트 간의 거리
        CGameObject* pHitObject;       // 충돌한 오브젝트
        CComponent* pHitComponent;     // 충돌한 컴포넌트 (콜라이더)
        _float3 vDir{};

        vector<_uint> Groups = { COLLISION_NON_PHYSIC_BLOCK, COLLISION_BLOCK };

        // 몬스터와 충돌 검사
        pHitObject = m_pGameInstance->Ray_Cast(
            vHeadPos, // 시작 위치 (플레이어의 머리 위치)
            m_pTransformCom->Get_State(CTransform::STATE_LOOK), // 시선 방향
            5.f, // 최대 탐색 거리
            COLLISION_MONSTER, // 충돌 그룹
            &fDist); //거리저장

        if (pHitObject)
        {
            if (CMonster* monster = dynamic_cast<CMonster*>(pHitObject))
            {
                monster->Knock_back(m_pTransformCom->Get_State(CTransform::STATE_LOOK));
                monster->Add_Hp(-10.f);
                return;
            }

        }
    }

    // 이 조건문이 있는 이유는 깨지는 블럭이 캐는 중에 취소하면
    // 비활성화 시켜주려고 넣음.
    if (m_pGameInstance->Key_Up(VK_LBUTTON))
    {
        m_DestroyCube->SetActive(false);
    }

    if (m_pGameInstance->Key_Down(VK_RBUTTON) && !g_bMainInventoryOpen && !g_bFurnaceUiOpen && !g_bMCraftingTableOpen)
    {
        ITEMNAME eCurItem = CUI_Mgr::Get_Instance()->GetItemTypeName();

        _float fDist;                  // 광선과 오브젝트 간의 거리
        CGameObject* pHitObject;       // 충돌한 오브젝트
        CComponent* pHitComponent;     // 충돌한 컴포넌트 (콜라이더)
        _float3 vDir{};

        // Ray Casting: Instancing된 오브젝트와 충돌 검사
        pHitObject = m_pGameInstance->Ray_Cast_InstancedObjects(
            vHeadPos, // 시작 위치 (카메라 또는 플레이어의 머리 위치)
            m_pTransformCom->Get_State(CTransform::STATE_LOOK), // 시선 방향
            5.f, // 최대 탐색 거리
            COLLISION_BLOCK, // 충돌 그룹
            &fDist, // 충돌한 거리 저장
            &vDir,  //충돌 방향 저장
            &pHitComponent // 충돌한 콜라이더 저장
        );

        if (pHitObject)
        {
            if (CBreakableRect* pBreakableRect = dynamic_cast<CBreakableRect*>(pHitObject)) {
                pBreakableRect->Destroy();
                return;
            }

            // 충돌한 오브젝트가 CBreakableCube인지 확인 후 형변환
            if (CBreakableCube* pBreakableCube = dynamic_cast<CBreakableCube*>(pHitObject)) {

                if (CFurnace* _furnace = dynamic_cast<CFurnace*>(pHitObject)) {
                    m_isActiveMouse = true;
                    ShowCursor(true);
                    g_bFurnaceUiOpen = true;

                    if (CFurnaceUi* _furnaceUi = dynamic_cast<CFurnaceUi*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_FurnaceUi")))) {
                        _furnaceUi->Set_Furnace(_furnace);

                        list<CGameObject*> _objlist = m_pGameInstance->Get_GameObjectList(LEVEL_YU, TEXT("Layer_FurnaceDetailUi"));
                        for (auto& obj : _objlist) {
                            if (CBurnUi* _burnUi = dynamic_cast<CBurnUi*>(obj)) {
                                _furnaceUi->Set_FurnaceBurnUi(_burnUi);
                            }
                            if (CBurnResultUi* _burnUi = dynamic_cast<CBurnResultUi*>(obj)) {
                                _furnaceUi->Set_FurnaceBurnResultUi(_burnUi);
                            }
                        }
                    }
                    
                    return;
                }

                if (CCraftingTableCube* _craftingTable = dynamic_cast<CCraftingTableCube*>(pHitObject)) {
                    m_isActiveMouse = true;
                    ShowCursor(true);
                    g_bMCraftingTableOpen = true;
                    return;
                }


                // 충돌한 콜라이더를 CCollider_Cube로 형변환
                CCollider_Cube* pCollider_Cube = static_cast<CCollider_Cube*>(pHitComponent);
                if (!pCollider_Cube)
                    return;

                //콜라이더 큐브의 위치 + 방향을 계산해서
                //Create_Cube 함수 실행

                _float3 vCreatePos = pCollider_Cube->Get_Offset() + pBreakableCube->GetPos();
                m_pTerrain->Create_Cube(m_pTarget_Transform_Com->Get_State(CTransform::STATE_POSITION), eCurItem, vCreatePos, vDir);

                CUI_Mgr::Get_Instance()->ItemCount_Update(eCurItem, -1);

                //이놈이 실행하는게아니라
                //활성화 된 청크에 있는 Breakable큐브 중 eHandBlock과 같은 타입의 큐브에서 Create 큐브를 호출해야함
                //pBreakableCube->Create_Cube(vPos, vDir);
            }
        }
    }


    if (m_pGameInstance->Key_Down(VK_ESCAPE))
    {
        if (m_isActiveMouse)
        {
            g_bMainInventoryOpen = false; 
            g_bFurnaceUiOpen = false;
            g_bMCraftingTableOpen = false;
            m_isActiveMouse = false;
            CUI_Mgr::Get_Instance()->Get_Item()->Clear_ItemTextRender();

            ShowCursor(false);

            RECT rc;
            GetClientRect(g_hWnd, &rc);
            POINT ptCenter = { rc.right / 2, rc.bottom / 2 };

            // 마우스를 다시 중앙으로 이동
            ClientToScreen(g_hWnd, &ptCenter);
            SetCursorPos(ptCenter.x, ptCenter.y);
        }
        else
        {
            m_isActiveMouse = true;
            ShowCursor(true);
        }
    }

    if (m_pGameInstance->Key_Down('E')) {
        if (g_bFurnaceUiOpen || g_bMCraftingTableOpen)
            return;

        if (!g_bMainInventoryOpen) {
            g_bMainInventoryOpen = true;
            m_isActiveMouse = true;
            ShowCursor(true);
        }
        else {
            g_bMainInventoryOpen = false;
            CUI_Mgr::Get_Instance()->Get_Item()->Clear_ItemTextRender();
            m_isActiveMouse = false;
            ShowCursor(false);

            RECT rc;
            GetClientRect(g_hWnd, &rc);
            POINT ptCenter = { rc.right / 2, rc.bottom / 2 };

            // 마우스를 다시 중앙으로 이동
            ClientToScreen(g_hWnd, &ptCenter);
            SetCursorPos(ptCenter.x, ptCenter.y);
        }
    }

    //충돌체 해제
    for (auto pCollider : Colliders)
    {
        m_pGameInstance->Out_Collider_CollisiomGroup(COLLISION_BLOCK, pCollider);
    }
}

void CCamera_Player::Follow_Player(_float fTimeDelta)
{
    if (!m_pTarget_Transform_Com)
        return;

    // === 플레이어의 회전 각도 가져오기 ===
    _float3 vLook = m_pTarget_Transform_Com->Get_State(CTransform::STATE_LOOK);
    m_fYaw = atan2f(vLook.x, vLook.z);

    // === 카메라 회전 벡터 생성 ===
    _float3 vLookDir = {
        cosf(m_fPitch) * sinf(m_fYaw),
        sinf(m_fPitch),
        cosf(m_fPitch) * cosf(m_fYaw)
    };

    // === 걷는 애니메이션 타이머 ===
    _float3 vVelocity = m_pTarget_Rigidbody_Com->Get_Velocity();
    _float fSpeed = sqrtf(vVelocity.x * vVelocity.x + vVelocity.z * vVelocity.z);
    m_fWalkTime += 2.f * fSpeed * fTimeDelta;
    if (m_fWalkTime > D3DX_PI * 2.f)
        m_fWalkTime -= D3DX_PI * 2.f;


    // === 좌우 & 위아래 흔들림 계산 ===
    _float fShakeOffset_X{};
    _float fShakeOffset_Y{};
	if (m_eCameraMode == E_CAMERA_MODE::FPS)
	{
        fShakeOffset_X = cosf(m_fWalkTime) * 0.01f;
        fShakeOffset_Y = fabs(sinf(m_fWalkTime) * 0.01f);
	}
    else if (m_eCameraMode == E_CAMERA_MODE::TPS)
    {
		fShakeOffset_X = cosf(m_fWalkTime) * 0.05f;
		fShakeOffset_Y = fabs(sinf(m_fWalkTime) * 0.05f);
    }

    // === 플레이어의 머리 위치 설정 ===
    _float3 vRight = { cosf(m_fYaw), 0.f, -sinf(m_fYaw) };
    _float3 playerPos = m_pTarget_Transform_Com->Get_State(CTransform::STATE_POSITION);
    _float headHeight = 1.5f;

    // === 카메라 위치 설정 ===
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, playerPos + _float3(0.f, headHeight, 0.f) + vRight * fShakeOffset_X + _float3(0.f, fShakeOffset_Y, 0.f));

	list<CCollider*> Colliders;

    // === 카메라 모드에 따라 다른 처리 ===
    if (m_eCameraMode == E_CAMERA_MODE::FPS)
    {
        m_pTransformCom->LookAt(playerPos + _float3(0.f, headHeight, 0.f) + /*vRight * fShakeOffset_X + _float3(0.f, fShakeOffset_Y, 0.f)*/ + vLookDir);
    }
    else if (m_eCameraMode == E_CAMERA_MODE::TPS)
    {
        //충돌체 추가
        _float3 vStevePos = m_pTarget_Transform_Com->Get_State(CTransform::STATE_POSITION) + _float3{ 0.f, 1.5f, 0.f };
        //_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
        _float3 vMidPos = (m_pTransformCom->Get_State(CTransform::STATE_POSITION) + vStevePos) * 0.5f;
        Colliders = m_pTerrain->Active_Near_Chunk_Colliders(vMidPos, m_fSpringArmLength * m_fSpringArmLength);

        // === 3인칭 스프링 암 거리 조절 ===
        _float fTargetDist{};
        CGameObject* pGameObject = m_pGameInstance->Ray_Cast_InstancedObjects(m_pTransformCom->Get_State(CTransform::STATE_POSITION), -vLookDir, m_fSpringArmLength, COLLISION_BLOCK, &fTargetDist);

        // Ray_Cast 결과 확인
        if (pGameObject)
        {
            fTargetDist = clamp(fTargetDist, 0.5f, m_fSpringArmLength);
        }
        else
        {
            // 충돌이 없을 경우, 기본 거리 사용
            fTargetDist = m_fSpringArmLength;
        }

        // === 최종 카메라 위치 계산 ===
        _float3 vFinalCameraPos = playerPos + _float3(0.f, headHeight, 0.f) + vRight * fShakeOffset_X + _float3(0.f, fShakeOffset_Y, 0.f) - (vLookDir * fTargetDist);

        // === 카메라 위치 바로 적용 ===
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, vFinalCameraPos);
        m_pTransformCom->LookAt(playerPos + _float3(0.f, headHeight, 0.f) + vRight * fShakeOffset_X + _float3(0.f, fShakeOffset_Y, 0.f));

        //충돌체 해제
        for (auto pCollider : Colliders)
        {
            m_pGameInstance->Out_Collider_CollisiomGroup(COLLISION_BLOCK, pCollider);
        }
    }
}


void CCamera_Player::On_MouseMove(_float fTimeDelta)
{
    // === 화면 중앙 좌표 계산 ===
    RECT rc;
    GetClientRect(g_hWnd, &rc);
    POINT ptCenter = { rc.right / 2, rc.bottom / 2 };

    // 현재 마우스 좌표 가져오기
    POINT ptMouse;
    GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse);

    // 마우스 이동량 계산 (중앙 기준)
    _int iMouseMoveY = ptMouse.y - ptCenter.y;
    _int iMouseMoveX = ptMouse.x - ptCenter.x;

    m_pTarget_Transform_Com->Turn({ 0.f, 1.f, 0.f }, fTimeDelta * iMouseMoveX * 0.05f);

    // Pitch 값 업데이트 (상하 회전)
    m_fPitch -= iMouseMoveY * fTimeDelta * m_fMouseSensor;
    m_fPitch = max(-XM_PIDIV2 + 0.1f, min(XM_PIDIV2 - 0.1f, m_fPitch)); // 상하 회전 제한

    // 마우스를 다시 중앙으로 이동
    ClientToScreen(g_hWnd, &ptCenter);
    SetCursorPos(ptCenter.x, ptCenter.y);

}

HRESULT CCamera_Player::Ready_Components()
{
	CTransform::TRANSFORM_DESC TransformDesc{};
	TransformDesc.fSpeedPerSec = 30.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(180.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

    // 깨지는 블럭 가져오고.
    m_DestroyCube = (CDestroyCube*)m_pGameInstance->Get_Object(
        LEVEL_YU,
        LAYER_DESTROY_CUBE.c_str(),
        0
        );

    // 비활성화.
    m_DestroyCube->SetActive(false);

	return S_OK;
}

CCamera_Player* CCamera_Player::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Player* pInstance = new CCamera_Player(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Player::Clone(void* pArg)
{
	CCamera_Player* pInstance = new CCamera_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCamera_Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Player::Free()
{
	__super::Free();

    Safe_Release(m_pCrosshair);
	Safe_Release(m_pTerrain);
    Safe_Release(m_pPlayer);
    Safe_Release(m_pTarget_Transform_Com);
    Safe_Release(m_pTarget_Rigidbody_Com);
}
