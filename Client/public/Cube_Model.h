#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Anim;
END

class CCube_Model : public CGameObject
{
public:
	enum ANIM {
		IDLE, SWING, WALK, ANIM_END
	};

public:
	typedef struct tagItemDesc
	{
		wstring TextureTag = {};
		LEVEL TextureLevel = {};
	}DESC;

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
	bool isAttack = { false };
	ANIM m_eCurAnim = { IDLE };
	int m_bisTPS = { -1 };

private:
	HRESULT Ready_Components();
	HRESULT Ready_Bone();
	HRESULT Ready_Animation();

private:
	CTexture* m_pTextureCom = { nullptr };
	vector<CVIBuffer_Anim*> m_pVIBufferComs = { nullptr };
	CSkeletalAnimator* m_pSkeletalAnimator = { nullptr };

public:
	static CCube_Model* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};