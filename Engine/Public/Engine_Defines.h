﻿#pragma once

#pragma warning (disable : 4251)
#pragma warning (disable : 4819)

#include <d3d9.h>
#include <d3dx9.h>

#include <string>
#include <map>
#include <unordered_map>
#include <list>
#include <vector>
#include <algorithm>
#include <random>
#include <functional>
#include <set>
using namespace std;


#include "Engine_Enum.h"
#include "Engine_Function.h"
#include "Engine_Typedef.h"
#include "Matrix.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"

#include "FastNoiseLite.h"

using namespace Engine;


#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

#define	VK_MAX		0xff



