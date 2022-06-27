#pragma once


#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")//콘솔창 생성

#include "pch_common.h"

//엔진을 바꿀 경우 속성 - C/C++ - 전처리기 - 전처리기 정의에서 바꿔줄 것
#ifdef DX11_RENDERER
//#include "pch_dx_11.h"
#include "DX11Renderer.h"
typedef DX11Renderer RenderEngine;//todo : typedef CUSTOM_ENGINE engine
#endif
