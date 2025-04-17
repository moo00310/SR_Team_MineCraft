#include "Level_Ending.h"
#include "GameInstance.h"
#include "Ending_Text.h"

CLevel_Ending::CLevel_Ending(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{

}

HRESULT CLevel_Ending::Initialize()
{
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_Background"))))
		return E_FAIL;

	Ready_EndingEvents();

	m_pGameInstance->PlayBGM(TEXT("hal3_1"));

	return S_OK;
}

void CLevel_Ending::Update(_float fTimeDelta)
{
	if (0.1f < fTimeDelta)
		return;

	m_fTime += fTimeDelta;

	for (auto& event : m_EndingEvents)
	{
		if (!event.bTriggered && m_fTime >= event.fTriggerTime)
		{
			CEnding_Text::ENDING_TEXT_DESC tDesc{};
			tDesc.fX = g_iWinSizeX >> 1;
			tDesc.fY = g_iWinSizeY * 1.1f;
			tDesc.wstrText = event.wstrText;

			if (FAILED(m_pGameInstance->Add_GameObject(
				LEVEL_ENDING,
				TEXT("Prototype_GameObject_Ending_Text"),
				LEVEL_ENDING,
				TEXT("Layer_Text"),
				&tDesc)))
			{
				return;
			}

			event.bTriggered = true;
		}
	}

}

HRESULT CLevel_Ending::Render()
{
	SetWindowText(g_hWnd, TEXT("엔딩 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Ending::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_ENDING, TEXT("Prototype_GameObject_Ending_Background"),
		LEVEL_ENDING, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

void CLevel_Ending::Ready_EndingEvents()
{
	m_EndingEvents.push_back({ 0.f,		L"임무결 (팀장)" });
	m_EndingEvents.push_back({ 1.f,		L"- 모델링, 애니메이션, 몬스터AI" });

	m_EndingEvents.push_back({ 3.f,		L"최유경" });
	m_EndingEvents.push_back({ 4.f,		L"- 맵, 블럭 최적화" });

	m_EndingEvents.push_back({ 6.f,		L"장현우" });
	m_EndingEvents.push_back({ 7.f,		L"- UI, 크래프팅" });

	m_EndingEvents.push_back({ 9.f,		L"한영웅" });
	m_EndingEvents.push_back({ 10.f,	L"- 프레임워크, 충돌 최적화" });

	m_EndingEvents.push_back({ 12.f,    L"김동혁" });
	m_EndingEvents.push_back({ 13.f,	L"- FX, 파티클시스템" });
}

CLevel_Ending* CLevel_Ending::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Ending* pInstance = new CLevel_Ending(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Ending");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Ending::Free()
{
	__super::Free();

}
