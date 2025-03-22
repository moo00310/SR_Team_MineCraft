#include "..\Public\Collider_Manager.h"
#include "Component.h"
#include "Transform.h"
//#include "Collider_Rect.h"
#include "Collider_Cube.h"
#include "GameInstance.h"

//IMPLEMENT_SINGLETON(CCollider_Manager)

CCollider_Manager::CCollider_Manager(LPDIRECT3DDEVICE9 pGraphic_Device)
	:m_pGraphic_Device{ pGraphic_Device }
{
}

HRESULT CCollider_Manager::Add_CollisionGroup(_uint iGroupIndex, CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_pGameObjects[iGroupIndex].push_back(pGameObject);
	Safe_AddRef(pGameObject);

	return S_OK;
}

void CCollider_Manager::Out_CollisiomGroup(_uint iGroupIndex, CGameObject * pGameObject)
{
		auto iter = m_pGameObjects[iGroupIndex].begin();
		while (iter != m_pGameObjects[iGroupIndex].end())
		{
			if (*iter == pGameObject)
				iter = m_pGameObjects[iGroupIndex].erase(iter);
			else
				++iter;
		}
}


HRESULT CCollider_Manager::Reset_ColliderGroup()
{
	for (_uint i = 0; i < m_iNumGroups; ++i)
	{
		for (auto& pGameObject : m_pGameObjects[i])
		{
			if (nullptr != pGameObject)
			{
				Safe_Release(pGameObject);
			}
		}
		m_pGameObjects[i].clear();
	}

	return S_OK;
}

CGameObject*  CCollider_Manager::Collision_Check_with_Group(_uint iGroupIndex, CComponent* pCollider, COLLISION_TYPE eCollisionType, _Out_ _float3* pOutDepth, _Out_ CCollider_Cube::COLLSION_DIR* pOutDir)
{
	//초기화
	if (pOutDepth) *pOutDepth = { 0.f, 0.f, 0.f };
	if (pOutDir) *pOutDir = CCollider_Cube::COLLSION_DIR::NONE;

	CComponent* pMyCollider = { pCollider };
	CComponent* pOtherCollider = { nullptr };

	for (auto& iter : m_pGameObjects[iGroupIndex])
	{
		pOtherCollider = static_cast<CCollider_Cube*>(iter->Find_Component(TEXT("Com_Collider_Cube")));
		//자기 자신과는 충돌 안하도록
		if (pOtherCollider == pMyCollider)
			continue;

		if (nullptr != iter)
		{
			switch (eCollisionType)
			{
			case Engine::CCollider_Manager::COLLSIION_CUBE:
				if (pOtherCollider == nullptr)
					continue;
				if (true == ((dynamic_cast<CCollider_Cube*>(pMyCollider)->Collision_Check((CCollider_Cube*)pOtherCollider, pOutDepth, pOutDir))))
					return iter;
				break;
			default:
				break;
			}
			
		}
	}

	return nullptr;
}

_bool CCollider_Manager::Collision_Check_Group_Multi(
	_uint iGroupIndex,
	list<CCollider_Cube::COLLISION_INFO>& Collision_Infos,
	CComponent* pCollider,
	COLLISION_TYPE eCollisionType
)
{
	
	_float3 OutDistance = { 0.f, 0.f, 0.f };
	CCollider_Cube::COLLSION_DIR OutDir = CCollider_Cube::COLLSION_DIR::NONE;

	CComponent* pMyCollider = pCollider;
	CComponent* pOtherCollider = nullptr;

	for (auto& iter : m_pGameObjects[iGroupIndex])
	{
		if (nullptr == iter)
			continue;

		pOtherCollider = static_cast<CCollider_Cube*>(iter->Find_Component(TEXT("Com_Collider_Cube")));

		// 자기 자신과의 충돌 방지
		if (pOtherCollider == pMyCollider)
			continue;

		switch (eCollisionType)
		{
		case Engine::CCollider_Manager::COLLSIION_CUBE:
			if (pOtherCollider == nullptr)
				continue;

			if (static_cast<CCollider_Cube*>(pMyCollider)->Collision_Check(
				static_cast<CCollider_Cube*>(pOtherCollider), &OutDistance, &OutDir))
			{
				// 충돌한 오브젝트와 충돌 방향을 저장
				CCollider_Cube::COLLISION_INFO tInfo;
				tInfo.pGameObject = iter;
				tInfo.eCollisionDir = OutDir;  // 충돌 방향 저장
				tInfo.fDistance = OutDistance;  // 충돌 거리 저장

				Collision_Infos.push_back(tInfo); // 벡터에 추가
			}
			break;

		default:
			break;
		}
	}

	return !Collision_Infos.empty();  // 충돌된 오브젝트가 있다면 true
}




