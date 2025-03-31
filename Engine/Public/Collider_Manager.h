#pragma once

#include "Base.h"
#include "GameObject.h"
#include "LineManager.h"
#include "Collider.h"
#include "Collider_Cube.h"
#include "Collider_Capsule.h"

BEGIN(Engine)

class CCollider_Manager final : public CBase
{
public:
	enum COLLISION_TYPE { COLLSIION_CUBE, COLLSIION_CAPSULE, COLLSION_END };
private:
	CCollider_Manager(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CCollider_Manager() = default;

public:
	//���� ������Ʈ�� ������ �ִ� ��� �ݶ��̴��� ��� �ϴ� �Լ�
	HRESULT Add_CollisionGroup(_uint iGroupIndex, class CGameObject* pGameObject);

	//�ݶ��̴� �ϳ��� ��� �ϴ� �Լ�
	HRESULT Add_Collider_CollisionGroup(_uint iGroupIndex, class CCollider* pCollider);

	//�ݶ��̴��� �׷쿡�� �����ϴ� �Լ�
	void Out_CollisiomGroup(_uint iGroupIndex, class CGameObject* pGameObject);

	//�ݶ��̴� �׷��� �ʱ�ȭ �ϴ� �Լ�
	HRESULT Reset_ColliderGroup();

	// Ư�� �׷� ������ �浹�� �˻��ϰ�, ù ��°�� ������ �浹 ��ü�� ��ȯ�մϴ�.
	class CGameObject* Collision_Check_with_Group(
		_uint iGroupIndex,									// �˻��� �׷��� �ε���
		CComponent* pCollider,								// �浹�� �˻��� ��� Collider
		COLLISION_TYPE eCollisionType,						// �浹�� �ݶ��̴� ����
		_Out_ _float3* pOutDepth = nullptr,					// (�ɼ�) �浹 ���̸� ��ȯ�� ������
		_Out_ CCollider::COLLISION_DIR* pOutDir = nullptr	// (�ɼ�) �浹 ������ ��ȯ�� ������
	);

	// Ư�� �׷� ������ ���� �浹�� �˻��ϰ�, �浹�� ��ü ����� ��ȯ�մϴ�.
	_bool Collision_Check_Group_Multi(
		_uint iGroupIndex,											// �˻��� �׷��� �ε���
		list<CCollider_Cube::COLLISION_INFO>& CollisionObjects,		// �浹�� ��ü ������ ������ ����Ʈ (���)
		CComponent* pCollider,										// �浹�� �˻��� ��� Collider
		COLLISION_TYPE eCollisionType								// �浹�� �ݶ��̴� ����
	);


	//�Ϲ� ����
	CGameObject* Ray_Cast(
		const _float3& rayOrigin,					//���� ������
		const _float3& rayDir,						//���� ����
		_float fMaxDistanc,							//�ִ� �浹 ����
		_uint iGroupIndex,							//�浹 �� �׷�
		_Out_ _float* pDist							//�浹 �������� �Ÿ�
	);

	//�ν��Ͻ� ������Ʈ ����
	CGameObject* Ray_Cast_InstancedObjects(
		const _float3& rayOrigin,					//���� ������
		const _float3& rayDir,						//���� ����
		_float fMaxDistanc,							//�ִ� �浹 ����
		_uint iGroupIndex,							//�浹 �� �׷�
		_Out_ _float* fDist = nullptr,				//�浹 �������� �Ÿ�
		_Out_ _float3* pOutCollision_Dir = nullptr, //�浹 ����
		_Out_ CComponent** ppOutCollider = nullptr	//�浹�� �ݶ��̴�
	);

	//�ν��Ͻ� ������Ʈ ����
	CGameObject* Ray_Cast_MultiGroup_InstancedObjects(
		const _float3& rayOrigin,					//���� ������
		const _float3& rayDir,						//���� ����
		_float fMaxDistanc,							//�ִ� �浹 ����
		const vector<_uint>& vGroupIndices,			//�浹 �� �׷� ����
		_Out_ _float* pDist = nullptr,				//�浹 �������� �Ÿ�
		_Out_ _float3* pOutCollision_Dir = nullptr,	//�浹 ����
		_Out_ CComponent** ppOutCollider = nullptr	//�浹�� �ݶ��̴�
	);

	void  Render();
private:
	CLineManager*	m_pLineManager{ nullptr };

private:
	_uint						m_iNumGroups = {};
	list<class CCollider*>*		m_pColliders{ nullptr };
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr }; //���� ����� �Ϸ��� ��������

public:
	HRESULT						Initialize(_uint iNumGroups);
	static CCollider_Manager*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint CollisionGroups);
	virtual void				Free() override;
};
END