#pragma once

/* Ư�� ������Ͽ� �Ҽӵ��� �ʴ� ������������ �����صδ� ��Ȱ */
/* ��Ÿ �ٸ� ��������(.h)���� ��Ŭ����ϴ� ��Ȱ. */
#include <Windows.h>
#include <process.h>
#include "Client_Enum.h"
#include "Client_Struct.h"
#include "Client_Tag.h"


namespace Client
{
	const unsigned int			g_iWinSizeX = 1280;
	const unsigned int			g_iWinSizeY = 720;
}

extern HWND g_hWnd;

extern _bool g_bChangeLevel;
extern _bool g_bMainInventoryOpen;
extern _float g_fBright;
extern LPD3DXFONT g_pTitleFont;
extern LPD3DXFONT g_pDetailFont;

using namespace Client;

#define Rect_Model_Count 17
#define Cube_Model_Count 7
#define Item_Textue_Count 24




