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

  unsigned __stdcall Thread_Trampoline(void* thread);

  struct AURION_API ThreadDescription
  {
    HANDLE handle = nullptr;
    DWORD id = DWORD();
    ThreadRoutine routine = nullptr;
    void* arg = nullptr;
  };

  class AURION_API Thread : public IThread
  {
  public:
    explicit Thread(ThreadRoutine routine = nullptr, void* arg = nullptr);
    ~Thread() override;

    // Copying might introduce double-joins
    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;

    Thread(Thread&&) noexcept;
    Thread& operator=(Thread&&) noexcept;

    void Start() override;

    void Sleep(u32 ms) override;

    void Join() override;

    void* GetNativeHandle() override;
    const ThreadStatus& GetStatus() override;

  private:
    ThreadDescription m_desc;
    ThreadStatus m_status; // TODO: Make this Atomic
  };

#endif
}