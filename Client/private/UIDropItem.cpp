#include "UIDropItem.h"

CUIDropItem::CUIDropItem(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CUIObject(pGraphic_Device)
{
}

CUIDropItem::CUIDropItem(CUIDropItem& Prototype) :
	CUIObject(Prototype)
{
	Safe_AddRef(m_pTransformCom);
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pVIBufferCom);
}

HRESULT CUIDropItem::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIDropItem::Initialize(void* pArg)
{
	UIOBJECT_DESC Desc{};

	Desc.fSizeX = 50;
	Desc.fSizeY = 50;
	Desc.fX = g_iWinSizeX * 0.5f;
	Desc.fY = g_iWinSizeY * 0.5f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
	{
		return E_FAIL;
	}

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

	return S_OK;
}

void CUIDropItem::Priority_Update(_float fTimeDelta)
{
}

void CUIDropItem::Update(_float fTimeDelta)
{
	_float3 direction = { m_vDirection.x, m_vDirection.y, 0.f};
	m_pTransformCom->Go_Direction(direction, fTimeDelta);

	_float2 distancePosition = m_vTargetPosition - (_float2)m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	float distance = D3DXVec2Length(&distancePosition);

	if (distance < 5.f)
	{
		SetActive(false);
		return;
	}
}

void CUIDropItem::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
	{
		return;
	}		
}

HRESULT CUIDropItem::Render()
{
	if (FAILED(m_pTransformCom->Bind_Resource()))
	{
		return E_FAIL;
	}

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
	{
		return E_FAIL;
	}		

	if (FAILED(m_pTextureCom->Bind_Resource(m_kITEM_NAME)))
	{
		return E_FAIL;
	}	

	__super::Begin();
	
	m_pVIBufferCom->Render();

	__super::End();

	return S_OK;
}

CUIDropItem* CUIDropItem::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUIDropItem* pInstance = new CUIDropItem(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUIDropItem");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUIDropItem::Clone(void* pArg)
{
	CUIDropItem* pInstance = new CUIDropItem(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CUIDropItem");
		Safe_Release(pInstance);
	}	

	return pInstance;
}

void CUIDropItem::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}

void CUIDropItem::OnPushPool()
{
}

void CUIDropItem::SetTransform(_float2 _position)
{
	m_fX = _position.x;
	m_fY = _position.y;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
}

void CUIDropItem::SetDirection(_float2 _direction)
{
	D3DXVec2Normalize(&_direction, &_direction);

	m_vDirection = _direction;
}

CTransform* CUIDropItem::GetTransform() const
{
	return m_pTransformCom;
}

void CUIDropItem::SetTargetPosition(_float2 _target)
{
	m_vTargetPosition = _target;
}

void CUIDropItem::SetItemName(ITEMNAME _itemName)
{
	m_kITEM_NAME = _itemName;
}

HRESULT CUIDropItem::Ready_Components()
{
	CTransform::TRANSFORM_DESC		TransformDesc{ 600.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Item"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
	
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer_Rect"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}
