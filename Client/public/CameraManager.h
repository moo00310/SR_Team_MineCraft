#pragma once
#include "Base.h"
#include "Camera.h"

class CCameraManager : public CBase
{
private:
	CCameraManager();
	virtual ~CCameraManager() = default;
public:
	HRESULT Initialize();
public:
	void Change_Camera();
private:
	list<CCamera*> m_Cameras;
	list<CCamera*>::iterator m_CurrentCamera;
public:
	static CCameraManager* Create();
	void Free();
};

