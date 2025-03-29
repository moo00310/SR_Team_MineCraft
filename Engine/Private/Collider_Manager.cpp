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
	//�ʱ�ȭ
	if (pOutDepth) *pOutDepth = { 0.f, 0.f, 0.f };
	if (pOutDir) *pOutDir = CCollider::COLLISION_DIR::NONE;

	CComponent* pMyCollider = { pCollider };
	CComponent* pOtherCollider = { nullptr };

	for (auto& iter : m_pColliders[iGroupIndex])
	{
		pOtherCollider = iter;
		//�ڱ� �ڽŰ��� �浹 ���ϵ���
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

	return !Collision_Infos.empty();  // �浹�� ������Ʈ�� �ִٸ� true
}




CGameObject* CCollider_Manager::Ray_Cast(const _float3& rayOrigin, const _float3& rayDir, _float maxDistance, _uint eGroup, _Out_ _float& fDist)
{
	fDist = 0.f;

	// ���� ���� ���� ����ȭ
	_float3 vNormalRayDir;
	D3DXVec3Normalize(&vNormalRayDir, &rayDir);

	CGameObject* closestObject = nullptr;
	_float closestDist = FLT_MAX;

	// �ش� �׷쿡 ���� ��� ������Ʈ �˻�
	for (auto& iter : m_pColliders[eGroup])
	{
		CCollider_Cube* pOtherCollider = static_cast<CCollider_Cube*>(iter);
		if (nullptr == pOtherCollider)
			continue;

		//CCollider_Cube::COLLCUBE_DESC& CubeDesc = pOtherCollider->Get_Desc();
		const _float4x4* pWorldMatrix = pOtherCollider->Get_Transform()->Get_WorldMatrix();
		const _float3 halfSize = pOtherCollider->Get_Radius();

		// OBB �߽� �� ���� �� ����
		_float3 obbCenter(pWorldMatrix->_41, pWorldMatrix->_42, pWorldMatrix->_43);
		_float3 axes[3] = {
			_float3(pWorldMatrix->_11, pWorldMatrix->_12, pWorldMatrix->_13),
			_float3(pWorldMatrix->_21, pWorldMatrix->_22, pWorldMatrix->_23),
			_float3(pWorldMatrix->_31, pWorldMatrix->_32, pWorldMatrix->_33)
		};

		// �� �� ����ȭ
		D3DXVec3Normalize(&axes[0], &axes[0]);
		D3DXVec3Normalize(&axes[1], &axes[1]);
		D3DXVec3Normalize(&axes[2], &axes[2]);

		// ���̿� OBB�� ���� �˻� (Slab ���)
		_float tMin = 0.f;
		_float tMax = FLT_MAX;
		_bool hit = true; // OBB���� �浹 ����

		for (int i = 0; i < 3; ++i)
		{
			_float3 lValue = obbCenter - rayOrigin;
			_float e = D3DXVec3Dot(&axes[i], &lValue);
			_float f = D3DXVec3Dot(&axes[i], &vNormalRayDir);

			if (fabs(f) > 1e-5f) // ���̰� ��� �������� �ʴٸ�
			{
				_float t1 = (e - halfSize[i]) / f;
				_float t2 = (e + halfSize[i]) / f;

				if (t1 > t2)
					std::swap(t1, t2);

				tMin = max(tMin, t1);
				tMax = min(tMax, t2);

				if (tMin > tMax) // ���� ����
				{
					hit = false;
					break;
				}
			}
			else // ���̰� ��� ������ ���
			{
				if (-e - halfSize[i] > 0.0f || -e + halfSize[i] < 0.0f)
				{
					hit = false;
					break;
				}
			}
		}

		// ���� ����� �浹 ���� ã��
		if (hit && tMin <= maxDistance && tMin < closestDist)
		{
			closestDist = tMin;
			closestObject = iter->Get_Owner();
		}
	}

	// �浹�� ���
	if (closestObject)
	{
		fDist = closestDist;
		m_pLineManager->Add_Line(rayOrigin, vNormalRayDir, maxDistance, true);
		return closestObject;
	}

	// �浹���� ���� ���
	m_pLineManager->Add_Line(rayOrigin, vNormalRayDir, maxDistance, false);
	return nullptr;
}


