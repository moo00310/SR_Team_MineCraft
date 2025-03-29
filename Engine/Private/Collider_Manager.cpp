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

	list<CComponent*> Colliders = pGameObject->Find_Component_All(TEXT("Com_Collider_Cube"));

	for(CComponent* pCollider : Colliders)
	{
		m_pColliders[iGroupIndex].push_back(static_cast<CCollider*>(pCollider));
		Safe_AddRef(pCollider);
	}



	return S_OK;
}

HRESULT CCollider_Manager::Add_Collider_CollisionGroup(_uint iGroupIndex, CCollider* pCollider)
{
	if (nullptr == pCollider)
		return E_FAIL;

	m_pColliders[iGroupIndex].push_back(pCollider);
	Safe_AddRef(pCollider);

	return S_OK;
}

void CCollider_Manager::Out_CollisiomGroup(_uint iGroupIndex, CGameObject * pGameObject)
{
	/*auto iter = m_pColliders[iGroupIndex].begin();
	while (iter != m_pColliders[iGroupIndex].end())
	{
		if (*iter == pGameObject)
			iter = m_pColliders[iGroupIndex].erase(iter);
		else
			++iter;
	}*/
}


HRESULT CCollider_Manager::Reset_ColliderGroup()
{
	for (_uint i = 0; i < m_iNumGroups; ++i)
	{
		for (auto& pCollider : m_pColliders[i])
		{
			if (nullptr != pCollider)
			{
				Safe_Release(pCollider);
			}
		}
		m_pColliders[i].clear();
	}

	return S_OK;
}

