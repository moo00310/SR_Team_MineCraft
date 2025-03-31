#include "BTTask_Patrol.h"
#include "Monster.h"

STATUS CBTTask_Patrol::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
    if (!_Obj) return STATUS::FAIL;

    CMonster* pMonster = static_cast<CMonster*>(_Obj);
    pMonster->Set_Animation(CMonster::WALK);

    // Transform 컴포넌트 가져오기
    CTransform* pTransform = pMonster->Get_Transform();
    if (!pTransform) return STATUS::FAIL;

    m_fElapsedTime += _fTimeDelta;
    m_fToIdle += _fTimeDelta;

    // 일정 시간이 지나면 새로운 랜덤 회전 설정
    if (m_fElapsedTime > 3.0f && !m_isTurning)
    {
        int randomValue = rand() % 361 - 180; // -180 ~ 180 정수값
        m_fTargetRotation = D3DXToRadian(static_cast<_float>(randomValue)); // 라디안 변환
        m_fTurnDuration = 1.0f + (rand() % 200) / 100.0f; // 1.0 ~ 3.0초 동안 회전
        m_fTurnElapsedTime = 0.0f;
        m_isTurning = true; // 회전 시작
        m_fElapsedTime = 0.0f;
    }

    if (m_isTurning)
    {
        pTransform->Turn({ 0.0f, 1.0f, 0.0f }, m_fTargetRotation * _fTimeDelta); // Y축 회전
        m_fTurnElapsedTime += _fTimeDelta;

        if (m_fTurnElapsedTime >= m_fTurnDuration) // 회전 시간이 지나면 종료
        {
            m_isTurning = false;
        }
    }

    //움직일라 하는데 속도가 안난다 점프함 ㅋㅋ
    if (D3DXVec3LengthSq(&pMonster->Get_RigidBody()->Get_Velocity()) < 0.5f)
    {
        pMonster->Get_RigidBody()->Jump(7.f);
    }

    // 직진 이동
    pTransform->Go_Straight(_fTimeDelta);
    return STATUS::RUNNING;
}