CGameObject* CCollider_Manager::Ray_Cast_InstancingObject(const _float3& rayOrigin, const _float3& rayDir, _float fMaxDistanc, _uint iGroupIndex, _Out_ _float* pDist, _Out_ _float3* pOutCollision_Dir, _Out_ CComponent** ppOutCollider)
{
	//CCollider::COLLISION_DIR 6������
	//���⼭ �����ɽ�Ʈ�� �΋H�� �ݶ��̴��� ������ �˰� ����

	// �ʱ�ȭ
	if (ppOutCollider)
		*ppOutCollider = nullptr;

	// ���� ������ ����ȭ
	_float3 vNormalRayDir;
	D3DXVec3Normalize(&vNormalRayDir, &rayDir);

	CGameObject* closestObject = nullptr;
	_float closestDist = FLT_MAX;
	CComponent* closestCollider = nullptr;

	// Ư�� �׷��� ��� ������Ʈ �˻�
	for (auto& iter : m_pColliders[iGroupIndex])
	{
		CCollider_Cube* pOtherCollider = static_cast<CCollider_Cube*>(iter);
		if (nullptr == pOtherCollider)
			continue;

		if (pOtherCollider->Get_bColliderActive() == false)
			continue;

		//CCollider_Cube::COLLCUBE_DESC& CubeDesc = pOtherCollider->Get_Desc();

		// AABB�� �ּ�, �ִ� ��ǥ ���
		_float3 minBound{ pOtherCollider->Get_Offset() - pOtherCollider->Get_Radius() };
		_float3 maxBound{ pOtherCollider->Get_Offset() + pOtherCollider->Get_Radius() };

		// ���̿� AABB�� �浹 �˻�
		_float tMin = 0.f, tMax = fMaxDistanc;
		_bool hit = true;

		for (int i = 0; i < 3; ++i)
		{
			if (fabs(vNormalRayDir[i]) < 1e-5f) // ���̰� �ش� ��� ���� ������ ���
			{
				if (rayOrigin[i] < minBound[i] || rayOrigin[i] > maxBound[i])
				{
					hit = false;
					break;
				}
			}
			else // �Ϲ����� ���
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

		// ���� ����� �浹 ������Ʈ ã��
		if (hit && tMin < closestDist)
		{
			closestDist = tMin;
			closestObject = iter->Get_Owner();
			closestCollider = iter;

			if (pOutCollision_Dir)
			{
				// �浹�� ���� ���� ���
				_float3 hitNormal = { 0.f, 0.f, 0.f };
				for (int i = 0; i < 3; ++i)
				{
					_float t1 = (minBound[i] - rayOrigin[i]) / vNormalRayDir[i];
					_float t2 = (maxBound[i] - rayOrigin[i]) / vNormalRayDir[i];

					if (fabs(tMin - t1) < 1e-5f)  // tMin�� t1���� ������ ���
						hitNormal[i] = -1.f;
					else if (fabs(tMin - t2) < 1e-5f)  // tMin�� t2���� ������ ���
						hitNormal[i] = 1.f;
				}

				*pOutCollision_Dir = hitNormal;
			}


		}
	}

	// �浹�� ��� ��ȯ
	if (closestObject)
	{
		if(pDist)
		*pDist = closestDist;
		if (ppOutCollider)
			*ppOutCollider = closestCollider;
		m_pLineManager->Add_Line(rayOrigin, vNormalRayDir, fMaxDistanc, true);
		return closestObject;
	}

	// �浹�� ���� ���
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