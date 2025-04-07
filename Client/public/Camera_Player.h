#pragma once
#include "Client_Defines.h"
#include "Camera.h"
#include "Steve.h"
#include "DestroyCube.h"

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
    CTransform* GetTransform() const;

private:
    POINT               m_ptOldMousePos = {};
    _float              m_fMouseSensor = {};
    CTransform*         m_pTarget_Transform_Com = { nullptr };
    CRigidbody*         m_pTarget_Rigidbody_Com = { nullptr };
    _float              m_fYaw{};
    _float              m_fPitch{};
    enum class E_CAMERA_MODE { FPS, TPS };
    E_CAMERA_MODE       m_eCameraMode = E_CAMERA_MODE::TPS; // ±âº»°ª TPS
    CSteve*             m_pPlayer = {nullptr};

private:
    //_float3             m_vCameraPos{};
    _bool               m_isActiveMouse{ false };
    _float              m_fWalkTime{};
    _float              m_fSpringArmLength{ 5.f };
    CDestroyCube*       m_DestroyCube = { nullptr };
	class CMCTerrain*   m_pTerrain = { nullptr };

private:
    void    Input_Key(_float fTimeDelta);
    void    Follow_Player(_float fTimeDelta);
    void    On_MouseMove(_float fTimeDelta);
private:
    HRESULT Ready_Components();

public:
    static CCamera_Player* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free();
};

END
