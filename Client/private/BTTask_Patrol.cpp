#include "BTTask_Patrol.h"
#include "Monster.h"

STATUS CBTTask_Patrol::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
    if (!_Obj) return STATUS::FAIL;

    CMonster* pMonster = static_cast<CMonster*>(_Obj);
    pMonster->Set_Animation(CMonster::WALK);

    // Transform ������Ʈ ��������
    CTransform* pTransform = pMonster->Get_Transform();
    if (!pTransform) return STATUS::FAIL;

    m_fElapsedTime += _fTimeDelta;
    m_fToIdle += _fTimeDelta;

    if (m_fToIdle > 12.f)
    {
        m_fToIdle = 0.f;
        m_fElapsedTime = 0.f;
        return STATUS::FAIL;
    }

    // ���� �ð��� ������ ���ο� ���� ȸ�� ����
    if (m_fElapsedTime > 3.0f && !m_isTurning)
    {
        int randomValue = rand() % 361 - 180; // -180 ~ 180 ������
        m_fTargetRotation = D3DXToRadian(static_cast<_float>(randomValue)); // ���� ��ȯ
        m_fTurnDuration = 1.0f + (rand() % 200) / 100.0f; // 1.0 ~ 3.0�� ���� ȸ��
        m_fTurnElapsedTime = 0.0f;
        m_isTurning = true; // ȸ�� ����
        m_fElapsedTime = 0.0f;
    }

    if (m_isTurning)
    {
        pTransform->Turn({ 0.0f, 1.0f, 0.0f }, m_fTargetRotation * _fTimeDelta); // Y�� ȸ��
        m_fTurnElapsedTime += _fTimeDelta;

        if (m_fTurnElapsedTime >= m_fTurnDuration) // ȸ�� �ð��� ������ ����
        {
            m_isTurning = false;
        }
    }

    // ���� �̵�
    pTransform->Go_Straight(_fTimeDelta);
    cout << "��Ʈ��" << endl;
    return STATUS::RUNNING;
}

