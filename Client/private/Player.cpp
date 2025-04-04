#include "Player.h"

#include "GameInstance.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject { pGraphic_Device }
{
}

CPlayer::CPlayer(const CPlayer& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{		
	/*if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, PROTOTYPE_GAMEOBJECT_PARTICLE_DASH,
		LEVEL_STATIC, LAYER_PARTICLE_DASH)))
		return E_FAIL;

	m_pParticleDash = (CParticleSystem*)m_pGameInstance->Get_Object(LEVEL_STATIC, LAYER_PARTICLE_DASH.c_str(), 0);*/

	/*if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, PROTOTYPE_GAMEOBJECT_PARTICLE_SAND_MINING,
		LEVEL_STATIC, LAYER_PARTICLE_SAND_MINING)))
		return E_FAIL;

	m_pParticleSandMining = (CParticleSystem*)m_pGameInstance->Get_Object(LEVEL_STATIC, LAYER_PARTICLE_SAND_MINING.c_str(), 0);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, PROTOTYPE_GAMEOBJECT_PARTICLE_WOOD_MINING,
		LEVEL_STATIC, LAYER_PARTICLE_WOOD_MINING)))
		return E_FAIL;

	m_pParticleWoodMining = (CParticleSystem*)m_pGameInstance->Get_Object(LEVEL_STATIC, LAYER_PARTICLE_WOOD_MINING.c_str(), 0);*/

	/*if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, PROTOTYPE_GAMEOBJECT_PARTICLE_SAND_DESTROY,
		LEVEL_STATIC, LAYER_PARTICLE_SAND_DESTROY)))
		return E_FAIL;

	m_pParticleSandDestroy = (CParticleSystem*)m_pGameInstance->Get_Object(LEVEL_STATIC, LAYER_PARTICLE_SAND_DESTROY.c_str(), 0);*/

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(static_cast<_float>(rand() % 10), 0.5f, static_cast<_float>(rand() % 10)));
	m_count = 0.f;
	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{	
}

void CPlayer::Update(_float fTimeDelta)
{	
	if (GetKeyState(VK_UP) & 0x8000)
	{
		/*m_count += fTimeDelta;
		if (m_count >= 0.2f)
		{
			m_pParticleDash->Replay(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

			m_count = 0.f;
		}*/

		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * -1.f);
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta);
	}

	if (GetKeyState('X') & 0x8000)
	{
		//m_pParticleSandDestroy->Replay(m_pTransformCom->Get_State(CTransform::STATE_POSITION));		
		//CParticleSystem* particle = (CParticleSystem*)m_pGameInstance->PushPool(LEVEL_STATIC,	// 가져올 씬
		//	PROTOTYPE_GAMEOBJECT_PARTICLE_DASH,	// 가져올 프로토타입.
		//	LEVEL_HYEOK,	// 적용 씬.
		//	LAYER_PARTICLE);	// 애드오브젝트에 추가할 레이어

		//if (particle != nullptr)
		//{
		//	particle->GetTransform()->Set_State(CTransform::STATE_LOOK, m_pTransformCom->Get_State(CTransform::STATE_LOOK));
		//	particle->GetTransform()->Set_State(CTransform::STATE_UP, m_pTransformCom->Get_State(CTransform::STATE_UP));
		//	particle->GetTransform()->Set_State(CTransform::STATE_RIGHT, m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
		//	particle->Replay(m_pTransformCom->Get_State(CTransform::STATE_POSITION));					
		//}		

		// 사망
		//CParticleSystem* particle = (CParticleSystem*)m_pGameInstance->PushPool(LEVEL_STATIC,	// 가져올 씬
		//	PROTOTYPE_GAMEOBJECT_PARTICLE_DIE,	// 가져올 프로토타입.
		//	LEVEL_STATIC,	// 적용 씬.
		//	LAYER_PARTICLE);	// 애드오브젝트에 추가할 레이어

		//if (particle != nullptr)
		//{			
		//	particle->Replay(m_pTransformCom->Get_State(CTransform::STATE_POSITION));			
		//}

		// 폭죽.
		//CParticleSystem* particle = (CParticleSystem*)m_pGameInstance->PushPool(LEVEL_STATIC,	// 가져올 씬
		//	PROTOTYPE_GAMEOBJECT_PARTICLE_FIRE_CRACKER,	// 가져올 프로토타입.
		//	LEVEL_STATIC,	// 적용 씬.
		//	LAYER_PARTICLE);	// 애드오브젝트에 추가할 레이어

		//if (particle != nullptr)
		//{			
		//	particle->Replay(m_pTransformCom->Get_State(CTransform::STATE_POSITION));			
		//}

		// 기 모으기.
		//CParticleSystem* particle = (CParticleSystem*)m_pGameInstance->PushPool(LEVEL_STATIC,	// 가져올 씬
		//	PROTOTYPE_GAMEOBJECT_PARTICLE_GLITTER,	// 가져올 프로토타입.
		//	LEVEL_STATIC,	// 적용 씬.
		//	LAYER_PARTICLE);	// 애드오브젝트에 추가할 레이어

		//if (particle != nullptr)
		//{			
		//	particle->Replay(m_pTransformCom->Get_State(CTransform::STATE_POSITION));			
		//}

		// 사과 먹는 파티클.
		CParticleEventManager::Get_Instance()->OnParticle(
			PROTOTYPE_GAMEOBJECT_PARTICLE_EATING,
			m_pTransformCom);

		_float3 screenPos = m_pTransformCom->WorldToScreen();
		int a = 10;
	}

	if (GetKeyState('Z') & 0x8000)
	{	
		CParticleSystem* particle = (CParticleSystem*)m_pGameInstance->PushPool(LEVEL_STATIC,	// 가져올 씬
			PROTOTYPE_GAMEOBJECT_PARTICLE_EXPLOSION,	// 가져올 프로토타입.
			LEVEL_HYEOK,	// 적용 씬.
			LAYER_PARTICLE);	// 애드오브젝트에 추가할 레이어		

		if (particle != nullptr)
		{
			particle->GetTransform()->Set_State(CTransform::STATE_LOOK, m_pTransformCom->Get_State(CTransform::STATE_LOOK));
			particle->GetTransform()->Set_State(CTransform::STATE_UP, m_pTransformCom->Get_State(CTransform::STATE_UP));
			particle->GetTransform()->Set_State(CTransform::STATE_RIGHT, m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
			particle->Replay(m_pTransformCom->Get_State(CTransform::STATE_POSITION));								
		}
	}
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CPlayer::Render()
{	
	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	SetUp_RenderState();

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	Release_RenderState();

	return S_OK;
}

HRESULT CPlayer::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 254);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	/*m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);*/

	return S_OK;
}

HRESULT CPlayer::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

HRESULT CPlayer::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL; 	

	return S_OK;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer* pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	/*Safe_Release(m_pParticleDash);
	Safe_Release(m_pParticleSandMining);
	Safe_Release(m_pParticleWoodMining);
	Safe_Release(m_pParticleSandDestroy);*/
}
