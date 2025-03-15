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

HRESULT CCollider_Manager::Add_CollisionGroup(COLLISION_GROUP eCollisionGroup, CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects[eCollisionGroup].push_back(pGameObject);
	Safe_AddRef(pGameObject);

	return S_OK;
}

void CCollider_Manager::Out_CollisiomGroup(COLLISION_GROUP eCollisionGroup, CGameObject * pGameObject)
{
		auto iter = m_GameObjects[eCollisionGroup].begin();
		while (iter != m_GameObjects[eCollisionGroup].end())
		{
			if (*iter == pGameObject)
				iter = m_GameObjects[eCollisionGroup].erase(iter);
			else
				++iter;
		}
}


HRESULT CCollider_Manager::Reset_ColliderGroup()
{
	for (_uint i = 0; i < COLLISION_GROUPEND; ++i)
	{
		for (auto& pGameObject : m_GameObjects[i])
		{
			if (nullptr != pGameObject)
			{
				Safe_Release(pGameObject);
			}
		}
		m_GameObjects[i].clear();
	}

	return S_OK;
}
_bool  CCollider_Manager::Collision_with_Group(COLLISION_GROUP eGroup, class CGameObject* pGameObject, COLLISION_TYPE eCollisionType, _float3* pOutDistance )
{
	CComponent* pOtherCollider = { nullptr };
	CComponent* pMyCollider = { nullptr };
	pMyCollider = pGameObject->Find_Component(TEXT("Com_Collider_Cube"));

	for (auto& iter : m_GameObjects[eGroup])
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
				if (true == ((dynamic_cast<CCollider_Cube*>(pMyCollider)->Collision_Check((CCollider_Cube*)pOtherCollider, pOutDistance))))
					return true;
				break;
			default:
				break;
			}
			
		}
	}

	return false;
}

_bool CCollider_Manager::Collision_Check_Group_Multi(COLLISION_GROUP eGroup, vector<class CGameObject*>& vecDamagedObj, CGameObject * pDamageCauser, COLLISION_TYPE eCollisionType)
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

_bool CCollider_Manager::Ray_Cast(const _float4x4* matWorld, _float3 vOrigin, _float3 vDir, _float fLength, CCollider_Manager::COLLISION_GROUP eGroup)
{
	if (!m_pGraphic_Device) return true;

	// 선의 두 개의 점을 정의 (빨간색 적용)
	VTXPOSCOL vertices[] =
	{
		{ vOrigin, D3DCOLOR_XRGB(255, 0, 0) },
		{ vOrigin + vDir * fLength, D3DCOLOR_XRGB(255, 0, 0) }
	};

	// 월드 행렬 설정
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, matWorld);

	// FVF 설정
	m_pGraphic_Device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

	// 선 그리기
	m_pGraphic_Device->DrawPrimitiveUP(D3DPT_LINELIST, 1, vertices, sizeof(VTXPOSCOL));
	
	return true;
}

bool CCollider_Manager::IntersectRayOBB(const _float3& rayOrigin, const _float3& rayDir, float& tMin, CCollider_Manager::COLLISION_GROUP eGroup)
{
	for (auto& iter : m_GameObjects[eGroup])
	{
		CCollider_Cube* pOtherCollider = static_cast<CCollider_Cube*>(iter->Find_Component(TEXT("Com_Collider_Cube")));
		CCollider_Cube::COLLRECTDESC& CubeDesc = pOtherCollider->Get_Desc();
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
		tMin = 0.0f;
		float tMax = FLT_MAX;

		for (int i = 0; i < 3; ++i)
		{
			_float3 lValue = obbCenter - rayOrigin;
			float e = D3DXVec3Dot(&(axes[i]), &(lValue));
			float f = D3DXVec3Dot(&(axes[i]), &rayDir);

			if (fabs(f) > 1e-6f)  // 레이가 축과 평행하지 않은 경우
			{
				float t1 = (e - halfSize[i]) / f;
				float t2 = (e + halfSize[i]) / f;
				if (t1 > t2) std::swap(t1, t2);
				tMin = max(tMin, t1);
				tMax = min(tMax, t2);

				if (tMin > tMax) // 교차하지 않으면 다음 OBB 검사
					break;
			}
			else if (-e - halfSize[i] > 0.0f || -e + halfSize[i] < 0.0f)
			{
				break;  // 레이가 축과 평행하면서 OBB 밖에 있음
			}
		}

		// OBB와 충돌한 경우 즉시 true 반환
		if (tMin <= tMax)
			return true;
	}

	return false;
}



CCollider_Manager* CCollider_Manager::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	return new CCollider_Manager(pGraphic_Device);
}

void CCollider_Manager::Free()
{
	for (_uint i = 0; i < COLLISION_GROUPEND; ++i)
		m_GameObjects[i].clear();
}