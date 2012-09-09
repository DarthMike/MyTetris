// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

//GENERAL C++ LIBRARIES
#include <stdio.h>
#include <tchar.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

//STD LIBRARIES
#include <map>
#include <string>
#include <vector>

//Memory leaks detection
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

/*
FROM: http://cgempire.com/forum/tutorials-101/easy-memory-leak-detection-c-c-546.html
GREAT WORK!
*/

#if _DEBUG 
#define FINDMEMLEAK(val) _DetectMemoryLeaks::DetectMemoryLeaks(val) 
//Memory leak detection 
class _DetectMemoryLeaks 
{ 
public: 
	static _DetectMemoryLeaks& DetectMemoryLeaks(int val) 
	{ 
		static _DetectMemoryLeaks inst; 
		_crtBreakAlloc=val; 
		return inst; 
	} 
	~_DetectMemoryLeaks() 
	{ 
		//Dumps memory leaks when destructed (end of program)
		_CrtDumpMemoryLeaks(); 
	} 
}; 
#else 
#define FINDMEMLEAK(val) 
#endif
