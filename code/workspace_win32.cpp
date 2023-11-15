
// #ifdef	STD_INCLUDE_DECL
// #endif	// STD_INCLUDE_DECL
// #ifdef	STD_INCLUDE_FUNC
// #endif	// STD_INCLUDE_FUNC

//#pragma optimize( "", off )

#include <math.h> // sqrtf, cosf, sinf
#include <stdint.h> // Type definitions
#include <stdlib.h> // rand
#include <time.h> // time
#include <float.h> // FLT_MAX
#include <cstdio> // sprintf
#include <shlwapi.h>
#include <windows.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_trueType.h"

#include "TypeDef.h"
#include "math.cpp"

#define 	STD_INCLUDE_DECL
#include "Vector.cpp"
#include "Memory.cpp"
#include "Font.cpp"
#include "userInput.cpp"
#undef	STD_INCLUDE_DECL

#define	STD_INCLUDE_FUNC
#include "Vector.cpp"
#include "Memory.cpp"
#include "Font.cpp"
#include "userInput.cpp"
#undef	STD_INCLUDE_FUNC

#include "Color.cpp"
#include "DampedSpring.cpp"
#include "Parser.cpp"

#include "Locker_Config.h"
#include "AssetID.h"


#include <gl/gl.h>
#include "OpenGL.cpp"
//#include "inputString.cpp"

#include "Win32_Platform.h"

#define APP_TargetFPS  ( 60.0f )

//#include <dsound.h>
//#include "AudioSystem.h"
#if USE_FMOD
#include "fmod_studio.h"
#endif

//#include "Intersect.h"

#include "Entity.h"
#include "Locker.h"
#include "Debug_Win32.cpp"
#include "tools.cpp"
//#include "Intersect.cpp"

//#include "AudioSystem.cpp"
#if USE_FMOD
#include "FMod.cpp"
#endif

#include "bmp.cpp"
//#include "wav.cpp"

#include "Draw.cpp"

#include "FramerateTest.cpp"
#include "Particle.cpp"
#include "Worker.cpp"
#include "Locker.cpp"

#include "Win32_Platform.cpp"
