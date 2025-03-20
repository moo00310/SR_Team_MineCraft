#include "BTTask_Patrol.h"
#include "Monster.h"

STATUS CBTTask_Patrol::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
    if (!_Obj) return STATUS::FAIL;

    CMonster* pMonster = static_cast<CMonster*>(_Obj);

    // Transform ������Ʈ ��������
    CTransform* pTransform = pMonster->Get_Transform();
    if (!pTransform) return STATUS::FAIL;

    //����ƽ ������ �����ϴ� ������ ����(�׽�Ʈ��)
    static _float elapsedTime = 0.0f;
    static _float targetRotation = 0.0f; // ��ǥ ȸ�� ����
    static bool isTurning = false;
    static _float turnElapsedTime = 0.0f; // ȸ�� ���� �ð�
    static _float turnDuration = 0.0f;    // ȸ�� ���� �ð�

    elapsedTime += _fTimeDelta;

    // ���� �ð��� ������ ���ο� ���� ȸ�� ����
    if (elapsedTime > 3.0f && !isTurning)
    {
        int randomValue = rand() % 91 - 45; // -45 ~ 45 ������
        targetRotation = D3DXToRadian(static_cast<_float>(randomValue)); // ���� ��ȯ
        turnDuration = 1.0f + (rand() % 200) / 100.0f; // 1.0 ~ 3.0�� ���� ȸ��
        turnElapsedTime = 0.0f;
        isTurning = true; // ȸ�� ����
        elapsedTime = 0.0f;
    }

    if (isTurning)
    {
        pTransform->Turn({ 0.0f, 1.0f, 0.0f }, targetRotation * _fTimeDelta); // Y�� ȸ��
        turnElapsedTime += _fTimeDelta;

        if (turnElapsedTime >= turnDuration) // ȸ�� �ð��� ������ ����
        {
            isTurning = false;
        }
    }

    // ���� �̵�
    pTransform->Go_Straight(_fTimeDelta);

    return STATUS::RUNNING;
}

