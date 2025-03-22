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

_bool  CCollider_Manager::Collision_with_Group(_uint iGroupIndex, CComponent* pCollider, COLLISION_TYPE eCollisionType, _Out_ _float3* pOutDistance, _Out_ CCollider_Cube::COLLSION_DIR* pOutDir)
{
	CComponent* pMyCollider = { pCollider };
	CComponent* pOtherCollider = { nullptr };

	for (auto& iter : m_pGameObjects[iGroupIndex])
	{
		pOtherCollider = static_cast<CCollider_Cube*>(iter->Find_Component(TEXT("Com_Collider_Cube")));
		//자기 자신과는 충돌 안하도록
		if (pOtherCollider == pMyCollider)
			continue;

		if (nullptr != iter /*&& CGameInstance::Get_Instance()->Is_In_Frustum(iter->Get_Position(), iter->Get_Radius()) == true*/)
		{

			switch (eCollisionType)
			{
			/*case Engine::CCollider_Manager::COLLISION_RECT:
				 DamageOwner = (CCollider_Rect*)pGameObject->Find_Component(TEXT("Com_Collider_Rect"));
				 Target = (CCollider_Rect*)iter->Find_Component(TEXT("Com_Collider_Rect"));
				if (Target == nullptr)
					continue;
				if (true == (dynamic_cast<CCollider_Rect*>(DamageOwner)->Collision_Check((CCollider_Rect*)Target, pOutDistance)))
					return true;
				break;*/
			case Engine::CCollider_Manager::COLLSIION_BOX:
				if (pOtherCollider == nullptr)
					continue;
				if (true == ((dynamic_cast<CCollider_Cube*>(pMyCollider)->Collision_Check((CCollider_Cube*)pOtherCollider, pOutDistance, pOutDir))))
					return true;
				break;
			default:
				break;
			}
			
		}
	}

	return false;
}

_bool CCollider_Manager::Collision_Check_Group_Multi(_uint iGroupIndex, vector<class CGameObject*>& vecDamagedObj, CGameObject * pDamageCauser, COLLISION_TYPE eCollisionType)
{
	/*CComponent* Target = nullptr;
	CComponent* DamageOwner = nullptr;

	for (auto& iter : m_GameObjects[eGroup])
	{
		if (nullptr != iter && CGameInstance::Get_Instance()->Is_In_Frustum(iter->Get_Position(), iter->Get_Radius()) == true)
		{
			switch (eCollisionType)
			{
			case Engine::CCollider_Manager::COLLISION_RECT:
				DamageOwner = (CCollider_Rect*)pDamageCauser->Find_Component(TEXT("Com_Collider_Rect"));
				Target = (CCollider_Rect*)iter->Find_Component(TEXT("Com_Collider_Rect"));
				if (Target == nullptr)
					continue;

				if (true == (dynamic_cast<CCollider_Rect*>(DamageOwner)->Collision_Check((CCollider_Rect*)Target)))
					vecDamagedObj.push_back(iter);
				break;
			case Engine::CCollider_Manager::COLLSIION_BOX:
				DamageOwner = (CCollider_Cube*)pDamageCauser->Find_Component(TEXT("Com_Collider_Cube"));
				Target = (CCollider_Cube*)iter->Find_Component(TEXT("Com_Collider_Cube"));
				if (Target == nullptr)
					continue;

				if (true == (dynamic_cast<CCollider_Cube*>(DamageOwner)->Collision_Check((CCollider_Cube*)Target)))
					vecDamagedObj.push_back(iter);
				break;
			default:
				break;
			}
			
		}
	}
	
	if (vecDamagedObj.size() > 0)
	{
		return true;
	}
	else {
		return false;
	}*/

	return false;
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