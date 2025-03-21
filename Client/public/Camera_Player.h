#pragma once
#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Player final : public CCamera
{
public:
    typedef struct tagCamera_Player_Desc : public CCamera::CAMERA_DESC
    {
        _float          fMouseSensor{};
        CGameObject* pTarget = { nullptr };
    } CAMERA_PLAYER_DESC;

private:
    CCamera_Player(LPDIRECT3DDEVICE9 pGraphic_Device);
    CCamera_Player(const CCamera_Player& Prototype);
    virtual ~CCamera_Player() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Update(_float fTimeDelta) override;
    virtual void Update(_float fTimeDelta) override;
    virtual void Late_Update(_float fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    POINT               m_ptOldMousePos = {};
    _float              m_fMouseSensor = {};
    CTransform*         m_pTargetTransformCom = { nullptr };
    _float              m_fYaw{};
    _float              m_fPitch{};
    _float              m_fRoll{};

    // === 1인칭 ↔ 3인칭 전환을 위한 변수 추가 ===
    enum class E_CAMERA_MODE { FPS, TPS };
    E_CAMERA_MODE       m_eCameraMode = E_CAMERA_MODE::TPS; // 기본값 TPS

private:
    void    Input_Key(_float fTimeDelta);
private:
    HRESULT Ready_Components();

public:
    static CCamera_Player* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free();
};

END
