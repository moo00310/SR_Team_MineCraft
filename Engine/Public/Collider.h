#pragma once
#include "Component.h"

class CCollider abstract : public CComponent
{
public:
    enum class COLLISION_DIR { NONE, LEFT, RIGHT, UP, DOWN, FRONT, BACK };

protected:
	CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;

public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

