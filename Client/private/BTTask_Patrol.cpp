#include "BTTask_Patrol.h"
#include "Monster.h"

STATUS CBTTask_Patrol::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
    if (!_Obj) return STATUS::FAIL;

    CMonster* pMonster = static_cast<CMonster*>(_Obj);

    // Transform 컴포넌트 가져오기
    CTransform* pTransform = pMonster->Get_Transform();
    if (!pTransform) return STATUS::FAIL;

    //스태틱 변수들 불편하니 수정할 예정(테스트임)
    static _float elapsedTime = 0.0f;
    static _float targetRotation = 0.0f; // 목표 회전 각도
    static bool isTurning = false;
    static _float turnElapsedTime = 0.0f; // 회전 진행 시간
    static _float turnDuration = 0.0f;    // 회전 지속 시간

    elapsedTime += _fTimeDelta;

    // 일정 시간이 지나면 새로운 랜덤 회전 설정
    if (elapsedTime > 3.0f && !isTurning)
    {
        int randomValue = rand() % 91 - 45; // -45 ~ 45 정수값
        targetRotation = D3DXToRadian(static_cast<_float>(randomValue)); // 라디안 변환
        turnDuration = 1.0f + (rand() % 200) / 100.0f; // 1.0 ~ 3.0초 동안 회전
        turnElapsedTime = 0.0f;
        isTurning = true; // 회전 시작
        elapsedTime = 0.0f;
    }

    if (isTurning)
    {
        pTransform->Turn({ 0.0f, 1.0f, 0.0f }, targetRotation * _fTimeDelta); // Y축 회전
        turnElapsedTime += _fTimeDelta;

        if (turnElapsedTime >= turnDuration) // 회전 시간이 지나면 종료
        {
            isTurning = false;
        }
    }

    // 직진 이동
    pTransform->Go_Straight(_fTimeDelta);

    return STATUS::RUNNING;
}

