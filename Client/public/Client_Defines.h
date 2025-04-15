#pragma once

/* 특정 헤더파일에 소속되지 않는 전역정보들을 선언해두는 역활 */
/* 기타 다른 정의파일(.h)들을 인클루드하는 역활. */
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

// 스캔 관련 변수들.
extern _bool g_bIsScan;				// 스캔 발동 여부.
extern _float g_fScanRange;			// 스캔 범위.
extern _float g_fScanTime;			// 스캔 현재 시간.
extern _float g_fScanEndTime;		// 스캔 지속시간.
extern _float g_fScanSpeed;			// 스캔이 뻗어나가는 속도.

// 워든 근거리 공격 관련 변수들.
extern _bool g_bIsWave;				// 워든 근거리 웨이브 발동 여부.
extern _float g_fWaveRange;			// 워든 근거리 공격 범위.
extern _float g_fWaveTime;			// 워든 웨이브 현재 시간.
extern _float g_fWaveEndTime;		// 워든 웨이브 지속시간.
extern _float g_fWaveSpeed;			// 워든 웨이브 뻗어나가는 속도.

using namespace Client;

#define Rect_Model_Count 18
#define Cube_Model_Count 9
#define Item_Textue_Count 27




