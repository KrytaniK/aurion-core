#pragma once

#ifdef AURION_PLATFORM_WINDOWS
	#define AURION_EXPORT __declspec(dllexport)
	#define AURION_IMPORT __declspec(dllimport)
#else
	#define AURION_EXPORT __attribute__((visibility("default")))
	#define AURION_IMPORT __attribute__((visibility("default")))
#endif

// If compiling Aurion Core as a static library, neutralise the API macro
#ifdef AURION_CORE_STATIC_COMPILE
  #define AURION_API
#else // Otherwise, compile as a shared library when defined
  #ifdef AURION_CORE_SHARED_COMPILE
    #define AURION_API AURION_EXPORT
  #else
    #define AURION_API AURION_IMPORT
  #endif
#endif