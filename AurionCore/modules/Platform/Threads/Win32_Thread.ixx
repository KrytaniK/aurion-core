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
    ThreadRoutine routine = nullptr;
    void* arg = nullptr;
    ThreadResult result = {};

    HANDLE handle;
    DWORD id;
  };

  unsigned __stdcall Thread_Trampoline(void* desc);

  void CreateThread(Thread& thread, const ThreadRoutine& routine, void* arg);
  void JoinThread(Thread& thread);

#endif
}