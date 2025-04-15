#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Ending final : public CLevel
{
private:
	typedef struct tagEndingEvent
	{
		_float	fTriggerTime;           // �� �� �ڿ� ǥ������
		wstring wstrText;				// ǥ���� �ؽ�Ʈ
		bool bTriggered = false;		// �̹� ����ƴ��� üũ
	}ENDING_EVENT;

private:
	CLevel_Ending(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Ending() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	void	Ready_EndingEvents();

private:
	_float m_fTime{ 0.f };
	vector<ENDING_EVENT> m_EndingEvents;

public:
	static CLevel_Ending* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END