CGameObject*  CCollider_Manager::Collision_Check_with_Group(_uint iGroupIndex, CComponent* pCollider, COLLISION_TYPE eCollisionType, _Out_ _float3* pOutDepth, _Out_ CCollider_Cube::COLLISION_DIR* pOutDir)
{
	//초기화
	if (pOutDepth) *pOutDepth = { 0.f, 0.f, 0.f };
	if (pOutDir) *pOutDir = CCollider::COLLISION_DIR::NONE;

	CComponent* pMyCollider = { pCollider };
	CComponent* pOtherCollider = { nullptr };

	for (auto& iter : m_pColliders[iGroupIndex])
	{
		pOtherCollider = iter;
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
					return iter->Get_Owner();
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
	COLLISION_TYPE eOther_CollisionType
)
{
	
	_float3 OutDistance = { 0.f, 0.f, 0.f };
	CCollider::COLLISION_DIR OutDir = CCollider::COLLISION_DIR::NONE;

	CComponent* pMyCollider = pCollider;

	for (auto& iter : m_pColliders[iGroupIndex])
	{
		if (nullptr == iter)
			continue;

		if (iter == nullptr)
			continue;
		if (iter == pMyCollider)
			continue;

		switch (eOther_CollisionType)
		{
		case CCollider_Manager::COLLSIION_CUBE:
			/*if (static_cast<CCollider_Cube*>(pCollider)->Get_bColliderActive())
			{
				
			}*/

			if (static_cast<CCollider*>(pMyCollider)->Collision_Check(
				static_cast<CCollider_Cube*>(iter), &OutDistance, &OutDir))
			{
				CCollider_Cube::COLLISION_INFO tInfo;
				tInfo.pGameObject = iter->Get_Owner();
				tInfo.eCollisionDir = OutDir;
				tInfo.vDepth = OutDistance;
				Collision_Infos.push_back(tInfo);
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

	// 레이 방향 벡터 정규화
	_float3 vNormalRayDir;
	D3DXVec3Normalize(&vNormalRayDir, &rayDir);

	CGameObject* closestObject = nullptr;
	_float closestDist = FLT_MAX;

	// 해당 그룹에 속한 모든 오브젝트 검사
	for (auto& iter : m_pColliders[eGroup])
	{
		CCollider_Cube* pOtherCollider = static_cast<CCollider_Cube*>(iter);
		if (nullptr == pOtherCollider)
			continue;

		//CCollider_Cube::COLLCUBE_DESC& CubeDesc = pOtherCollider->Get_Desc();
		const _float4x4* pWorldMatrix = pOtherCollider->Get_Transform()->Get_WorldMatrix();
		const _float3 halfSize = pOtherCollider->Get_Radius();

		// OBB 중심 및 로컬 축 추출
		_float3 obbCenter(pWorldMatrix->_41, pWorldMatrix->_42, pWorldMatrix->_43);
		_float3 axes[3] = {
			_float3(pWorldMatrix->_11, pWorldMatrix->_12, pWorldMatrix->_13),
			_float3(pWorldMatrix->_21, pWorldMatrix->_22, pWorldMatrix->_23),
			_float3(pWorldMatrix->_31, pWorldMatrix->_32, pWorldMatrix->_33)
		};

		// 각 축 정규화
		D3DXVec3Normalize(&axes[0], &axes[0]);
		D3DXVec3Normalize(&axes[1], &axes[1]);
		D3DXVec3Normalize(&axes[2], &axes[2]);

		// 레이와 OBB의 교차 검사 (Slab 방식)
		_float tMin = 0.f;
		_float tMax = FLT_MAX;
		_bool hit = true; // OBB와의 충돌 여부

		for (int i = 0; i < 3; ++i)
		{
			_float3 lValue = obbCenter - rayOrigin;
			_float e = D3DXVec3Dot(&axes[i], &lValue);
			_float f = D3DXVec3Dot(&axes[i], &vNormalRayDir);

			if (fabs(f) > 1e-5f) // 레이가 축과 평행하지 않다면
			{
				_float t1 = (e - halfSize[i]) / f;
				_float t2 = (e + halfSize[i]) / f;

				if (t1 > t2)
					std::swap(t1, t2);

				tMin = max(tMin, t1);
				tMax = min(tMax, t2);

				if (tMin > tMax) // 교차 없음
				{
					hit = false;
					break;
				}
			}
			else // 레이가 축과 평행한 경우
			{
				if (-e - halfSize[i] > 0.0f || -e + halfSize[i] < 0.0f)
				{
					hit = false;
					break;
				}
			}
		}

		// 가장 가까운 충돌 지점 찾기
		if (hit && tMin <= maxDistance && tMin < closestDist)
		{
			closestDist = tMin;
			closestObject = iter->Get_Owner();
		}
	}

	// 충돌한 경우
	if (closestObject)
	{
		fDist = closestDist;
		m_pLineManager->Add_Line(rayOrigin, vNormalRayDir, maxDistance, true);
		return closestObject;
	}

	// 충돌하지 않은 경우
	m_pLineManager->Add_Line(rayOrigin, vNormalRayDir, maxDistance, false);
	return nullptr;
}


CGameObject* CCollider_Manager::Ray_Cast_InstancingObject(const _float3& rayOrigin, const _float3& rayDir, _float fMaxDistanc, _uint iGroupIndex, _Out_ _float* pDist, _Out_ _float3* pOutCollision_Dir, _Out_ CComponent** ppOutCollider)
{
	//CCollider::COLLISION_DIR 6방향임
	//여기서 레이케스트로 부딫힌 콜라이더의 방향을 알고 싶음

	// 초기화
	if (ppOutCollider)
		*ppOutCollider = nullptr;

	// 레이 방향을 정규화
	_float3 vNormalRayDir;
	D3DXVec3Normalize(&vNormalRayDir, &rayDir);

	CGameObject* closestObject = nullptr;
	_float closestDist = FLT_MAX;
	CComponent* closestCollider = nullptr;

	// 특정 그룹의 모든 오브젝트 검사
	for (auto& iter : m_pColliders[iGroupIndex])
	{
		CCollider_Cube* pOtherCollider = static_cast<CCollider_Cube*>(iter);
		if (nullptr == pOtherCollider)
			continue;

		if (pOtherCollider->Get_bColliderActive() == false)
			continue;

		//CCollider_Cube::COLLCUBE_DESC& CubeDesc = pOtherCollider->Get_Desc();

		// AABB의 최소, 최대 좌표 계산
		_float3 minBound{ pOtherCollider->Get_Offset() - pOtherCollider->Get_Radius() };
		_float3 maxBound{ pOtherCollider->Get_Offset() + pOtherCollider->Get_Radius() };

		// 레이와 AABB의 충돌 검사
		_float tMin = 0.f, tMax = fMaxDistanc;
		_bool hit = true;

		for (int i = 0; i < 3; ++i)
		{
			if (fabs(vNormalRayDir[i]) < 1e-5f) // 레이가 해당 축과 거의 평행한 경우
			{
				if (rayOrigin[i] < minBound[i] || rayOrigin[i] > maxBound[i])
				{
					hit = false;
					break;
				}
			}
			else // 일반적인 경우
			{
				_float t1 = (minBound[i] - rayOrigin[i]) / vNormalRayDir[i];
				_float t2 = (maxBound[i] - rayOrigin[i]) / vNormalRayDir[i];
				if (t1 > t2) std::swap(t1, t2);
				tMin = max(tMin, t1);
				tMax = min(tMax, t2);
				if (tMin > tMax)
				{
					hit = false;
					break;
				}
			}
		}

		// 가장 가까운 충돌 오브젝트 찾기
		if (hit && tMin < closestDist)
		{
			closestDist = tMin;
			closestObject = iter->Get_Owner();
			closestCollider = iter;

			if (pOutCollision_Dir)
			{
				// 충돌한 면의 방향 계산
				_float3 hitNormal = { 0.f, 0.f, 0.f };
				for (int i = 0; i < 3; ++i)
				{
					_float t1 = (minBound[i] - rayOrigin[i]) / vNormalRayDir[i];
					_float t2 = (maxBound[i] - rayOrigin[i]) / vNormalRayDir[i];

					if (fabs(tMin - t1) < 1e-5f)  // tMin이 t1에서 결정된 경우
						hitNormal[i] = -1.f;
					else if (fabs(tMin - t2) < 1e-5f)  // tMin이 t2에서 결정된 경우
						hitNormal[i] = 1.f;
				}

				*pOutCollision_Dir = hitNormal;
			}


		}
	}

	// 충돌한 경우 반환
	if (closestObject)
	{
		if(pDist)
		*pDist = closestDist;
		if (ppOutCollider)
			*ppOutCollider = closestCollider;
		m_pLineManager->Add_Line(rayOrigin, vNormalRayDir, fMaxDistanc, true);
		return closestObject;
	}

	// 충돌이 없을 경우
	m_pLineManager->Add_Line(rayOrigin, vNormalRayDir, fMaxDistanc, false);

	return nullptr;
}


void CCollider_Manager::Render()
{
	m_pLineManager->Render_Lines();
}

HRESULT CCollider_Manager::Initialize(_uint iNumGroups)
{
	m_iNumGroups = iNumGroups;

	m_pColliders = new list<class CCollider*>[iNumGroups];

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
		for (auto& GameObject : m_pColliders[i])
			Safe_Release(GameObject);

		m_pColliders[i].clear();
	}

	Safe_Delete_Array(m_pColliders);
	Safe_Release(m_pLineManager);
}