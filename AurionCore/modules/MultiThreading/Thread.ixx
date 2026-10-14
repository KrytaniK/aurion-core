module;

#include <Aurion/Export.h>

export module Aurion.Multithreading:Thread;

import Aurion.Types;

export namespace Aurion
{
  typedef void (*ThreadRoutine)(void* arg);

  enum class AURION_API ThreadStatus : u8
  {
    Uninitialized = 0,
    Created = 1 << 0,
    Delayed = 1 << 1,
    Blocked = 1 << 2,
    Running = 1 << 3,
    Finished = 1 << 4,
    Joined  = 1 << 5,
  };

  struct AURION_API IThread
  {
    virtual ~IThread() = default;

    virtual void Start() = 0;

    virtual void Sleep(u32 ms) = 0;
    virtual void Join() = 0;

    virtual void* GetNativeHandle() = 0;
    virtual const ThreadStatus& GetStatus() = 0;
  };
}