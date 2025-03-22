#include "Sword.h"
#include "GameInstance.h"

CSword::CSword(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CItem_Model{ pGraphic_Device }
{
}

CSword::CSword(const CSword& Prototype)
    : CItem_Model(Prototype)
{
}

HRESULT CSword::Initialize_Prototype()
{ 
  
    return S_OK;
}

HRESULT CSword::Initialize(void* pArg)
{
    DESC* pDesc = (DESC*)pArg;
    m_TextureTag = pDesc->TextureTag;
    m_TextureLevel = pDesc->TextureLevel;

    __super::Initialize(pArg);

    if (FAILED(Ready_Bone()))
        return E_FAIL;

    if (FAILED(Ready_Anime()))
        return E_FAIL;

    return S_OK;
}

void CSword::Priority_Update(_float fTimeDelta)
{
	
}

void CSword::Update(_float fTimeDelta)
{
    
}

void CSword::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
    
    if (m_pGameInstance->Key_Pressing(VK_LBUTTON))
    {
		m_eCurAnim = ANIM::SWING;
    }

     //애니메이션 적용
    if (m_eCurAnim != ANIM::NONE)
        Update_Motion(m_eCurAnim, fTimeDelta);
    
    if (m_pGameInstance->Key_Down(VK_F5))
    {
        m_bisTPS *= -1;
    }
    if (m_bisTPS > 0)
    {
        if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this)))
            return;
    }
}

HRESULT CSword::Render()
{
    __super::Render();

    return S_OK;
}

HRESULT CSword::Update_Motion(_int _type, _float fTimeDelta)
{
    switch (_type)
    {
    case CSword::IDLE:
        Idle_Sword(fTimeDelta);
        return S_OK;
    case CSword::SWING:
        Swing_Sword(fTimeDelta);
        return S_OK;
    case CSword::ANIM_END:
    default:
        return S_OK;
    }
}

void CSword::Swing_Sword(_float fTimeDelta)
{
	if (m_Animations[SWING].empty())
		return;

	if (FAILED(Update_Anime(SWING, fTimeDelta)))
		return;

	if (m_Animations[SWING].back().fTime < fElapsedTime)
	{
		fElapsedTime = 0.f;
		m_eCurAnim = ANIM::IDLE;
	}

}

void CSword::Idle_Sword(_float fTimeDelta)
{
	if (m_Animations[IDLE].empty())
		return;

	if (FAILED(Update_Anime(IDLE, fTimeDelta)))
		return;

	if (m_Animations[IDLE].back().fTime < fElapsedTime)
	{
		fElapsedTime = 0.f;
	}
}

CSword* CSword::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CSword* pInstance = new CSword(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CSword");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSword::Clone(void* pArg)
{
    CSword* pInstance = new CSword(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CSword");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSword::Free()
{
	m_eCurAnim = ANIM::NONE;
    __super::Free();


}

HRESULT CSword::Ready_Bone()
{
    // 스윙 프레임 행렬을 벡터에 저장
    Matrix mat = {};
    mat.Turn_Radian(_float3(0.f, 1.f, 0.f), D3DXToRadian(70));
    mat.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(45));
    mat.Set_State(mat.STATE_POSITION, _float3(1.f, -0.5f, 1.2f));


    m_Bone = { "root", -1, mat, mat, Matrix(), Matrix() };


    return S_OK;
}

HRESULT CSword::Ready_Anime()
{
    /*------------------------
    * 스윙 애니메이션
    ----------------------------*/

    Matrix matrix = {};
    matrix.Turn_Radian(_float3(0.f, 0.f, 1.f), D3DXToRadian(100));
    matrix.Set_State(matrix.STATE_POSITION, _float3(-1.5f, 0.2f, -0.3f));

    Matrix matrix2 = { matrix };
    matrix2.Set_State(matrix.STATE_POSITION, _float3(-1.5f, -1.5f, 2.f));

	m_swing.push_back({ 0.f, Matrix()});
	m_swing.push_back({ 0.15f, matrix });
	m_swing.push_back({ 0.4f, matrix2 });
	m_swing.push_back({ 1.f, Matrix() });
   
    m_Animations.emplace(SWING, m_swing);

    /*------------------------
    * 대기 애니메이션
    ----------------------------*/
	m_IDLE.push_back({ 0.f, Matrix() });
    m_Animations.emplace(IDLE, m_IDLE);

    return S_OK;
}