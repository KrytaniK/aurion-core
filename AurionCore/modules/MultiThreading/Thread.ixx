module;

#include <Aurion/Export.h>

export module Aurion.Multithreading:Thread;

export namespace Aurion
{
  typedef void (*ThreadRoutine)(void* arg);

  struct AURION_API ThreadResult
  {
    void* data = nullptr;
  };

  struct AURION_API ThreadBase
  {
    ThreadRoutine routine = nullptr;
    void* arg = nullptr;
    ThreadResult result = {};
  };
}