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
extern _bool g_bMCraftingTableOpen;
extern _bool g_bFurnaceUiOpen;
extern _float g_fBright;
extern LPD3DXFONT g_pTitleFont;
extern LPD3DXFONT g_pDetailFont;
extern LPD3DXFONT g_pWaveFont;

// ��ĵ ���� ������.
extern _bool g_bIsScan;				// ��ĵ �ߵ� ����.
extern _float g_fScanRange;			// ��ĵ ����.
extern _float g_fScanTime;			// ��ĵ ���� �ð�.
extern _float g_fScanEndTime;		// ��ĵ ���ӽð�.
extern _float g_fScanSpeed;			// ��ĵ�� ������� �ӵ�.

// ���� �ٰŸ� ���� ���� ������.
extern _bool g_bIsWave;				// ���� �ٰŸ� ���̺� �ߵ� ����.
extern _float g_fWaveRange;			// ���� �ٰŸ� ���� ����.
extern _float g_fWaveTime;			// ���� ���̺� ���� �ð�.
extern _float g_fWaveEndTime;		// ���� ���̺� ���ӽð�.
extern _float g_fWaveSpeed;			// ���� ���̺� ������� �ӵ�.

using namespace Client;

#define Rect_Model_Count 18
#define Cube_Model_Count 9
#define Item_Textue_Count 27




