module;

#include <Aurion/Platform.h>

#if defined(AURION_PLATFORM_WINDOWS)

#include <Aurion/Export.h>
#include <windows.h> // For HANDLE & DWORD

#endif

export module Aurion.Multithreading:Win32_Thread;

import :Thread;

export namespace Aurion
{
#if defined(AURION_PLATFORM_WINDOWS)

  typedef unsigned __stdcall (*ThreadRoutine_Win32)(void* arg);

  struct AURION_API Thread : public ThreadBase
  {
    HANDLE handle;
    DWORD id;
  };

  static unsigned __stdcall Thread_Trampoline(void* thread);

  static Thread CreateThread(const ThreadRoutine& routine, void* arg);
  static void JoinThread(const Thread& thread);

#endif
}