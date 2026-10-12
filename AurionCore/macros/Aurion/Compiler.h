#pragma once

namespace Aurion::Compiler
{
#if defined(__clang__)
#define AURION_COMPILER_CLANG
  inline constexpr bool Clang = true;
#elif defined(__MSC_VER)
  #define AURION_COMPILER_MSVC
  inline constexpr bool MSCV = true;
#elif defined (__GNUC__)
  #define AURION_COMPILER_GCC
  inline constexpr bool GCC = true;
#endif

#ifndef AURION_COMPILER_MSVC
  inline constexpr bool MSCV = false;
#endif
#ifndef AURION_COMPILER_CLANG
  inline constexpr bool Clang = false;
#endif
#ifndef AURION_COMPILER_GCC
  inline constexpr bool GCC = false;
#endif
}