CGameObject* CCollider_Manager::Ray_Cast(const _float3& rayOrigin, const _float3& rayDir, _float maxDistance, _uint eGroup, _Out_ _float& fDist)
{
	fDist = 0.f;

	_float3 vNormalRayDir;
	D3DXVec3Normalize(&vNormalRayDir, &rayDir);

	for (auto& iter : m_pGameObjects[eGroup])
	{
		CCollider_Cube* pOtherCollider = static_cast<CCollider_Cube*>(iter->Find_Component(TEXT("Com_Collider_Cube")));
		CCollider_Cube::COLLCUBE_DESC& CubeDesc = pOtherCollider->Get_Desc();
		const _float4x4* pWorldMatrix = CubeDesc.pTransformCom->Get_WorldMatrix();
		const _float3 halfSize = { CubeDesc.fRadiusX, CubeDesc.fRadiusY, CubeDesc.fRadiusZ };

		// OBB의 중심 및 로컬 축 추출
		_float3 obbCenter(pWorldMatrix->_41, pWorldMatrix->_42, pWorldMatrix->_43);
		_float3 axes[3] = {
			_float3(pWorldMatrix->_11, pWorldMatrix->_12, pWorldMatrix->_13),
			_float3(pWorldMatrix->_21, pWorldMatrix->_22, pWorldMatrix->_23),
			_float3(pWorldMatrix->_31, pWorldMatrix->_32, pWorldMatrix->_33)
		};
		D3DXVec3Normalize(&axes[0], &axes[0]);
		D3DXVec3Normalize(&axes[1], &axes[1]);
		D3DXVec3Normalize(&axes[2], &axes[2]);

		// 레이와 OBB의 교차 검사 (Slab 방식)
		fDist = 0.f;
		_float tMax = FLT_MAX;
		_bool hit = true;  // 현재 OBB에 대해 충돌 판정

		for (int i = 0; i < 3; ++i)
		{
			_float3 lValue = obbCenter - rayOrigin;
			_float e = D3DXVec3Dot(&axes[i], &lValue);
			_float f = D3DXVec3Dot(&axes[i], &vNormalRayDir);

			if (fabs(f) > 1e-6f)  // 레이가 해당 축과 평행하지 않은 경우
			{
				_float t1 = (e - halfSize[i]) / f;
				_float t2 = (e + halfSize[i]) / f;
				if (t1 > t2)
					std::swap(t1, t2);
				fDist = max(fDist, t1);
				tMax = min(tMax, t2);

				if (fDist > tMax)  // 슬랩 간에 교차가 없으면 충돌 없음
				{
					hit = false;
					break;
				}
			}
			else // 레이가 해당 축과 평행한 경우
			{
				// 만약 레이 원점이 슬랩 밖에 있다면 충돌 없음
				if (-e - halfSize[i] > 0.0f || -e + halfSize[i] < 0.0f)
				{
					hit = false;
					break;
				}
			}
		}

		// 현재 OBB와 충돌한 경우 거리 제한 추가
		if (hit && fDist <= maxDistance)
		{
			m_pLineManager->Add_Line(rayOrigin, vNormalRayDir, maxDistance, true);
			return iter;
		}
	}
	m_pLineManager->Add_Line(rayOrigin, vNormalRayDir, maxDistance, false);
	return nullptr;
}



void CCollider_Manager::Render()
{
	m_pLineManager->Render_Lines();
}

HRESULT CCollider_Manager::Initialize(_uint iNumGroups)
{
	m_iNumGroups = iNumGroups;

	m_pGameObjects = new list<class CGameObject*>[iNumGroups];

	m_pLineManager = CLineManager::Create(m_pGraphic_Device);
	if (!m_pLineManager)
		return E_FAIL;

	return S_OK;
}

CCollider_Manager* CCollider_Manager::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumGroups)
{
	CCollider_Manager* pInstance = new CCollider_Manager(pGraphic_Device);

	if (FAILED(pInstance->Initialize(iNumGroups)))
	{
		MSG_BOX("Failed to Created : CCollider_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider_Manager::Free()
{
	__super::Free();

	for (size_t i = 0; i < m_iNumGroups; i++)
	{
		for (auto& GameObject : m_pGameObjects[i])
			Safe_Release(GameObject);

		m_pGameObjects[i].clear();
	}

	Safe_Delete_Array(m_pGameObjects);

	Safe_Release(m_pLineManager);
}