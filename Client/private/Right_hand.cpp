#include "Right_hand.h"

CRight_hand::CRight_hand(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device  }
{
}

CRight_hand::CRight_hand(const CRight_hand& Prototype)
    : CGameObject(Prototype)
{
}

HRESULT CRight_hand::Initialize_Prototype()
{
    return E_NOTIMPL;
}

HRESULT CRight_hand::Initialize(void* pArg)
{
    return E_NOTIMPL;
}

void CRight_hand::Priority_Update(_float fTimeDelta)
{
}

void CRight_hand::Update(_float fTimeDelta)
{
}

void CRight_hand::Late_Update(_float fTimeDelta)
{
}

HRESULT CRight_hand::Render()
{
    return E_NOTIMPL;
}
