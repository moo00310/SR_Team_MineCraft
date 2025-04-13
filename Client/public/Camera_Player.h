#pragma once
#include "Client_Defines.h"
#include "Camera.h"
#include "Steve.h"
#include "DestroyCube.h"
#include "Crosshair.h"
#include "LayHitCube.h"

BEGIN(Client)

class CCamera_Player final : public CCamera
{
public:
    typedef struct tagCamera_Player_Desc : public CCamera::CAMERA_DESC
    {
        _float          fMouseSensor{};
        CGameObject* pTarget = { nullptr };
    } CAMERA_PLAYER_DESC;

    enum class E_CAMERA_MODE { FPS, TPS, R_TPS, CUTSCENE };

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

public:
    CTransform* GetTransform() const;
    E_CAMERA_MODE   Get_CameraMode() { return m_eCameraMode; }

public:
    void	Start_Cutscene(_float3 vPos);
    void	End_Cutscene();

private:
    POINT               m_ptOldMousePos = {};
    _float              m_fMouseSensor = {};
    CTransform*         m_pTarget_Transform_Com = { nullptr };
    CRigidbody*         m_pTarget_Rigidbody_Com = { nullptr };
    _float              m_fYaw{};
    _float              m_fPitch{};

    E_CAMERA_MODE       m_eCameraMode = E_CAMERA_MODE::TPS;
    CSteve* m_pPlayer = { nullptr };
    CCrosshair* m_pCrosshair{ nullptr };
    CLayHitCube* m_pHitCube{ nullptr };

private:
    //_float3             m_vCameraPos{};
    _bool               m_isActiveMouse{ false };
    _float              m_fWalkTime{};
    _float              m_fSpringArmLength{ 5.f };
    CDestroyCube* m_DestroyCube = { nullptr };
    class CMCTerrain* m_pTerrain = { nullptr };
    vector<_uint> m_Groups;

private:
    _float m_AttackDamage = {};
    _float m_AttackCoolTime = {};
    _float m_CoolTimeDelta = {};

private:
    _float  m_fAngle{ 0.f };
    _float3 m_vCutScene_LookPos{};

private:
    void    Input_Key(_float fTimeDelta);
    void    Follow_Player(_float fTimeDelta);

    void    Orbit_Around_Pos(_float fTimeDelta);
    void    On_MouseMove(_float fTimeDelta);
    void    Setting_Damage();
    void    LayHitCube();

private:
    HRESULT Ready_Components();

public:
    static CCamera_Player* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free();
};

END
