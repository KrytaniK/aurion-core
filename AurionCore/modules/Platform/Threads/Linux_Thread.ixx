module;

#include <Aurion/Platform.h>

#if defined(AURION_PLATFORM_LINUX)

#include <Aurion/Export.h>
#include <pthread.h>

#endif

export module Aurion.Multithreading:Linux_Thread;

import :Thread;

export namespace Aurion
{
#if defined(AURION_PLATFORM_LINUX)

  struct AURION_API Thread : public ThreadBase
  {
    pthread_t handle = NULL;
  };

  static void* Thread_Trampoline(void* thread);

  static Thread CreateThread(const ThreadRoutine& routine, void* arg);
  static void JoinThread(const Thread& thread);

#endif
}