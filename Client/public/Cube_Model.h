#include "Client_Defines.h"
#include "RightHand_Object.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Anim;
END

class CCube_Model : public CRightHand_Object
{
protected:
	CCube_Model(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCube_Model(const CCube_Model& Prototype);
	virtual ~CCube_Model() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components();

private:
	HRESULT Ready_Bone() override;
	HRESULT Ready_Animation() override;
	void Motion_Idle(_float fTimeDelta) override;
	void Motion_Swing(_float fTimeDelta) override;
	void Motion_Walk(_float fTimeDelta) override;
	void KeyInput() override;

public:
	static CCube_Model* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};