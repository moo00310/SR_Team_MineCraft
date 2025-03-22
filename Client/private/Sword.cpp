#include "Sword.h"

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
        Update_Anime(SWING, fTimeDelta);
    }
    
    if (m_pGameInstance->Key_Down(VK_F5))
    {
        m_bisTPS *= -1;
    }
    if (m_bisTPS > 0)
    {
        if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this)))
            return;
    }
}

HRESULT CSword::Render()
{
    __super::Render();

    return S_OK;
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

    return S_OK;
}