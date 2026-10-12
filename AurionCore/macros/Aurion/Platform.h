#pragma once

namespace Aurion::Platform
{
#if defined(_WIN32)
  #define AURION_PLATFORM_WINDOWS
  #define WIN32_LEAN_AND_MEAN
  inline constexpr bool Windows = true;
#elif defined(__APPLE__) || defined(__MACH__)
  #include <TargetConditionals.h>
  #if TARGET_OS_OSX
    #define AURION_PLATFORM_MACOS
    inline constexpr bool MacOS = true;
  #endif
#elif defined(__linux__)
  #define AURION_PLATFORM_LINUX
  inline constexpr bool Linux = true;
#else
  #error "Unsupported Platform"
#endif

#ifndef AURION_PLATFORM_WINDOWS
  inline constexpr bool Windows = false;
#endif
#ifndef AURION_PLATFORM_MACOS
  inline constexpr bool MacOS = false;
#endif
#ifndef AURION_PLATFORM_LINUX
  inline constexpr bool Linux = false;
#endif
}