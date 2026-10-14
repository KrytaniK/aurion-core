module;

#include <Aurion/Export.h>

export module Aurion.Multithreading:Thread;

import Aurion.Types;

export namespace Aurion
{
  typedef void* ThreadResult;
  typedef ThreadResult (*ThreadRoutine)(void* arg);

  enum class AURION_API ThreadStatus : u8
  {
    Uninitialized = 0,
    Created = 1 << 0,
    Waiting = 1 << 2,
    Delayed = 1 << 3,
    Blocked = 1 << 4,
    Running = 1 << 5,
    Joined  = 1 << 6,
  };

  struct AURION_API IThread
  {
    virtual ~IThread() = default;

    virtual void Sleep(u32 ms) = 0;
    virtual ThreadResult Join() = 0;
    virtual void* GetNativeHandle() = 0;
  };
}